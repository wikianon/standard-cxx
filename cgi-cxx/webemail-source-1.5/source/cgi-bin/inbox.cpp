#include "inbox.h"
#include "webemail.h"
#include "attachments.h"
#include "delete.h"
#include <vector>
#include "mbplugin.h"


//list contents of mail box
void index_inbox()
{
	char *from;
	char *subject;
	char status[50];
	int quota_on;
	int total;
	int count = 0; //used to see if we should show or not
	int start = 0;  //where to start the listing
	int size = 0;  //how many to list.
	int index_found = 0;
	char *env;
	char *cookie_env;
	char *index;
	char indexspace[100];
	char query[40];
	char *query_start;
	char *query_size;

	mb_list *list;
	mb_list *current;
	mb_message_header *head;

	//fetchmail
	char poppath[400];
	FILE *popfile;

	//check if we should only show part.
	//use url command line argument first
	if ((env = getenv("QUERY_STRING"))==NULL)
	{
		menu();
		return;
	}
	if (strlen(env) >= 12)
	{
		strncpy(query, env+12, 40);
		index_found = 1;
	}
	if (index_found == 0 && (cookie_env = getenv("HTTP_COOKIE")) != NULL)
	{
		index = finddata(100, cookie_env, "inbox_view=", indexspace);
		if (index != NULL)
		{
			strncpy(query, index, 40);
			index_found = 1;
		}
	}
	if (index_found == 1)
	{
		int i = 0;
		query_start = query;
		while (query[i] != '\0')
		{
			if (query[i] == '-')
			{
				query[i] = '\0';
				query_size = query+i+1;
			}
			i++;
		}
		start = atoi(query_start);
		size = atoi(query_size);
	}



	list = (*mb_get_message_list)(plugindata, MB_INBOX);

	cout << "Content-type: text/HTML" << endl;
	if (start != 0)
		cout << "Set-cookie: inbox_view=" << start << "-" << size << endl;
	cout << "Set-cookie: current_view=index_inbox" << endl;
	cout << "Pragma: no-cache" << endl;
	//else
	//	cout << "Set-cookie: inbox_view=" << start << "-" << size << "; expires=Wednesday, 09-JAN-1980 23:00:00 GMT" << endl;
	cout << endl;
	//cout << cookie_env;
	//cout << index;
	(*display_menu_head)(plugindata);

	if (list == NULL)
	{
		cout << "No Mail.<br><br>";
	}
	current = list;

	while (current != NULL)
	{
		if (current->data == NULL)
		{
			fprintf(stderr, "webemail:disinbox() Data error\n");
			exit(0);
		}
		head = current->data;

		if (size != 0 && head->hidden == MB_NOT_HIDDEN) //increase count
		{
			count++;
		}
		//show only between start and start+size
		if (size != 0 && (count < start || count >= start+size))
		{
			current = current->next;
			continue;
		}

		if (head->reply == MB_REPLIED)
		{
			strncpy(status, "replied", 50);
		}
		else if (head->read == MB_READ)
		{
			strncpy(status, "old", 50);
		}
		else
		{
			strncpy(status, "new", 50);
		}
		if (head->subject != NULL)
		{
	      		subject = head->subject;
		}
		else
		{
			subject = "";
		}
		if (head->from != NULL)
		{
	      		from = head->from;
		}
		else
		{
			from = "";
		}
		if (head->hidden == MB_NOT_HIDDEN)
			(*display_menu_entree)(plugindata, from, subject, status, head->id_number);
		current = current->next;

	}
	/*if (USEEXTERNMAIL)
	{
		snprintf(poppath, 400, "%s/.fetchmailrc",info->pw_dir);
		popfile = fopen(poppath, "r");
		if (popfile != NULL)
		{
			cout << "<a href=\"" << EMAILPATH << "?popcheck\">";
			cout << WEM_Check_external;
			cout << "</a><br>";

			fclose(popfile);
		}
	}*/


	quota_on = (*mb_over_quota)(plugindata, MB_INBOX, MAX_MBOX_SIZE);

	if (list == NULL)
		total = 0;
	else
		total = list->total_messages;

	(*display_menu_tail)(plugindata, quota_on, total, start, size);
	(*mb_free_message_list)(list);

	return;
}

//read actual message
//this one needs to replace sent and saved boxes
void read_inbox()
{
	mb_message *message;

	char *env;
	int number; 	//email to read
	passwd *info;

//needs to be removed once send plugin is installed.
	info = getpwnam(globaluser);
	if (info == NULL)
	{
		error("showinbox(): can't get uid info");
		return;
	}
//end here


	//get mail number
	if ((env = getenv("QUERY_STRING"))==NULL)
	{
		menu();
		return;
	}
	number = atoi(env+11);

	message = (*mb_get_message)(plugindata, MB_INBOX, number);
	if (message == NULL)
	{
		error("showinbox(): bad mail file.");
		return;
	}

	cout << "content-type: text/HTML" << endl;
	cout << "Pragma: No-Cache" << endl << endl;

	(*display_read_head)(plugindata, number);

//tries to prevent js attacks
	//distroytag(data, "</textarea");

	if (message->to != NULL)
	{
		removereturn(message->to);
		cout << "To: " << message->to << endl;
	}
	if (message->from != NULL)
	{
		removereturn(message->from);
		cout << "From: " << message->from << endl;
	}
	if (message->subject != NULL)
	{
		removereturn(message->subject);
		cout << "Subject: " << message->subject << endl;
	}
	if (message->date != NULL)
	{
		removereturn(message->date);
		cout << "Date: " << message->date << endl;
	}
	if (message->cc != NULL)
	{
		removereturn(message->cc);
		cout << "Cc: " << message->cc << endl;
	}


	if (message->message != NULL)
	{
		cout << message->message << endl;
	}

	(*display_read_middle)(plugindata, number);

	if (USEATTACHMENTS && message->num_attach > 0)
	{
		(*display_read_attachments_head)(plugindata, number);

		for (int loop = 1; loop <= message->num_attach; loop++)
		{
			char mode[20];
			if (message->ismime)
			{
				strncpy(mode, "mime", 20);
			}
			else
			{
				strncpy(mode, "uuencode", 20);
			}

			(*display_read_attachments)(plugindata, number, loop,
			cleanfilename(message->attach[loop]), mode);
		}

		(*display_read_attachments_tail)(plugindata, number);
	}

	(*display_read_tail)(plugindata, number);

 	//sends receipt
 	if (message->return_receipt_to != NULL
	&& message->read == MB_NOT_READ)
 	{
 		FILE * out;
 		out = popen(SENDMAIL, "w");
 		if (out != NULL) //if an error has occurred do not report it.
 		{


 			removereturn(message->return_receipt_to);
 			fprintf(out, "To: %s\n", message->return_receipt_to);
 			if (message->subject != NULL)
 			{
 				removereturn(message->subject);
 				fprintf(out, "Subject: Receipt: %s\n", message->subject);
 			}
 			else
 				fprintf(out, "Subject: Receipt\n");

 			//sending Return-Path
 			FILE *returnfile;
 			char returnpath[400];
 			char returnaddr[400];
 			snprintf(returnpath, 400, "%s/.webmail/return",info->pw_dir);
 			returnfile = fopen(returnpath, "r");
 			if (returnfile != NULL && USERRETURN)
 			{
 				for (int i = 0; i < 400; i++)
 				{
 					returnaddr[i] = fgetc(returnfile);
 					if (returnaddr[i] == EOF)
 					{
 						returnaddr[i] = '\0';
 						break;
 					}
 				}
 				fclose(returnfile);

 				fprintf(out, "Reply-to: <%s>\n", returnaddr);
 				fprintf(out, "From: %s\n", returnaddr);

 			}
 			else {
 				if (HOSTMODE == 2) //defined
 				{
 					fprintf(out, "Reply-to: <%s@%s>\n", info->pw_name, HOSTNAME);
 					fprintf(out, "From: %s@%s\n", info->pw_name, HOSTNAME);
 				}
 				else if (HOSTMODE == 1) //httpd
 				{
 					char *host;
 					if ((host = getenv("SERVER_NAME")) != NULL)
 					{
  					 	fprintf(out, "Reply-to: <%s@%s>\n", info->pw_name, host);
 					 	fprintf(out, "From: %s@%s\n", info->pw_name, host);
 				 	}
 					else
 						fprintf(out, "From: %s\n", info->pw_name);
 				}
 				else	//Use sendmail
 				{
 					 fprintf(out, "From: %s\n", info->pw_name);
 				}
 			}
 			fprintf(out, "\n");
 			fprintf(out, "Your message has been delivered.\n");
			if (pclose(out) != 0)
 			{
 			}
 			sync(); //needed or markold will erase the receipt, to yourself only.
 		}

 	}

 	if (message->read == MB_NOT_READ)
 		mb_mark(plugindata, MB_INBOX, number, MB_READ, MB_NO_OP);

	(*mb_free_message)(message);
	return;
}
void delete_inbox()
{
	char *env;
	int number;

	if ((env = getenv("QUERY_STRING"))==NULL)
	{
		error("deleteinbox(): Error reading system environment");
		return;
	}

	number = atoi(env+13);

 	(*mb_delete_message)(plugindata, MB_INBOX, number);

	disjavascript("", "Mail deleted", "menu");
	return;
}
void multi_del_inbox()
{
	char data;
	char number[10];
	vector <int> todel;  //cause I have to have dynamic storage
 	int *to_pass;  //cause I have to pass an array.

	int i = 0;
	int mark = 0;
	int k =0;

	while (1)
	{
		data = fgetc(stdin);
		if (data == EOF)
			break;

		if (data == '=')
		{
			mark = 0;
			number[k] = '\0';
			k = 0;
			todel.push_back(atoi(number));
		}

		if (mark == 1)
		{
			number[k] = data;
			k++;
			if (k > 9) //prevent a buffer overflow
				k = 9;
		}

		if (data == 'm')
		{
			mark=1;
			k=0;
		}
	}
	to_pass = new int[todel.size()];
	for (int i = 0; i < todel.size(); i++)
	{
		to_pass[i] = todel[i];
	}

	(*mb_multi_delete_message)(plugindata, MB_INBOX, to_pass, todel.size());
	delete to_pass;
	disjavascript("?index_inbox", "Mail deleted", "mail list");
	return;
}
void viewhtml_inbox()
{
	char *env;
	int number;
	mb_message *message;

	if ((env = getenv("QUERY_STRING"))==NULL)
	{
		menu();
		return;
	}
	number = atoi(env+15);

  	message = (*mb_get_message)(plugindata, MB_INBOX, number);
	if (message == NULL)
	{
		error("viewhtml_inbox():Error while trying to load message.");
		return;
	}

	cout << "content-type: text/HTML" << endl;
	cout << "Pragma: No-Cache" << endl << endl;

	(*display_view_html_head)(plugindata, number);

	if (message->message != NULL)
		cout << message->message;
	else
		cout << "";

	(*display_view_html_tail)(plugindata, number);
	(*mb_free_message)(message);

	return;

}
void viewprint_inbox()
{
	char *env;
	int number;
	mb_message *message;

	if ((env = getenv("QUERY_STRING"))==NULL)
	{
		menu();
		return;
	}
	number = atoi(env+16);
	message = (*mb_get_message)(plugindata, MB_INBOX, number);
	if (message == NULL)
	{
		error("viewprint_inbox(): Error accessing mail");
		return;
	}

	cout << "content-type: text/plain" << endl;
	cout << "Pragma: no-cache" << endl << endl;

	if (message->to != NULL)
	{
		removereturn(message->to);
		cout << "To: " << message->to << endl;
	}
	if (message->from != NULL)
	{
		removereturn(message->from);
		cout << "From: " << message->from << endl;
	}
	if (message->subject != NULL)
	{
		removereturn(message->subject);
		cout << "Subject: " << message->subject << endl;
	}
	if (message->date != NULL)
	{
		removereturn(message->date);
		cout << "Date: " << message->date << endl;
	}
	if (message->cc != NULL)
	{
		removereturn(message->cc);
		cout << "Cc: " << message->cc << endl;
	}
	if (message->reply_to != NULL)
	{
		removereturn(message->reply_to);
		cout << "Reply-to: " << message->reply_to << endl;
	}
	if (message->message != NULL)
	{
		cout << message->message;
	}
	(*mb_free_message)(message);

	return;
}
void u_attach_inbox()
{

	char *env;
	char *env2;
	int loop = 15;
	int attachnum;
	int mailnum;
	//char mailbox[400];
	//passwd *info;

	if ((env = getenv("QUERY_STRING"))==NULL)
	{
		disjavascript("", "Attach display error", "menu");
		return;
	}

	while (env[loop] != '\0')
	{
		if (env[loop] == 'x')
		{
			env[loop] = '\0';
			env2=env+loop+1;

		}
		loop++;
	}
	attachnum = atoi(env2);
	mailnum = atoi(env+15);
	//info=getpwnam(globaluser);
	//snprintf(mailbox, 400, MAILBOXPATH);

	//showattachment(mailbox, mailnum, attachnum);
	if ((*mb_get_attachment)(plugindata, MB_INBOX, mailnum, attachnum, stdout) != 0)
	{
		error("disattach(): error with plugin.");
	}


}

void m_attach_inbox()
{
	char *env;
	char *env2;
	int loop = 15;
	int attachnum;
	int mailnum;
	//char mailbox[400];
	//passwd *info;

	if ((env = getenv("QUERY_STRING"))==NULL)
	{
		disjavascript("?menusaved", "Attach display error", "menu");
		return;
	}

	while (env[loop] != '\0')
	{
		if (env[loop] == 'x')
		{
			env[loop] = '\0';
			env2=env+loop+1;

		}
		loop++;
	}
	attachnum = atoi(env2);
	mailnum = atoi(env+15);
	//info=getpwnam(globaluser);
	//snprintf(mailbox, 400, MAILBOXPATH);

	//mshowattachment(mailbox, mailnum, attachnum);
	if ((*mb_get_attachment)(plugindata, MB_INBOX, mailnum, attachnum, stdout) != 0)
	{
		error("mdisattach(): error with plugin.");
	}
}
void reply_inbox() //sets up reply then calls write
{
	char *env;
	int number = 0;
	if ((env = getenv("QUERY_STRING")) == NULL)
	{
		error("Reply_inbox(): ERROR! no environment");
		return;
	}
	number = atoi(env+12);

	wewrite(1, "", number, MB_INBOX);
}
void forward_inbox()
{
	char *env;
	int number = 0;
	if ((env = getenv("QUERY_STRING")) == NULL)
	{
		error("forward_inbox(): ERROR! no environment");
		return;
	}
	number = atoi(env+14);

	wewrite(3, "", number, MB_INBOX);
	return;
}
