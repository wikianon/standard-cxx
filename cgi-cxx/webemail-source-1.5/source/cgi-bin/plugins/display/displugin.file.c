#include "../../plugin.h"
#include "../../displugin.h"
#include "../../addrplugin.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//#define HTML_PATH "/home/httpd/html/email/"
#define NAME info->display_arg2

struct word_list {
	struct word_list *next;
	char *key;
	char *entree;
};

void* append_to(char * dest, char *src)
{
	if (dest != NULL)
	{
		dest = realloc(dest, strlen(dest)+strlen(src)+1);
		strncpy(dest+strlen(dest), src, strlen(src)+1);
	}
	else
	{
		dest = malloc(strlen(src)+1);
		strncpy(dest, src, strlen(src)+1);
	}
	return dest;
}
//takes care of problem char actors.
void display_escapeprint(char * data)
{
        int i = 0;
        while (data[i] != '\0')
        {
                if (data[i] == '<')
                        printf("%s", "&lt;");
                else if (data[i] == '>')
                        printf("%s", "&gt;");
                else if (data[i] == '&')
                        printf("%s", "&amp;");
                else if (data[i] == '\"')
                        printf("%s", "&quot;");
                else if (data[i] == ' ')
                        printf("%s", "&nbsp;");
                else
                        printf("%c", data[i]);
                i++;
        }
        return;
}

//adds one word at a time to a list
struct word_list * add_word_to_list(struct word_list *list, char *key, char *entree)
{
	struct word_list *toadd;
	toadd = malloc(sizeof(struct word_list));

	toadd->next = list;

	toadd->key = malloc(strlen(key)+1);
	strcpy(toadd->key, key);

	if (entree != NULL)
	{
		toadd->entree = malloc(strlen(entree)+1);
		strcpy(toadd->entree, entree);
	}
	else 
	{// entree is null so put in a space
		toadd->entree = malloc(strlen(" ")+1);
		strcpy(toadd->entree, "");
	}	
	return toadd;
}
//deletes a list
void del_list(struct word_list *list)
{
	if (list != NULL)
	{
		free(list->key);
		free(list->entree);
		del_list(list->next);
		free(list);
	}
	return;
}
//adds the plugin_data to a list
struct word_list *add_data_to_list(struct word_list *transdata, struct plugin_data *info)
{
	transdata = add_word_to_list(transdata, "user_name", info->user_name);
	transdata = add_word_to_list(transdata, "url", info->url);
	transdata = add_word_to_list(transdata, "server_name", info->server_name);
	transdata = add_word_to_list(transdata, "isp_url", info->isp_url);
	transdata = add_word_to_list(transdata, "main_url", info->main_url);
	transdata = add_word_to_list(transdata, "link_text", info->link_text);
	transdata = add_word_to_list(transdata, "web_email_support", info->web_email_support);
	transdata = add_word_to_list(transdata, "web_email_ver", info->web_email_ver);

	return transdata;
}

//prints out a webpage with a translation list
void filter_print(char *filename, struct word_list *list)
{
	FILE *datafile;
	char line[450]; //only reading in 400, but I have a 50 buffer so when I compare
			//and I go beyond the 400 I don't run in to problems.
	int size = 0;
	int i = 0;
	struct word_list *current = list;

	datafile = fopen(filename, "r");

	if (datafile == NULL)
	{	
		printf("<html><body>\n");
		printf("Plugin file not found!<br>\n");
		printf("%s", filename);
		printf("</body></html>");
		return;
	}

	while (fgets(line, 400, datafile) != NULL)
	{
		i = 0;
		size = strlen(line);
		while (i < size)
		{
			if (strncmp("<!-#replace ", line+i, 12)==0) //this is where the 50 buffer comes in
			{
				i+=12;
				current = list;
				while (current != NULL)
				{
					if (strncmp(current->key, line+i, strlen(current->key))==0 &&
					line[strlen(current->key)+i] == '>')
					{
						if (strncmp(current->key, "hidden", 6)==0 ||
						strncmp(current->key, "pop_check", 9)==0 ||
						strncmp(current->key, "index", 5)==0)
							printf("%s", current->entree);
						else
							display_escapeprint(current->entree); //htmlize the out put
						i += strlen(current->key);
						i++; //skip the >
						break; 
					}
					current = current->next;
				}
			}
			else
			{
				printf("%c", line[i]);
				i++;
			}
		} 
	}

	return;
}


//--internal functions, not part of the interface--------------







//---------------------------------------------------------
//---------------------------------------------------------
//---------------------------------------------------------
//---------------------------------------------------------




//-- General ---------------------------------

int get_interface_version()
{
	return 4;  //interface version 2
}
int get_plugin_subsystem()
{
	return 1; //the subsystem for the plugin, always 1
}

double get_plugin_version() //plugins version
{
	return 0.9;
}

char* get_language() //reports the language 
{
	char *type;
	type = malloc(3);  //must make on heap or on a plugin unload problems could arise.
	strncpy(type, "??", 3);
	return type;  //return english
}
char* get_plugin_name() //report plug in name
{
	char *name;
	name = malloc(20);
	strncpy(name, "File (devel)", 19);
	return name;
}

//-- Main -------------------------------------

void display_main_window(struct plugin_data *info, char *toshow) //probably aframe
{
	char path[400];
	struct word_list *transdata = NULL;
	
	snprintf(path, 400, "%s%s/main_window.html", info->display_arg1, NAME);

	transdata = add_data_to_list(transdata, info);
	transdata = add_word_to_list(transdata, "toshow", toshow);

	filter_print(path, transdata);
	del_list(transdata);

	return;
}
void display_main_title(struct plugin_data *info)
{
	char path[400];
	struct word_list *transdata = NULL;

	snprintf(path, 400, "%s%s/main_title.html", info->display_arg1, NAME);

	transdata = add_data_to_list(transdata, info);
	filter_print(path, transdata);
	del_list(transdata);
	
	return;
}
void display_main_menu(struct plugin_data *info)
{
	char path[400];
	struct word_list *transdata = NULL;

	snprintf(path, 400, "%s%s/main_menu.html", info->display_arg1, NAME);

	transdata = add_data_to_list(transdata, info);
	filter_print(path, transdata);
	del_list(transdata);

	return;
}

//-- Inbox ------------------------------------

void display_menu_head(struct plugin_data *info)
{
	char path[400];
	struct word_list *transdata = NULL;

	snprintf(path, 400, "%s%s/menu_head.html", info->display_arg1, NAME);

	transdata = add_data_to_list(transdata, info);
	filter_print(path, transdata);
	del_list(transdata);

	return;

}
void display_menu_entree(struct plugin_data *info,
char *from, char *subject, char *status, int message_number)
//status, new, old, replied
{
	if (strcmp(status, "replied") == 0)
	{//replied need if
		printf("<img SRC=\"/email/%s/replied.jpeg\" ALT=\"(Replied)\" ", NAME);
		printf("%s", "BORDER=0 height=14 width=20>\n");
	}
	else if (strcmp(status, "old") == 0)
	{//old need if
		printf("<img SRC=\"/email/%s/old.jpeg\" ALT=\"(Old)\" ", NAME);
		printf("%s", "BORDER=0 height=14 width=20>\n");
	}
	else
	{//new
		printf("<img SRC=\"/email/%s/new.jpeg\" ALT=\"(New)\" ", NAME);
		printf("%s", "BORDER=0 height=14 width=20>\n");
	}

	printf("%s", "&nbsp;<input TYPE=\"checkbox\" NAME=\"m");
	printf("%d", message_number);
	printf("%s", "\">&nbsp;\n");
	printf("%s", "<a href=\"");
	printf("%s", info->url);
	printf("?read_inbox=%d", message_number);
	printf("%s", "\" TARGET=_top>\n");

	display_escapeprint(from);

	printf("%s", "</a>\n");
	if (subject != '\0')
	{
		printf("%s", "<br>&nbsp;&nbsp;");
		display_escapeprint(subject);
		printf("\n");
	}   

	printf("%s", "&nbsp;&nbsp;<a href=\"");
	printf("%s", info->url);
	printf("?delete_inbox=%d", message_number);
	printf("%s", "\" TARGET=_top>");

	printf("%s", "Delete</a><br><br>\n");

	return;
}
void display_menu_tail(struct plugin_data *info, int quota_on, int total_messages,
int current_start_mes, int current_block_size)
{
	char path[400];
	char number[15];

	char *index = NULL;
	char temp[400];
	int divs;
	int extra;
	int lower;
	int upper;
	int i;

	char popcheck[300];
	struct word_list *transdata = NULL;

	divs = total_messages / 10;
	extra = total_messages % 10;
	for (i = 0; i < divs; i++)
	{
		upper = (i*10)+10;
		lower = (i*10)+1;
		if (current_start_mes >= lower && current_start_mes <= upper)
			snprintf(temp, 400, "(%d-%d)&nbsp;", (i*10)+1, (i*10)+10);
		else
		{
			snprintf(temp, 400, "<a href=\"%s?index_inbox=%d-%d\">%d-%d</a>&nbsp;",
			 info->url,(i*10+1), 10, (i*10+1), (i*10+10));
		}
		index = append_to(index, temp);
	}
	if (extra > 0)
	{
		if (current_start_mes >= (divs*10+1))
			snprintf(temp, 400, "(%d-%d)&nbsp;", (i*10+1), total_messages);
		else
       		{
			snprintf(temp, 400, "<a href=\"%s?index_inbox=%d-%d\">%d-%d</a>&nbsp;",
			 info->url,(i*10+1), 10, (i*10+1), total_messages);
		}
		index = append_to(index, temp);
	}


	snprintf(number, 15, "%d", total_messages);
	snprintf(path, 400, "%s%s/menu_tail.html", info->display_arg1, NAME);


	transdata = add_data_to_list(transdata, info);
	transdata = add_word_to_list(transdata, "count", number);
	transdata = add_word_to_list(transdata, "index", index);


	if (info->use_externmail == 1)
        {
		snprintf(popcheck, 300, "<a href=\"%s?popcheck\">Check external mailbox</a><br>",info->url);
        }
	else
	{
		snprintf(popcheck, 300, "");
	}
	transdata = add_word_to_list(transdata, "pop_check", popcheck);

	free(index);
	filter_print(path, transdata);
	del_list(transdata);

	return;
}

//-- Inbox saved------------------------------------

void display_saved_menu_head(struct plugin_data *info)
{
	char path[400];
	struct word_list *transdata = NULL;

	snprintf(path, 400, "%s%s/saved_menu_head.html", info->display_arg1, NAME);

	transdata = add_data_to_list(transdata, info);
	filter_print(path, transdata);
	del_list(transdata);

	return;
}
void display_saved_menu_entree(struct plugin_data *info,
char *from, char *subject, char *status, int message_number)
//status, new, old, replied
{
	if (strcmp(status, "replied") == 0)
	{//replied need if
		printf("<img SRC=\"/email/%s/replied.jpeg\" ALT=\"(Replied)\" ", NAME);
		printf("%s", "BORDER=0 height=14 width=20>\n");
	}
	else if (strcmp(status, "old") == 0)
	{//old need if
		printf("<img SRC=\"/email/%s/old.jpeg\" ALT=\"(Old)\" ", NAME);
		printf("%s", "BORDER=0 height=14 width=20>\n");
	}
	else
	{//new
		printf("<img SRC=\"/email/%s/new.jpeg\" ALT=\"(New)\" ", NAME);
		printf("%s", "BORDER=0 height=14 width=20>\n");
	}

	printf("%s", "&nbsp;<input TYPE=\"checkbox\" NAME=\"m");
	printf("%d", message_number);
	printf("%s", "\">&nbsp;\n");
	printf("%s", "<a href=\"");
	printf("%s", info->url);
	printf("?read_saved=%d", message_number);
	printf("%s", "\" TARGET=_top>\n");

	display_escapeprint(from);

	printf("%s", "</a>\n");
	if (subject != '\0')
	{
		printf("%s", "<br>&nbsp;&nbsp;");
		display_escapeprint(subject);
		printf("\n");
	}

	printf("%s", "&nbsp;&nbsp;<a href=\"");
	printf("%s", info->url);
	printf("?delete_saved=%d", message_number);
	printf("%s", "\" TARGET=_top>");

	printf("%s", "Delete</a><br><br>\n");

	return;

}
void display_saved_menu_tail(struct plugin_data *info, int quota_on, int total_messages,
int current_start_mes, int current_block_size)
{
	char path[400];
	char number[15];

	char *index = NULL;
	char temp[400];
	int divs;
	int extra;
	int lower;
	int upper;
	int i;

	struct word_list *transdata = NULL;

	divs = total_messages / 10;
	extra = total_messages % 10;
	for (i = 0; i < divs; i++)
	{
		upper = (i*10)+10;
		lower = (i*10)+1;
		if (current_start_mes >= lower && current_start_mes <= upper)
			snprintf(temp, 400, "(%d-%d)&nbsp;", (i*10)+1, (i*10)+10);
		else
		{
			snprintf(temp, 400, "<a href=\"%s?index_saved=%d-%d\">%d-%d</a>&nbsp;",
			 info->url,(i*10+1), 10, (i*10+1), (i*10+10));
		}
		index = append_to(index, temp);
	}
	if (extra > 0)
	{
		if (current_start_mes >= (divs*10+1))
			snprintf(temp, 400, "(%d-%d)&nbsp;", (i*10+1), total_messages);
		else
       		{
			snprintf(temp, 400, "<a href=\"%s?index_saved=%d-%d\">%d-%d</a>&nbsp;",
			 info->url,(i*10+1), 10, (i*10+1), total_messages);
		}
		index = append_to(index, temp);
	}



	snprintf(number, 15, "%d", total_messages);
	snprintf(path, 400, "%s%s/saved_menu_tail.html", info->display_arg1, NAME);

	transdata = add_data_to_list(transdata, info);
	transdata = add_word_to_list(transdata, "count", number);
	transdata = add_word_to_list(transdata, "index", index);

	free(index);
	filter_print(path, transdata);
	del_list(transdata);

	return;
}

//-- Inbox sent ------------------------------------

void display_sent_menu_head(struct plugin_data *info)
{
	char path[400];
	struct word_list *transdata = NULL;

	snprintf(path, 400, "%s%s/sent_menu_head.html", info->display_arg1, NAME);

	transdata = add_data_to_list(transdata, info);
	filter_print(path, transdata);
	del_list(transdata);

	return;
}

void display_sent_menu_entree(struct plugin_data *info,
char *to, char *subject, char *status, int message_number)
//status, new, old, replied
{

	if (strcmp(status, "replied") == 0)
	{//replied need if
		printf("<img SRC=\"/email/%s/replied.jpeg\" ALT=\"(Replied)\" ", NAME);
		printf("%s", "BORDER=0 height=14 width=20>\n");
	}
	else if (strcmp(status, "old") == 0)
	{//old need if
		printf("<img SRC=\"/email/%s/old.jpeg\" ALT=\"(Old)\" ", NAME);
		printf("%s", "BORDER=0 height=14 width=20>\n");
	}
	else
	{//new
		printf("<img SRC=\"/email/%s/new.jpeg\" ALT=\"(New)\" ", NAME);
		printf("%s", "BORDER=0 height=14 width=20>\n");
	}

	printf("%s", "&nbsp;<input TYPE=\"checkbox\" NAME=\"m");
	printf("%d", message_number);
	printf("%s", "\">&nbsp;\n");
	printf("%s", "<a href=\"");
	printf("%s", info->url);
	printf("?read_sent=%d", message_number);
	printf("%s", "\" TARGET=_top>\n");

	display_escapeprint(to);

	printf("%s", "</a>\n");
	if (subject != '\0')
	{
		printf("%s", "<br>&nbsp;&nbsp;");
		display_escapeprint(subject);
		printf("\n");
	}

	printf("%s", "&nbsp;&nbsp;<a href=\"");
	printf("%s", info->url);
	printf("?delete_sent=%d", message_number);
	printf("%s", "\" TARGET=_top>");

	printf("%s", "Delete</a><br><br>\n");

	return;
}
void display_sent_menu_tail(struct plugin_data *info, int quota_on, int total_messages,
int current_start_mes, int current_block_size)
{
	char path[400];
	char number[15];

	char *index = NULL;
	char temp[400];
	int divs;
	int extra;
	int lower;
	int upper;
	int i;

	struct word_list *transdata = NULL;

	divs = total_messages / 10;
	extra = total_messages % 10;
	for (i = 0; i < divs; i++)
	{
		upper = (i*10)+10;
		lower = (i*10)+1;
		if (current_start_mes >= lower && current_start_mes <= upper)
			snprintf(temp, 400, "(%d-%d)&nbsp;", (i*10)+1, (i*10)+10);
		else
		{
			snprintf(temp, 400, "<a href=\"%s?index_sent=%d-%d\">%d-%d</a>&nbsp;",
			 info->url,(i*10+1), 10, (i*10+1), (i*10+10));
		}
		index = append_to(index, temp);
	}
	if (extra > 0)
	{
		if (current_start_mes >= (divs*10+1))
			snprintf(temp, 400, "(%d-%d)&nbsp;", (i*10+1), total_messages);
		else
       		{
			snprintf(temp, 400, "<a href=\"%s?index_sent=%d-%d\">%d-%d</a>&nbsp;",
			 info->url,(i*10+1), 10, (i*10+1), total_messages);
		}
		index = append_to(index, temp);
	}

	snprintf(number, 15, "%d", total_messages);
	snprintf(path, 400, "%s%s/sent_menu_tail.html", info->display_arg1, NAME);

	transdata = add_data_to_list(transdata, info);
	transdata = add_word_to_list(transdata, "count", number);
	transdata = add_word_to_list(transdata, "index", index);

	free(index);
	filter_print(path, transdata);
	del_list(transdata);

	return;
}


//-- Read -------------------------------------

//display everything up to message
void display_read_head(struct plugin_data *info, int message_number)
{

	char path[400];
	char number[15];
	struct word_list *transdata = NULL;

	snprintf(number, 15, "%d", message_number);
	snprintf(path, 400, "%s%s/read_head.html", info->display_arg1, NAME);

	transdata = add_data_to_list(transdata, info);
	transdata = add_word_to_list(transdata, "message_number", number);

	filter_print(path, transdata);
	del_list(transdata);

	return;
}

//display everything after message but before attachments
void display_read_middle(struct plugin_data *info, int message_number)
{
	char path[400];
	char number[15];
	struct word_list *transdata = NULL;

	snprintf(number, 15, "%d", message_number);
	snprintf(path, 400, "%s%s/read_middle.html", info->display_arg1, NAME);

	transdata = add_data_to_list(transdata, info);
	transdata = add_word_to_list(transdata, "message_number", number);

	filter_print(path, transdata);
	del_list(transdata);

	return;
}

void display_read_attachments_head(struct plugin_data *info, int message_number)
{
	char path[400];
	char number[15];
	struct word_list *transdata = NULL;

	snprintf(number, 15, "%d", message_number);
	snprintf(path, 400, "%s%s/read_attachments_head.html", info->display_arg1, NAME);

	transdata = add_data_to_list(transdata, info);
	transdata = add_word_to_list(transdata, "message_number", number);

	filter_print(path, transdata);
	del_list(transdata);

	return;
}

//display attachments
void display_read_attachments(struct plugin_data *info, int message_number,
int attachment_number, char *file_name, char *mode)
{
	printf("%s", "<a href=\"");
	printf("%s", "./attachments/");
	printf("%s", file_name);

	if (strcmp(mode, "mime") == 0)
	{
		printf("%s", "?m_attach_inbox=");
	}
	else
	{
		printf("%s", "?u_attach_inbox=");
	}
	printf("%d", message_number);
	printf("%s", "x");

	printf("%d", attachment_number);
	printf("%s", "\">");
	printf("%s", file_name);
	printf("%s", "</a><br>");
	return;
}

void display_read_attachments_tail(struct plugin_data *info, int
message_number)
{
	char path[400];
	char number[15];
	struct word_list *transdata = NULL;

	snprintf(number, 15, "%d", message_number);
	snprintf(path, 400, "%s%s/read_attachments_tail.html", info->display_arg1, NAME);

	transdata = add_data_to_list(transdata, info);
	transdata = add_word_to_list(transdata, "message_number", number);

	filter_print(path, transdata);
	del_list(transdata);

	return;
}

//display tail
void display_read_tail(struct plugin_data *info, int message_number)
{
	char path[400];
	char number[15];
	struct word_list *transdata = NULL;

	snprintf(number, 15, "%d", message_number);
	snprintf(path, 400, "%s%s/read_tail.html", info->display_arg1, NAME);

	transdata = add_data_to_list(transdata, info);
	transdata = add_word_to_list(transdata, "message_number", number);

	filter_print(path, transdata);
	del_list(transdata);

	return;
}
//-- Read saved -------------------------------------

//display everything up to message
void display_saved_read_head(struct plugin_data *info, int message_number)
{
	char path[400];
	char number[15];
	struct word_list *transdata = NULL;

	snprintf(number, 15, "%d", message_number);
	snprintf(path, 400, "%s%s/saved_read_head.html", info->display_arg1, NAME);

	transdata = add_data_to_list(transdata, info);
	transdata = add_word_to_list(transdata, "message_number", number);

	filter_print(path, transdata);
	del_list(transdata);

	return;
}

//display everything after message but before attachments
void display_saved_read_middle(struct plugin_data *info, int message_number)
{
	char path[400];
	char number[15];
	struct word_list *transdata = NULL;

	snprintf(number, 15, "%d", message_number);
	snprintf(path, 400, "%s%s/saved_read_middle.html", info->display_arg1, NAME);

	transdata = add_data_to_list(transdata, info);
	transdata = add_word_to_list(transdata, "message_number", number);

	filter_print(path, transdata);
	del_list(transdata);

	return;
}

void display_saved_read_attachments_head(struct plugin_data *info, int message_number)
{
	char path[400];
	char number[15];
	struct word_list *transdata = NULL;

	snprintf(number, 15, "%d", message_number);
	snprintf(path, 400, "%s%s/saved_read_attachments_head.html", info->display_arg1, NAME);

	transdata = add_data_to_list(transdata, info);
	transdata = add_word_to_list(transdata, "message_number", number);

	filter_print(path, transdata);
	del_list(transdata);

	return;
}

//display attachments
void display_saved_read_attachments(struct plugin_data *info, int message_number,
int attachment_number, char *file_name, char *mode)
{
	printf("%s", "<a href=\"");
	printf("%s", "./attachments/");
	printf("%s", file_name);

	if (strcmp(mode, "mime") == 0)
	{
		printf("%s", "?m_attach_saved=");
	}
	else
	{
		printf("%s", "?u_attach_saved=");
	}
	printf("%d", message_number);
	printf("%s", "x");

	printf("%d", attachment_number);
	printf("%s", "\">");
	printf("%s", file_name);
	printf("%s", "</a><br>");
	return;
}

void display_saved_read_attachments_tail(struct plugin_data *info, int message_number)
{
	char path[400];
	char number[15];
	struct word_list *transdata = NULL;

	snprintf(number, 15, "%d", message_number);
	snprintf(path, 400, "%s%s/saved_read_attachments_tail.html", info->display_arg1, NAME);

	transdata = add_data_to_list(transdata, info);
	transdata = add_word_to_list(transdata, "message_number", number);

	filter_print(path, transdata);
	del_list(transdata);

	return;
}

//display tail
void display_saved_read_tail(struct plugin_data *info, int message_number)
{
	char path[400];
	char number[15];
	struct word_list *transdata = NULL;

	snprintf(number, 15, "%d", message_number);
	snprintf(path, 400, "%s%s/saved_read_tail.html", info->display_arg1, NAME);

	transdata = add_data_to_list(transdata, info);
	transdata = add_word_to_list(transdata, "message_number", number);

	filter_print(path, transdata);
	del_list(transdata);

	return;
}
//-- Read sent -------------------------------------

//display everything up to message
void display_sent_read_head(struct plugin_data *info, int message_number)
{
	char path[400];
	char number[15];
	struct word_list *transdata = NULL;

	snprintf(number, 15, "%d", message_number);
	snprintf(path, 400, "%s%s/sent_read_head.html", info->display_arg1, NAME);

	transdata = add_data_to_list(transdata, info);
	transdata = add_word_to_list(transdata, "message_number", number);

	filter_print(path, transdata);
	del_list(transdata);

	return;
}

//display everything after message but before attachments
void display_sent_read_middle(struct plugin_data *info, int message_number)
{
	char path[400];
	char number[15];
	struct word_list *transdata = NULL;

	snprintf(number, 15, "%d", message_number);
	snprintf(path, 400, "%s%s/sent_read_middle.html", info->display_arg1, NAME);

	transdata = add_data_to_list(transdata, info);
	transdata = add_word_to_list(transdata, "message_number", number);

	filter_print(path, transdata);
	del_list(transdata);

	return;
}

void display_sent_read_attachments_head(struct plugin_data *info, int message_number)
{
	char path[400];
	char number[15];
	struct word_list *transdata = NULL;

	snprintf(number, 15, "%d", message_number);
	snprintf(path, 400, "%s%s/sent_read_attachments_head.html", info->display_arg1, NAME);

	transdata = add_data_to_list(transdata, info);
	transdata = add_word_to_list(transdata, "message_number", number);

	filter_print(path, transdata);
	del_list(transdata);

	return;
}

//display attachments
void display_sent_read_attachments(struct plugin_data *info, int message_number,
int attachment_number, char *file_name, char *mode)
{
	printf("%s", "<a href=\"");
	printf("%s", "./attachments/");
	printf("%s", file_name);

	if (strcmp(mode, "mime") == 0)
	{
		printf("%s", "?m_attach_sent=");
	}
	else
	{
		printf("%s", "?u_attach_sent=");
	}
	printf("%d", message_number);
	printf("%s", "x");

	printf("%d", attachment_number);
	printf("%s", "\">");
	printf("%s", file_name);
	printf("%s", "</a><br>");
	return;
}

void display_sent_read_attachments_tail(struct plugin_data *info, int message_number)
{
	char path[400];
	char number[15];
	struct word_list *transdata = NULL;

	snprintf(number, 15, "%d", message_number);
	snprintf(path, 400, "%s%s/sent_read_attachments_tail.html", info->display_arg1, NAME);

	transdata = add_data_to_list(transdata, info);
	transdata = add_word_to_list(transdata, "message_number", number);

	filter_print(path, transdata);
	del_list(transdata);

	return;
}

//display tail
void display_sent_read_tail(struct plugin_data *info, int message_number)
{
	char path[400];
	char number[15];
	struct word_list *transdata = NULL;

	snprintf(number, 15, "%d", message_number);
	snprintf(path, 400, "%s%s/sent_read_attachments_tail.html", info->display_arg1, NAME);

	transdata = add_data_to_list(transdata, info);
	transdata = add_word_to_list(transdata, "message_number", number);

	filter_print(path, transdata);
	del_list(transdata);

	return;
}

//-- Write -------------------------------------

//write every thing up to message
void display_write_head(struct plugin_data *info, struct wewrite_data *wewriteinfo)
//what ever write gets + hidden tags
{
	char path[400];
	struct word_list *transdata = NULL;

	snprintf(path, 400, "%s%s/write_head.html", info->display_arg1, NAME);

	transdata = add_data_to_list(transdata, info);
	transdata = add_word_to_list(transdata, "write_mode", wewriteinfo->mode);
	transdata = add_word_to_list(transdata, "to", wewriteinfo->to);
	transdata = add_word_to_list(transdata, "cc", wewriteinfo->cc);
	transdata = add_word_to_list(transdata, "subject", wewriteinfo->subject);
	transdata = add_word_to_list(transdata, "hidden", wewriteinfo->hidden);

	filter_print(path, transdata);
	del_list(transdata);

	return;
}


//write every thing after message
void display_write_tail(struct plugin_data *info, struct wewrite_data *wewriteinfo)
{
	char path[400];
	struct word_list *transdata = NULL;

	snprintf(path, 400, "%s%s/write_middle.html", info->display_arg1, NAME);

	transdata = add_data_to_list(transdata, info);
	transdata = add_word_to_list(transdata, "write_mode", wewriteinfo->mode);
	transdata = add_word_to_list(transdata, "to", wewriteinfo->to);
	transdata = add_word_to_list(transdata, "cc", wewriteinfo->cc);
	transdata = add_word_to_list(transdata, "subject", wewriteinfo->subject);
	transdata = add_word_to_list(transdata, "hidden", wewriteinfo->hidden);

	filter_print(path, transdata);
	del_list(transdata);


	if (info->use_attachments)
	{
		transdata = NULL;

		snprintf(path, 400, "%s%s/write_attachments.html", info->display_arg1, NAME);

		transdata = add_data_to_list(transdata, info);
		transdata = add_word_to_list(transdata, "write_mode", wewriteinfo->mode);
		transdata = add_word_to_list(transdata, "to", wewriteinfo->to);
		transdata = add_word_to_list(transdata, "cc", wewriteinfo->cc);
		transdata = add_word_to_list(transdata, "subject", wewriteinfo->subject);
		transdata = add_word_to_list(transdata, "hidden", wewriteinfo->hidden);

		filter_print(path, transdata);
		del_list(transdata);
	}

	transdata = NULL;

	snprintf(path, 400, "%s%s/write_tail.html", info->display_arg1, NAME);

	transdata = add_data_to_list(transdata, info);
	transdata = add_word_to_list(transdata, "write_mode", wewriteinfo->mode);
	transdata = add_word_to_list(transdata, "to", wewriteinfo->to);
	transdata = add_word_to_list(transdata, "cc", wewriteinfo->cc);
	transdata = add_word_to_list(transdata, "subject", wewriteinfo->subject);
	transdata = add_word_to_list(transdata, "hidden", wewriteinfo->hidden);

	filter_print(path, transdata);
	del_list(transdata);

	return;
}

//-- Config ------------------------------------

void display_config(struct plugin_data *info, struct config_data *configinfo)
{
	char path[400];
	struct word_list *transdata = NULL;

	snprintf(path, 400, "%s%s/config_head.html", info->display_arg1, NAME);

	transdata = add_data_to_list(transdata, info);

	filter_print(path, transdata);
	del_list(transdata);



//assuming forwarding is always enabled.
//not the best idea and should be FIXED!!!!!!
	transdata = NULL;

	snprintf(path, 400, "%s%s/config_forward.html", info->display_arg1, NAME);

	transdata = add_data_to_list(transdata, info);
	transdata = add_word_to_list(transdata, "config_forward", configinfo->forwarding_address);
	filter_print(path, transdata);
	del_list(transdata);


//assuming Signature is always enabled.
//not the best idea and should be FIXED!!!!!!!
	transdata = NULL;

	snprintf(path, 400, "%s%s/config_sign.html", info->display_arg1, NAME);

	transdata = add_data_to_list(transdata, info);
	transdata = add_word_to_list(transdata, "config_sign", configinfo->signature);
	filter_print(path, transdata);
	del_list(transdata);


	if (configinfo->show_pop)
	{
		transdata = NULL;

		snprintf(path, 400, "%s%s/config_pop.html", info->display_arg1, NAME);

		transdata = add_data_to_list(transdata, info);
		transdata = add_word_to_list(transdata, "config_pop_user", configinfo->user);
		transdata = add_word_to_list(transdata, "config_pop_pass", configinfo->password);
		transdata = add_word_to_list(transdata, "config_pop_server", configinfo->server);
		filter_print(path, transdata);
		del_list(transdata);

	}

	if (configinfo->show_return)
	{
		transdata = NULL;

		snprintf(path, 400, "%s%s/config_return.html", info->display_arg1, NAME);

		transdata = add_data_to_list(transdata, info);
		transdata = add_word_to_list(transdata, "config_return", configinfo->return_address);
		filter_print(path, transdata);
		del_list(transdata);

	}
	transdata = NULL;

	snprintf(path, 400, "%s%s/config_tail.html", info->display_arg1, NAME);

	transdata = add_data_to_list(transdata, info);
	filter_print(path, transdata);
	del_list(transdata);

	return;
}

//-- About -------------------------------------

void display_about(struct plugin_data *info)
{
	char path[400];
	struct word_list *transdata = NULL;

	snprintf(path, 400, "%s%s/about.html", info->display_arg1, NAME);

	transdata = add_data_to_list(transdata, info);
	filter_print(path, transdata);
	del_list(transdata);

	return;
}

//-- Login -------------------------------------

void display_login(struct plugin_data *info)
{

	char path[400];
	char servername[300];
	char authorinfo[300];

	struct word_list *transdata = NULL;

	snprintf(path, 400, "%s%s/login.html", info->display_arg1, NAME);

	transdata = add_data_to_list(transdata, info);

	if (strncasecmp(info->server_mode, "define", 6)==0) //use defined name
	{
		snprintf(servername, 300, "%s\n", info->server_name);

	}
	else if (strncasecmp(info->server_mode, "httpd", 5)==0)	//use httpds name
	{
		snprintf(servername, 300, "%s\n", info->server_name);
	}
	else	//display no name; sendmail
	{
		snprintf(servername, 300, "");
	}
	transdata = add_word_to_list(transdata, "server_name", servername);


	if (info->use_authorinfo)
	{
		snprintf(authorinfo, 300, "Written by Andrew Shaw\n");
	}
	else
	{
		snprintf(authorinfo, 300, "");
	}
	transdata = add_word_to_list(transdata, "author_info", authorinfo);


	filter_print(path, transdata);
	del_list(transdata);

	return;
}

//-- Error -------------------------------------

void display_error(struct plugin_data *info, char *error_message)
{

	char path[400];
	struct word_list *transdata = NULL;

	snprintf(path, 400, "%s%s/error.html", info->display_arg1, NAME);

	transdata = add_data_to_list(transdata, info);
	transdata = add_word_to_list(transdata, "error_message", error_message);

	filter_print(path, transdata);
	del_list(transdata);

	return;
}

//-- Address Book -----------------------------

void display_address_head(struct plugin_data *info)
{
	char path[400];
	struct word_list *transdata = NULL;

	snprintf(path, 400, "%s%s/address_head.html", info->display_arg1, NAME);

	transdata = add_data_to_list(transdata, info);

	filter_print(path, transdata);
	del_list(transdata);

	return;
}

void display_address_letterbreak(struct plugin_data *info, char letter)
{

	char path[400];
	char tletter[5];
	struct word_list *transdata = NULL;

	snprintf(tletter, 5, "%c", letter);
	snprintf(path, 400, "%s%s/address_letterbreak.html", info->display_arg1, NAME);

	transdata = add_data_to_list(transdata, info);
	transdata = add_word_to_list(transdata, "letter", tletter);

	filter_print(path, transdata);
	del_list(transdata);

	return;
}

void display_single_entre(struct plugin_data *info, struct addr_single_entry *addressdata)
{
	char path[400];

	char number[15];
	struct word_list *transdata = NULL;

	snprintf(number, 15, "%d", addressdata->uid);
	snprintf(path, 400, "%s%s/single_entre.html", info->display_arg1, NAME);

	transdata = add_data_to_list(transdata, info);
	transdata = add_word_to_list(transdata, "address_number", number);
	transdata = add_word_to_list(transdata, "address_name", addressdata->name);
	transdata = add_word_to_list(transdata, "address_email", addressdata->email_address);
	transdata = add_word_to_list(transdata, "address_phone", addressdata->phone);
	transdata = add_word_to_list(transdata, "address_fax", addressdata->fax);
	transdata = add_word_to_list(transdata, "address_line_1", addressdata->address_line_1);
	transdata = add_word_to_list(transdata, "address_line_2", addressdata->address_line_2);
	transdata = add_word_to_list(transdata, "address_notes", addressdata->notes);


	filter_print(path, transdata);
	del_list(transdata);

	return;

}

void display_group_entre(struct plugin_data *info, struct addr_group_entry *groupdata)
{
	char path[400];

	char number[15];
	struct word_list *transdata = NULL;

	snprintf(number, 15, "%d", groupdata->uid);
	snprintf(path, 400, "%s%s/group_entre.html", info->display_arg1, NAME);

	transdata = add_data_to_list(transdata, info);
	transdata = add_word_to_list(transdata, "group_number", number);
	transdata = add_word_to_list(transdata, "group_name", groupdata->name);


	filter_print(path, transdata);
	del_list(transdata);

	return;
}

void display_address_tail(struct plugin_data *info)
{
	char path[400];
	struct word_list *transdata = NULL;

	snprintf(path, 400, "%s%s/address_tail.html", info->display_arg1, NAME);

	transdata = add_data_to_list(transdata, info);
	filter_print(path, transdata);
	del_list(transdata);

	return;
}

void display_address_add_group_head(struct plugin_data *info, struct addr_group_entry *groupdata,
int addedit)
{
	char path[400];

	char number[15];
	char method[40];
	struct word_list *transdata = NULL;

	snprintf(number, 15, "%d", groupdata->uid);
	snprintf(path, 400, "%s%s/address_add_group_head.html", info->display_arg1, NAME);

	transdata = add_data_to_list(transdata, info);
	transdata = add_word_to_list(transdata, "group_number", number);
	transdata = add_word_to_list(transdata, "group_name", groupdata->name);

	if (addedit == 1)
		transdata = add_word_to_list(transdata, "group_addedit", "Edit");
        else
		transdata = add_word_to_list(transdata, "group_addedit", "Add");

	if (addedit == 1)
        {
		snprintf(method, 40, "reprocess_group=%s", number);
        }
        else
        {
		snprintf(method, 40, "process_group");
        }
	transdata = add_word_to_list(transdata, "group_method", method);


	filter_print(path, transdata);
	del_list(transdata);

	return;
}

void display_address_add_group_tail(struct plugin_data *info, struct addr_group_entry *groupdata,
int addedit)
{
	char path[400];

	char number[15];
	struct word_list *transdata = NULL;

	snprintf(number, 15, "%d", groupdata->uid);
	snprintf(path, 400, "%s%s/address_add_group_tail.html", info->display_arg1, NAME);

	transdata = add_data_to_list(transdata, info);
	transdata = add_word_to_list(transdata, "group_number", number);
	transdata = add_word_to_list(transdata, "group_name", groupdata->name);

	filter_print(path, transdata);
	del_list(transdata);

	return;
}


void display_address_add_single(struct plugin_data *info, struct addr_single_entry *addressdata,
int addedit)
{
	char path[400];

	char number[15];
	char method[40];
	struct word_list *transdata = NULL;

	snprintf(number, 15, "%d", addressdata->uid);
	snprintf(path, 400, "%s%s/address_add_single.html", info->display_arg1, NAME);

	transdata = add_data_to_list(transdata, info);
	transdata = add_word_to_list(transdata, "address_number", number);
	transdata = add_word_to_list(transdata, "address_name", addressdata->name);
	transdata = add_word_to_list(transdata, "address_email", addressdata->email_address);
	transdata = add_word_to_list(transdata, "address_phone", addressdata->phone);
	transdata = add_word_to_list(transdata, "address_fax", addressdata->fax);
	transdata = add_word_to_list(transdata, "address_line_1", addressdata->address_line_1);
	transdata = add_word_to_list(transdata, "address_line_2", addressdata->address_line_2);
	transdata = add_word_to_list(transdata, "address_notes", addressdata->notes);

	if (addedit == 1)
		transdata = add_word_to_list(transdata, "address_addedit", "Edit");
        else
		transdata = add_word_to_list(transdata, "address_addedit", "Add");

	if (addedit == 1)
        {
		snprintf(method, 40, "reprocess_single=%s", number);
        }
        else
        {
		snprintf(method, 40, "process_single");
        }
	transdata = add_word_to_list(transdata, "address_method", method);

	filter_print(path, transdata);
	del_list(transdata);

	return;

}

//-- view html --------------------------------------------
void display_view_html_head(struct plugin_data *info, int message_number)
{
	char path[400];
	char number[15];
	struct word_list *transdata = NULL;

	snprintf(number, 15, "%d", message_number);
	snprintf(path, 400, "%s%s/view_html_head.html", info->display_arg1, NAME);

	transdata = add_data_to_list(transdata, info);
	transdata = add_word_to_list(transdata, "message_number", number);

	filter_print(path, transdata);
	del_list(transdata);

	return;
}
void display_saved_view_html_head(struct plugin_data *info, int message_number)
{
	char path[400];
	char number[15];
	struct word_list *transdata = NULL;

	snprintf(number, 15, "%d", message_number);
	snprintf(path, 400, "%s%s/saved_view_html_head.html", info->display_arg1, NAME);

	transdata = add_data_to_list(transdata, info);
	transdata = add_word_to_list(transdata, "message_number", number);

	filter_print(path, transdata);
	del_list(transdata);

	return;
}
void display_sent_view_html_head(struct plugin_data *info, int message_number)
{
	char path[400];
	char number[15];
	struct word_list *transdata = NULL;

	snprintf(number, 15, "%d", message_number);
	snprintf(path, 400, "%s%s/sent_view_html_head.html", info->display_arg1, NAME);

	transdata = add_data_to_list(transdata, info);
	transdata = add_word_to_list(transdata, "message_number", number);

	filter_print(path, transdata);
	del_list(transdata);

	return;
}


void display_view_html_tail(struct plugin_data *info, int message_number)
{
	char path[400];
	char number[15];
	struct word_list *transdata = NULL;

	snprintf(number, 15, "%d", message_number);
	snprintf(path, 400, "%s%s/view_html_tail.html", info->display_arg1, NAME);

	transdata = add_data_to_list(transdata, info);
	transdata = add_word_to_list(transdata, "message_number", number);

	filter_print(path, transdata);
	del_list(transdata);

	return;
}
void display_saved_view_html_tail(struct plugin_data *info, int message_number)
{
	char path[400];
	char number[15];
	struct word_list *transdata = NULL;

	snprintf(number, 15, "%d", message_number);
	snprintf(path, 400, "%s%s/saved_view_html_tail.html", info->display_arg1, NAME);

	transdata = add_data_to_list(transdata, info);
	transdata = add_word_to_list(transdata, "message_number", number);

	filter_print(path, transdata);
	del_list(transdata);

	return;
}
void display_sent_view_html_tail(struct plugin_data *info, int message_number)
{
	char path[400];
	char number[15];
	struct word_list *transdata = NULL;

	snprintf(number, 15, "%d", message_number);
	snprintf(path, 400, "%s%s/sent_view_html_tail.html", info->display_arg1, NAME);

	transdata = add_data_to_list(transdata, info);
	transdata = add_word_to_list(transdata, "message_number", number);

	filter_print(path, transdata);
	del_list(transdata);

	return;
}

void display_java_script_head(struct plugin_data *info)
{
	char path[400];
	struct word_list *transdata = NULL;

	snprintf(path, 400, "%s%s/java_script_head.html", info->display_arg1, NAME);

	transdata = add_data_to_list(transdata, info);
	filter_print(path, transdata);
	del_list(transdata);

	return;
	//Java script will be displayed after this message, do not interfear with it!!!!!!
}
void display_java_script_tail(struct plugin_data *info)
{
	char path[400];
	struct word_list *transdata = NULL;

	snprintf(path, 400, "%s%s/java_script_tail.html", info->display_arg1, NAME);

	transdata = add_data_to_list(transdata, info);
	filter_print(path, transdata);
	del_list(transdata);

	return;
}

