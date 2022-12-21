#include "sent.h"
#include "webemail.h"
#include "attachments.h"
#include "delete.h"
#include <vector>

void index_sent()
{
	char *from;
	char *to;
	char *subject;
	char status[50];
	int quota_on;
	int total;
	int count = 0;
	int start = 0;
	int size = 0;
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

	//check if we should only show part.
	//use url command line argument first
	if ((env = getenv("QUERY_STRING"))==NULL)
	{
		menu();
		return;
	}
	if (strlen(env) >= 11)
	{
		strncpy(query, env+11, 40);
		index_found = 1;
	}
	if (index_found == 0 && (cookie_env = getenv("HTTP_COOKIE")) != NULL)
	{
		index = finddata(100, cookie_env, "sent_view=", indexspace);
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

	list = (*mb_get_message_list)(plugindata, MB_SENT);

	cout << "Content-type: text/HTML" << endl;
	if (start != 0)
		cout << "Set-cookie: sent_view=" << start << "-" << size << endl;
	cout << "Set-cookie: current_view=index_sent" << endl;
	cout << "Pragma: no-cache" << endl;
	cout << endl;

	(*display_sent_menu_head)(plugindata);

	if (list == NULL)
	{
		cout << "No sent mail.<br><br>";
	}
	current = list;

	while (current != NULL)
	{
		if (current->data == NULL)
		{
			fprintf(stderr, "webemail:display_sent() Data error\n");
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

		strncpy(status, "old", 50);

		if (head->subject != NULL)
		{
	      		subject = head->subject;
		}
		else
		{
			subject = "";
		}
		if (head->to != NULL)
		{
	      		to = head->to;
		}
		else
		{
			to = "";
		}
		if (head->hidden == MB_NOT_HIDDEN)
			(*display_sent_menu_entree)(plugindata, to, subject, status, head->id_number);
		current = current->next;

	}

	if (list == NULL)
		total = 0;
	else
		total = list->total_messages;

	(*display_sent_menu_tail)(plugindata, quota_on, total, start, size);
	(*mb_free_message_list)(list);

	return;
}
void read_sent()
{
	mb_message *message;

	char *env;
	int number; 	//email to read

	//get mail number
	if ((env = getenv("QUERY_STRING"))==NULL)
	{
		menu(); //ttd don't know what to put here?
		return;
	}
	number = atoi(env+10);

	message = (*mb_get_message)(plugindata, MB_SENT, number);
	if (message == NULL)
	{
		error("showsent(): bad mail file.");
		return;
	}

	cout << "content-type: text/HTML" << endl;
	cout << "Pragma: No-Cache" << endl << endl;

	(*display_sent_read_head)(plugindata, number);

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

	(*display_sent_read_middle)(plugindata, number);

	if (USEATTACHMENTS && message->num_attach > 0)
	{
		(*display_sent_read_attachments_head)(plugindata, number);

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

                        (*display_sent_read_attachments)(plugindata, number, loop,
                        cleanfilename(message->attach[loop]), mode);

		}

		(*display_sent_read_attachments_tail)(plugindata, number);
	}

	(*display_sent_read_tail)(plugindata, number);
	(*mb_free_message)(message);

	return;
}
void delete_sent()
{
	char *env;
	int number;

	if ((env = getenv("QUERY_STRING"))==NULL)
	{
		error("deletesent(): Error reading system environment");
		return;
	}

	number = atoi(env+12);

 	(*mb_delete_message)(plugindata, MB_SENT, number);

	disjavascript("?menusent", "Sent mail deleted", "menu");
	return;
}
void multi_del_sent()
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

	(*mb_multi_delete_message)(plugindata, MB_SENT, to_pass, todel.size());
	delete to_pass;
	disjavascript("?index_sent", "Sent mail deleted", "mail list");
	return;
}
void viewhtml_sent()
{
	char *env;
	int number;
	mb_message *message;

	if ((env = getenv("QUERY_STRING"))==NULL)
	{
		menu();
		return;
	}
	number = atoi(env+14);

  	message = (*mb_get_message)(plugindata, MB_SENT, number);
	if (message == NULL)
	{
		error("viewhtml_sent():Error while trying to load message.");
		return;
	}

	cout << "content-type: text/HTML" << endl;
	cout << "Pragma: No-Cache" << endl << endl;

	(*display_sent_view_html_head)(plugindata, number);

	if (message->message != NULL)
		cout << message->message;
	else
		cout << "";

	(*display_sent_view_html_tail)(plugindata, number);
	(*mb_free_message)(message);

	return;

}

void viewprint_sent()
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
	message = (*mb_get_message)(plugindata, MB_SENT, number);
	if (message == NULL)
	{
		error("viewprint_sent(): Error accessing mail");
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


void u_attach_sent()
{

	char *env;
	char *env2;
	int loop = 14;
	int attachnum;
	int mailnum;
	//char mailbox[400];
	//passwd *info;

	if ((env = getenv("QUERY_STRING"))==NULL)
	{
		disjavascript("?menusent", "Attach display error", "menu");
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
	mailnum = atoi(env+14);
	//info=getpwnam(globaluser);
	//snprintf(mailbox, 400, "%s/.webmail/sent", info->pw_dir);

	//showattachment(mailbox, mailnum, attachnum);
	if ((*mb_get_attachment)(plugindata, MB_SENT, mailnum, attachnum, stdout) != 0)
	{
		error("showsentattach(): error with plugin.");
	}
}
void m_attach_sent()
{
	char *env;
	char *env2;
	int loop = 14;
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
	mailnum = atoi(env+14);
	//info=getpwnam(globaluser);

	//snprintf(mailbox, 400, "%s/.webmail/sent", info->pw_dir);

	//mshowattachment(mailbox, mailnum, attachnum);
	if ((*mb_get_attachment)(plugindata, MB_SENT, mailnum, attachnum, stdout) != 0)
	{
		error("mshowsentattach(): error with plugin.");
	}
}


