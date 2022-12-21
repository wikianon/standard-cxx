#include "webemail.h"
#include "readconfig.h"

void intro() //login screen
{
	//char * browsertype;
	//if ((browsertype = getenv("HTTP_USER_AGENT"))==NULL)
	//{
	//	error("intro(): error reading browser type");
	//	return;
	//}



	cout << "content-type: text/HTML" << endl;
	cout << "Pragma: No-Cache" << endl << endl;

	(*display_login)(plugindata);

	return;
}
void wewrite(int mode, char *email, int reply, int mailbox_num) //compose/reply screen
{
	//mode 0 = normal, from scratch
	//mode 1 = reply
	//mode 2 = from address book
	//mode 3 = forward

	//mailbox_num, only used for realy and forwarding.

	FILE *mailstore;
	FILE *signature;
	mb_message *message;

	char signaturename[400];
	char hidden_line[400];

	passwd *info;
	int count = 0;
	char data[500];
	char subjectline[500];
	char finalsubject[500];  //subject with re: or fwd:

	wewrite_data *wewritedata;
	wewritedata = new wewrite_data;

	info=getpwnam(globaluser);
	snprintf(signaturename, 400, "%s/.signature", info->pw_dir);


	if (mode == 1 || mode == 3)  //finds From:/subject
	{			     //replying/forwarding
		message = (*mb_get_message)(plugindata, mailbox_num, reply);
		if (message == NULL)
		{
			error("wewrite(): Error accessing mail");
			return;
		}
	}

	if (mode == 1)
		wewritedata->mode = "Reply";  //sets a pointer to "reply"
	else if (mode == 3)
		wewritedata->mode = "Forward";
	else
		wewritedata->mode = "Send";
	wewritedata->cc = ""; //points to a null block

	cout << "content-type: text/HTML" << endl;
	cout << "Pragma: No-Cache" << endl << endl;

	//TO:
	if (mode == 1 && message->reply_to != NULL)
	{
		removereturn(message->reply_to);
		quotestospace(message->reply_to);
		commatospace(message->reply_to);
		wewritedata->to = message->reply_to;
	}
	else if (mode == 1 && message->from !=NULL)
	{
		removereturn(message->from);
		quotestospace(message->from);
		commatospace(message->from);
		wewritedata->to = message->from;
	}
	else if (mode == 2)
	{
		removereturn(email);
		wewritedata->to = email;
	}
	else
		wewritedata->to = "";


	//Subject:
	if (mode == 1 && message->subject != NULL)
	{
		removereturn(message->subject);
		strncpy(finalsubject, "Re: ", 499);
		strncpy(finalsubject+4, message->subject, 495);
		wewritedata->subject = finalsubject;
	}
	else if (mode == 3 && message->subject != NULL)
	{
		removereturn(message->subject);
		strncpy(finalsubject, "fwd: ", 499);
		strncpy(finalsubject+5, message->subject,494);
		wewritedata->subject = finalsubject;
	}
	else
	{
		strncpy(finalsubject, "", 499);
		wewritedata->subject = finalsubject;
	}


	//if repling tell sending function!!!
	if (mode == 1)
	{
		snprintf(hidden_line, 399,
		"<input type=\"HIDDEN\" name=\"replied\" value=\"%d\"><input type=\"HIDDEN\" name=\"mailbox\" value=\"%d\">\n", reply, mailbox_num);
	}
	else
		snprintf(hidden_line, 399, "<input type=\"HIDDEN\" name=\"replied\" value=\"0\">\n");
	wewritedata->hidden = hidden_line;

	(*display_write_head)(plugindata, wewritedata);

	//TEXT AREA

	if (mode == 1 || mode == 3)
	{
		cout << endl << endl;

		//tokenise the message
		int start = 0;
		int end = 0;
		int message_size = strlen(message->message);
		char * temp;
		int j;
		int n;
		for (int i = start; i < message_size; i++)
		{
			if (message->message[i] == '\n' || message->message[i] == '\0')
			{
				end = i;
				temp = new char[(end-start)+2];
				for (j = start, n = 0; j <= end; j++, n++)
				{
					temp[n] = message->message[j];
				}
				temp[(end-start)+1] = '\0';
				start = ++end; //move off token
				cout << ">" << temp;
				delete temp;
			}
		}

		(*mb_free_message)(message);
	}

	signature = fopen(signaturename, "r");
	if (signature != NULL)
	{
		if (bothlock(signature, 'r')==0)
		{
			cout << endl << endl;
			while(fgets(data, 499, signature) != NULL)
			{
				cout << data;
			}
		}
		bothunlock(signature);
		fclose(signature);
	}

	display_write_tail(plugindata, wewritedata);

	return;
}
void menu() //main menu
{
	char *env;
	char *cookie_env;
	char toshow[30];
	char *view;
	char viewspace[100];

	cout << "Content-type: text/HTML" << endl;
	cout << "Pragma: no-cache" << endl << endl;

	if ((env = getenv("QUERY_STRING")) != NULL)
	{
		if (strncmp(env, "menusaved", 9) == 0)
                {
			strncpy(toshow, "?index_saved", 30);
		}
		else if (strncmp(env, "menusent", 8) == 0)
                {
			strncpy(toshow, "?index_sent", 30);
		}
		else
		{
			strncpy(toshow, "?index_inbox", 30);
		}
	}
	else
	{
		strncpy(toshow, "?index_inbox", 30);
	}
	if ((cookie_env = getenv("HTTP_COOKIE")) != NULL)
	{
		view = finddata(100, cookie_env, "current_view=", viewspace);
		if (view != NULL)
		{
			snprintf(toshow, 30, "?%s", view);
		}
	}
	(*display_main_window)(plugindata, toshow);

	return;
}

void display_title()
{
	cout << "Content-type: text/HTML" << endl;
	cout << "Pragma: No-Cache" << endl << endl;

	(*display_main_title)(plugindata);

	return;
}
void display_menu()
{
	cout << "Content-type: text/HTML" << endl;
	cout << "Pragma: no-cache" << endl << endl;

	(*display_main_menu)(plugindata);

	return;
}
void display_conf()
{
	FILE *sign;
	FILE *forward;
	char signname[400];
	char signdata[500];
	char forwardname[400];
	char forwarddata[500];
	signdata[0] = '\0';
	forwarddata[0] = '\0';

	//fetchmail
	char popuser[400];
	char poppass[400];
	char popserver[400];
	FILE *popfile;
	char poppath[400];
	char popdata[400];
	int nopop = 0;

	//return address
	FILE *returnfile;
	char returnaddr[400];
	char returnname[400];
	returnaddr[0] = '\0';

	passwd *info;

	config_data *configdata;  //data to pass to plugin
	configdata = new config_data;
	configdata->show_pop = 0;
	configdata->show_forwarding = 0;
	configdata->show_return = 0;
	configdata->show_signature = 0;

	info=getpwnam(globaluser);

	//get forwarding info
	snprintf(forwardname, 400, "%s/.forward", info->pw_dir);
	configdata->show_forwarding=1;  //forwarding is always on. even if file
					//is null
 	forward = fopen(forwardname, "r");
	if (forward != NULL)
	{
		for (int i = 0; i < 500; i++)
		{
			forwarddata[i] = fgetc(forward);
			if (forwarddata[i] == EOF)
			{
				forwarddata[i] = '\0';
				break;
			}
		}
		fclose(forward);
	}
	configdata->forwarding_address = forwarddata;  //sets data


	if (USERRETURN)
	{
		configdata->show_return = 1; //allows user to set return path

		snprintf(returnname, 400, "%s/.webmail/return", info->pw_dir);
		returnfile = fopen(returnname, "r");
		if (returnfile != NULL)
		{
			for (int i = 0; i < 500; i++)
			{
				returnaddr[i] = fgetc(returnfile);
				if (returnaddr[i] == EOF)
				{
					returnaddr[i] = '\0';
					break;
				}
			}
			fclose(returnfile);
		}
		configdata->return_address = returnaddr;  //sets data
	}

	configdata->show_signature = 1;
	snprintf(signname, 400, "%s/.signature", info->pw_dir);
	sign = fopen(signname, "r");
	if (sign != NULL)
	{
		for (int i = 0; i < 500; i++)
		{
			signdata[i] = fgetc(sign);
			if (signdata[i] == EOF)
			{
				signdata[i] = '\0';
				break;
			}
		}
		fclose(sign);
	}
	configdata->signature = signdata; //sets data


	if(USEEXTERNMAIL)
	{
		configdata->show_pop = 1;
		configdata->nopop = 0;

		snprintf(poppath, 400, "%s/.fetchmailrc", info->pw_dir);
		popfile = fopen(poppath, "r");
		if (popfile != NULL)
		{
			fgets(popdata, 400, popfile);
			if (finddata(400, popdata, "popuser=", popuser) == NULL)
			{
				nopop = 1;
			}
			if (finddata(400, popdata, "poppass=", poppass) == NULL)
			{
				nopop = 1;
			}
			if (finddata(400, popdata, "popserver=", popserver) == NULL)
			{
				nopop = 1;
			}
			fclose(popfile);
		}
		else
			nopop = 1; //no .fetchmailrc
		removereturn(poppass);

		configdata->nopop = nopop;
		configdata->user = popuser;
		configdata->password = poppass;
		configdata->server = popserver;
	}

	cout << "content-type: text/HTML" << endl;
	cout << "Pragma: no-cache" << endl << endl;

	//call display config here.
	(*display_config)(plugindata, configdata);

	return;
}
void disjavascript(char *url, char *message1, char *message2)
{
	cout << "Content-type: text/html" << endl;
	cout << "Pragma: No-Cache" << endl << endl;

	(*display_java_script_head)(plugindata);

	cout << "<script>window.location=\"" << EMAILPATH << url << "\"</script>";
	cout << "<NOSCRIPT>" << message1 << ", click <a href=\"" << EMAILPATH << url << "\">here</a> ";
	cout << "to return to the " << message2 << ".<br>";
	cout << "To avoid seeing this message, enable javascript on your browser.</NOSCRIPT>";

	(*display_java_script_tail)(plugindata);

	return;
}
void popcheck()
{
	system("fetchmail -s 2> /dev/null 1> /dev/null"); //gets pop mail
	disjavascript("?index_inbox", "Mail checked", "mail list");
}



void escapeprint(char * data)
{
	int i = 0;
	while (data[i] != '\0')
	{
		if (data[i] == '<')
			cout << "&lt;";
		else if (data[i] == '>')
			cout << "&gt;";
		else if (data[i] == '&')
			cout << "&amp;";
		else if (data[i] == '\"')
			cout << "&quot;";
		else
			cout << data[i];
		i++;
	}
	return;
}



char* cleanfilename(char * data)
{
	char *end;

	//distroy any illege charators.
	for (int i = 0; data[i] != '\0'; i++)
	{
		if (data[i] == '"' || data[i] == '?' ||
		data[i] == ' ')
			data[i] = '_';
		if (data[i] == '\n')
			data[i] = '\0';

	}

	//remove _ from begining and end of filename 
	if (data[0] == '_')
	{
		for (int k = 0; data[k] != '\0'; k++)
		{
			data[k] = data[k+1]; 
		}
	}
	if (data[strlen(data)-1] == '_')
		data[strlen(data)-1] = '\0';

	end = data;

	for (int i = 0; data[i] != '\0'; i++)
	{
		if (data[i] == '\\' ||
		data[i] == '/' ||
		data[i] == ':')
		{
			end = data+i+1;
		}
	}
	return end;
}

void doconfig()
{
	char *browsertype;
	int browserpluginfound = 0;
	struct config_info *myconfig;
	struct config_info *current_config = NULL;
	struct data_entree *current_entree = NULL;

        if ((browsertype = getenv("HTTP_USER_AGENT"))==NULL)
        {
		cout << "Content-type: text/plain" << endl << endl;
		cout << "doconfig(): can't read browser type." << endl;
		cout << "Please upgrade your browser to one that supports http/1.1.";
                exit(0);
        }

	myconfig = read_config("/etc/webemail.conf");
	if (myconfig == NULL)
	{
		fputs ("doconfig(): No config file found!!!\n", stderr);
		cout << "Content-type: text/plain" << endl << endl;
		cout << "doconfig(): No config file found." << endl;
		cout << "Please contact your admin." << endl;
		exit (0);
	}

	//set defaults
	USEATTACHMENTS = 0;
	HOSTMODE = 3; //1=httpd, 2=define, 3=sendmail
	FOUNDNAME=0;  //used by define
	USEQUOTAS = 0;
	MAX_MBOX_SIZE = 0;
	int gotquotasize = 0;
	USEEXTERNMAIL = 0;
	USELINKHOME = 0;
	USELINKHOMENAME = 0;
	USEISPHELP=0;
	USEAUTHORINFO=0;
	USERRETURN=0;
	ALLOWROOT=0;
	SENDMAIL = new char[400];

	strncpy(SENDMAIL, "/usr/sbin/sendmail -t -i > /dev/null", 400);

	//blank out plugin data
	plugindata->server_mode='\0';
	plugindata->server_name='\0';
	plugindata->isp_url='\0';
	plugindata->main_url='\0';
	plugindata->link_text='\0';


	plugindata->use_attachments = 0;
	plugindata->use_externmail = 0;
	plugindata->use_authorinfo = 0;
	plugindata->userdefineablereturn = 0;
	plugindata->use_isp_help = 0;


	//setup nulls for the no value setting for the plugins.
	plugindata->mb_plugin = NULL;
	plugindata->mb_arg1 = NULL;
	plugindata->mb_arg2 = NULL;

	plugindata->display_plugin = NULL;
	plugindata->display_arg1 = NULL;
	plugindata->display_arg2 = NULL;

	current_config = myconfig;
	while(current_config != NULL)
	{
		current_entree = current_config->entree;
		if (current_entree == NULL)
		{
			current_config = current_config->next;
			continue;
		}

		if (strncasecmp(current_entree->data,
		"Attachments", 11) == 0)
		{
			current_entree = current_entree->next;

			if ((current_entree != NULL) &&
			(strncasecmp(current_entree->data, "on", 2) == 0))
			{
				plugindata->use_attachments = 1;
				USEATTACHMENTS = 1;
			}
		}
		else if (strncasecmp(current_entree->data,
		"AllowRootLogins", 15) == 0)
		{
			current_entree = current_entree->next;

			if ((current_entree != NULL) &&
			(strncasecmp(current_entree->data, "on", 2) == 0))
			{
				ALLOWROOT = 1;
			}
		}
		else if (strncasecmp(current_entree->data, 
		"SendMailPath", 12) ==0)
		{
			current_entree = current_entree->next;

			if (current_entree != NULL);
			{
				strncpy(SENDMAIL, current_entree->data, 400);
			}
		} 
		else if (strncasecmp(current_entree->data, 
		"UserDefineableReturn", 20) ==0)
		{
			current_entree = current_entree->next;

			if ((current_entree != NULL) &&
			(strncasecmp(current_entree->data, "on", 2) == 0))
			{
				plugindata->userdefineablereturn = 1;
				USERRETURN=1;
			}
		} 
		else if (strncasecmp(current_entree->data, 
		"GetHostName", 11) == 0)
		{
			current_entree = current_entree->next;
			if (current_entree != NULL)
			{
				plugindata->server_mode = new char[10]; 

				if (strncasecmp(current_entree->data, "httpd", 5)==0)
				{
					if((plugindata->server_name=getenv("SERVER_NAME"))==NULL)
  	 				{
						strncpy(plugindata->server_mode, "sendmail",10);
 						HOSTMODE=3;
			 		}
					else
					{
					strncpy(plugindata->server_mode, "httpd", 10);
 						HOSTMODE=1;
					}
	 			}
				else if (strncasecmp(current_entree->data, "define", 6)==0)
				{
					strncpy(plugindata->server_mode, "define", 10);
					HOSTMODE=2;
				}
				else if (strncasecmp(current_entree->data, "Sendmail", 8)==0)
				{
					strncpy(plugindata->server_mode, "sendmail", 10);
					HOSTMODE=3;
				}
				else
				{
					strncpy(plugindata->server_mode, "", 10);
				}
			}
		}
		else if (strncasecmp(current_entree->data, 
		"DefineHostName", 14) == 0)
		{
			current_entree = current_entree->next;

			if (current_entree != NULL)
			{
				FOUNDNAME=1;
				HOSTNAME = new char[strlen(current_entree->data)+1];
				strncpy(HOSTNAME, current_entree->data, strlen(current_entree->data)+1);
			}
		}
		else if (strncasecmp(current_entree->data, 
		"DefineLinkHome", 14) == 0)
		{
			current_entree = current_entree->next;

			if (current_entree != NULL)
			{
				USELINKHOME=1;
				LINKHOME = new char[strlen(current_entree->data)+1];
				strncpy(LINKHOME, current_entree->data, strlen(current_entree->data)+1);

				plugindata->main_url = new char[strlen(LINKHOME)+1];
				strncpy(plugindata->main_url,LINKHOME,strlen(LINKHOME)+1);
			}
		}
		else if (strncasecmp(current_entree->data, 
		"DefineLinkName", 14) == 0)
		{
			current_entree = current_entree->next;
			if (current_entree != NULL)
			{
				USELINKHOMENAME=1;
				removereturn(current_entree->data);
				LINKHOMENAME = new char[strlen(current_entree->data)+1];
				strncpy(LINKHOMENAME, current_entree->data, strlen(current_entree->data)+1);	

				plugindata->link_text =new char[strlen(LINKHOMENAME)+2];
				strncpy(plugindata->link_text, LINKHOMENAME,strlen(LINKHOMENAME));
			}
	  	}
 		else if (strncasecmp(current_entree->data,
		"UseQuotas", 9) == 0)
 		{
			current_entree = current_entree->next;

			if ((current_entree != NULL) &&
			(strncasecmp(current_entree->data, "on", 2) == 0))
			{
				USEQUOTAS = 1;
			}
		}
		else if (strncasecmp(current_entree->data, 
		"QuotaSize", 9) == 0)
		{
			current_entree = current_entree->next;
			
			if (current_entree != NULL)
			{
				MAX_MBOX_SIZE = atoi(current_entree->data);
				gotquotasize = 1;
			}
		}
		else if (strncasecmp(current_entree->data, 
		"SendMailPath", 12) == 0)
		{
			current_entree = current_entree->next;
			
			if (current_entree != NULL)
			{
				strncpy(SENDMAIL, current_entree->data, 400);
			}
		}
		else if (strncasecmp(current_entree->data, 
		"UseExternMail", 13) == 0)
		{
			current_entree = current_entree->next;

			if ((current_entree != NULL) &&
			(strncasecmp(current_entree->data, "on", 2) == 0))
			{
				plugindata->use_externmail = 1;
				USEEXTERNMAIL = 1;
			}
		}
		else if (strncasecmp(current_entree->data, 
		"UseAuthorInfo", 13) == 0)
		{
			current_entree = current_entree->next;

			if ((current_entree != NULL) &&
			(strncasecmp(current_entree->data, "on", 2) == 0))
			{
				plugindata->use_authorinfo = 1;
				USEAUTHORINFO = 1;
			}
		}
		else if (strncasecmp(current_entree->data,
		"DefineISPHelp", 13) == 0)
		{
			current_entree = current_entree->next;

			if (current_entree != NULL)
			{
				USEISPHELP=1;
				removereturn(current_entree->data);
				ISPHELP = new char[strlen(current_entree->data)+1];
				strncpy(ISPHELP, current_entree->data, strlen(current_entree->data)+1);

				plugindata->isp_url = new char[strlen(ISPHELP)+2];
				strncpy(plugindata->isp_url, ISPHELP, strlen(ISPHELP));
				plugindata->use_isp_help = 1;
			}
		}
		else if (strncasecmp(current_entree->data,
		"mbplugin", 8) == 0)
		{
			char *plugin = NULL;
			char *parg1 = NULL;
			char *parg2 = NULL;
			//set the default values

			current_entree = current_entree->next;
			if (current_entree != NULL)
			{
				plugin = current_entree->data;

				current_entree = current_entree->next;
				if (current_entree != NULL)
				{
					parg1 = current_entree->data;

					current_entree = current_entree->next;
					if (current_entree != NULL)
					parg2 = current_entree->data;
				}
			}
			if (plugindata->mb_plugin == NULL && plugin != NULL)
			{
				plugindata->mb_plugin = new char[strlen(plugin)+1];
				strncpy(plugindata->mb_plugin, plugin, strlen(plugin)+1);
				removereturn(plugindata->mb_plugin);
				//set the arguments only if the plugin is set.
				if (plugindata->mb_arg1 == NULL && parg1 != NULL)
				{
					plugindata->mb_arg1 = new char[strlen(parg1)+1];
					strncpy(plugindata->mb_arg1, parg1, strlen(parg1)+1);
					removereturn(plugindata->mb_arg1);
				}
				if (plugindata->mb_arg2 == NULL && parg2 != NULL)
				{
					plugindata->mb_arg2 = new char[strlen(parg2)+1];
					strncpy(plugindata->mb_arg2, parg2, strlen(parg2)+1);
					removereturn(plugindata->mb_arg2);
				}
			}

		}
		else if (strncasecmp(current_entree->data,
		"addrplugin", 10) == 0)
		{
			char *plugin = NULL;
			char *parg1 = NULL;
			char *parg2 = NULL;
			//set the default values

			current_entree = current_entree->next;
			if (current_entree != NULL)
			{
				plugin = current_entree->data;

				current_entree = current_entree->next;
				if (current_entree != NULL)
				{
					parg1 = current_entree->data;

					current_entree = current_entree->next;
					if (current_entree != NULL)
					parg2 = current_entree->data;
				}
			}
			if (plugindata->addr_plugin == NULL && plugin != NULL)
			{
				plugindata->addr_plugin = new char[strlen(plugin)+1];
				strncpy(plugindata->addr_plugin, plugin, strlen(plugin)+1);
				removereturn(plugindata->addr_plugin);
				//set the arguments only if the plugin is set.
				if (plugindata->addr_arg1 == NULL && parg1 != NULL)
				{
					plugindata->addr_arg1 = new char[strlen(parg1)+1];
					strncpy(plugindata->addr_arg1, parg1, strlen(parg1)+1);
					removereturn(plugindata->addr_arg1);
				}
				if (plugindata->addr_arg2 == NULL && parg2 != NULL)
				{
					plugindata->addr_arg2 = new char[strlen(parg2)+1];
					strncpy(plugindata->addr_arg2, parg2, strlen(parg2)+1);
					removereturn(plugindata->addr_arg2);
				}
			}

		}
		else if (strncasecmp(current_entree->data,
		"browsertype", 11) == 0)
		{
			char *browser = NULL;
			char *plugin = NULL;
			char *parg1 = NULL;
			char *parg2 = NULL;

			current_entree = current_entree->next;
			if (current_entree != NULL)
			{
				browser = current_entree->data;

				current_entree = current_entree->next;
				if (current_entree != NULL)
				{
					plugin = current_entree->data;

					current_entree = current_entree->next;
					if (current_entree != NULL)
					{
						parg1 = current_entree->data;

						current_entree = current_entree->next;
						if (current_entree != NULL)
						{
							parg2 = current_entree->data;
						}
					}
				}
			}
			if (browserpluginfound == 0 && strncasecmp(browser, "other", strlen(browser)) == 0)
			{//set the default if it finds it.
				plugindata->display_plugin = new char[strlen(plugin)+1];
				strncpy(plugindata->display_plugin, plugin, strlen(plugin)+1);
				removereturn(plugindata->display_plugin);

				if (parg1 != NULL)
				{
					plugindata->display_arg1 = new char[strlen(parg1)+1];
					strncpy(plugindata->display_arg1, parg1, strlen(parg1)+1);
					removereturn(plugindata->display_arg1);
				}
				if (parg2 != NULL)
				{
					plugindata->display_arg2 = new char[strlen(parg2)+1];
					strncpy(plugindata->display_arg2, parg2, strlen(parg2)+1);
					removereturn(plugindata->display_arg2);
				}

				browserpluginfound = 1;
			}
			if (strncasecmp(browser, browsertype, strlen(browser)) == 0)
			{//set the browser type.

				plugindata->display_plugin = new char[strlen(plugin)+1];
				strncpy(plugindata->display_plugin, plugin, strlen(plugin)+1);
				removereturn(plugindata->display_plugin);

				if (parg1 != NULL)
				{
					plugindata->display_arg1 = new char[strlen(parg1)+1];
					strncpy(plugindata->display_arg1, parg1, strlen(parg1)+1);
					removereturn(plugindata->display_arg1);
				}
				if (parg2 != NULL)
				{
					plugindata->display_arg2 = new char[strlen(parg2)+1];
					strncpy(plugindata->display_arg2, parg2, strlen(parg2)+1);
					removereturn(plugindata->display_arg2);
				}

				browserpluginfound = 1;
			}

		}
		else
		{
		}
		current_config = current_config->next;

	}

	if (browserpluginfound == 0)
	{
		fputs("Error can't find plugin for broswer", stderr);
		cout << "content-type: text/plain" << endl << endl;
		cout << "Error can't find plugin for broswer";
		exit(0);
	}

	//some tests
	if (HOSTMODE == 2 && FOUNDNAME == 0)// defined name, but no name
	{
		HOSTMODE = 3;
		strncpy(plugindata->server_mode, "sendmail", 10);
	}
	if (HOSTMODE == 2) //setup defined name
	{
		plugindata->server_name = new char[strlen(HOSTNAME)+2];
		strncpy(plugindata->server_name, HOSTNAME, strlen(HOSTNAME));
	}
	if (USEQUOTAS == 1 && gotquotasize == 0)
	{
		USEQUOTAS = 0;
	}

	free_config(myconfig);
	return;
}

