#include "../../plugin.h"
#include "../../displugin.h"
#include "../../addrplugin.h"
#include <stdlib.h>
#include <string.h>

//--internal functions, not part of the interface--------------

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



void display_header(struct plugin_data *info, char *title)
{
	printf("%s", "<table CELLSPACING=0 CELLPADDING=0 COLS=2 WIDTH=\"100%\" NOSAVE>\n");
	printf("%s", "<tr NOSAVE><td NOSAVE>\n");

	printf("%s", "<b><font size=\"+4\">");
	printf("%s", title);
	printf("%s", "</font></b>\n");
	printf("%s", "<font size=-1>&nbsp;&nbsp;&nbsp;(");
	printf("%s", info->user_name);
	printf("%s", ")</font>\n");

	printf("%s", "</td><td NOSAVE>\n");
	printf("%s", "<div align=right>\n");

	printf("%s", "&lt;&nbsp;<a href=\"");
	printf("%s", info->url);
	printf("%s", "\" TARGET=_top>Main</a>&nbsp;-&nbsp;");

	printf("%s", "<a href=\"");
	printf("%s", info->url);
	printf("%s", "?write\" TARGET=_top>Compose</a>&nbsp;-&nbsp;");
	printf("%s", "<a href=\"");
	printf("%s", info->url);
	printf("%s", "?book\" TARGET=_top>Address Book</a>&nbsp;-&nbsp;");
	printf("%s", "<a href=\"");
	printf("%s", info->url);
	printf("%s", "?about\" TARGET=_top>About</a>&nbsp;-&nbsp;");
	printf("%s", "<a href=\"");
	printf("%s", info->url);
	printf("%s", "?reset\" TARGET=_top>Logout</a>&nbsp;>");

	printf("%s", "</div>\n");
	printf("%s", "</td></tr></table><hr WIDTH=\"100%\">\n");

	return;
}

void display_tail(struct plugin_data *info)
{
	printf("%s", "<hr WIDTH=\"100%\">\n");
	printf("%s", "<br>Questions or comments? Refer to ");
	printf("%s", "<a href=\"");
	printf("%s", info->web_email_support);
	printf("%s", "\">");
	printf("%s", info->web_email_support);
	printf("%s", "</a>\n");
	return;
}






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
	strncpy(type, "en", 3);
	return type;  //return english
}
char* get_plugin_name() //report plug in name
{
	char *name;
	name = malloc(20);
	strncpy(name, "Classic (devel)", 19);
	return name;
}

//-- Main -------------------------------------

void display_main_window(struct plugin_data *info, char *toshow) //probably aframe
{
	printf("%s", "<html>\n");
	printf("%s", "<head>\n");
	printf("%s", "<title>Web E-Mail</title>\n");
	printf("%s", "<META HTTP-EQUIV=\"expires\" content=\"0\">\n");
	printf("%s", "</head>\n");
	printf("%s", "<FRAMESET ROWS=\"35,*\" BORDER=\"0\">\n");
	printf("%s", "<NOFRAME>Sorry, your browser does not ");
	printf("%s", "support frames.  The current release of ");
	printf("%s", "Web E-Mail does not have a non frame version.</NOFRAME>");

	printf("%s", "<FRAME SRC=\"");
	printf("%s", info->url);
	printf("%s", "?display_title\" NAME=\"TITLE\"");
	printf("%s", "MARGINHEIGHT=\"0\" MARGINWIDTH=\"0\" SCROLLING=\"NO\">");
	printf("%s", "<FRAMESET COLS=\"170,*\">");
	printf("%s", "<FRAME SRC=\"");
	printf("%s", info->url);
	printf("%s", "?display_menu\" NAME=\"MENU\" ");
	printf("%s", "MARGINHEIGHT=\"0\" MARGINWIDTH=\"0\" SCROLLING=\"NO\">");

	printf("%s", "<FRAME SRC=\"");
	printf("%s", info->url);
	printf("%s", toshow);
	printf("%s", "\" NAME=\"MAIL\" ");
	printf("%s", "MARGINHEIGHT=\"0\" MARGINWIDTH=\"0\" SCROLLING=\"AUTO\">");

	printf("%s", "</FRAMESET>\n");
	printf("%s", "</FRAMESET>\n");
	printf("%s", "</html>\n");

	return;
}
void display_main_title(struct plugin_data *info)
{
	printf("%s", "<html>\n");
	printf("%s", "<head>\n");
	printf("%s", "<title>Web E-Mail</title>\n");
	printf("%s", "</head>\n");
	printf("%s", "<body text=\"#000000\" bgcolor=\"#FFFFFF\" link=\"#0000FF\" ");
	printf("%s", "vlink=\"#0000FF\" alink=\"#FF0000\" background=/email/bg1.gif>");

	printf("%s", "<table CELLSPACING=0 CELLPADDING=0 COLS=2 WIDTH=\"100%\" NOSAVE >");
	printf("%s", "<tr NOSAVE>");
	printf("%s", "<td ALIGN=LEFT VALIGN=TOP NOSAVE>");
	printf("%s", "<img SRC=\"/email/title.gif\" ALT=\"");
	printf("%s", "Web E-Mail");
	printf("%s", "\" height=23 width=147 align=CENTER>\n");
	printf("%s", "<font size=-1>&nbsp;&nbsp;&nbsp;(");
	printf("%s", info->user_name);
	printf("%s", ")</font>\n");
	printf("%s", "</td>\n");

	printf("%s", "<td ALIGN=RIGHT VALIGN=TOP NOSAVE>");
	printf("%s", "&lt;Folders: ");
	printf("%s", "<a href=\"");
	printf("%s", info->url);
	printf("%s", "?index_inbox\" TARGET=MAIL>");
	printf("%s", "InBox</a>");
	printf("%s", "&nbsp;-&nbsp;");
	printf("%s", "<a href=\"");
	printf("%s", info->url);
	printf("%s", "?index_saved\" TARGET=MAIL>Saved</a>");
	printf("%s", " - ");
	printf("%s", "<a href=\"");
	printf("%s", info->url);
	printf("%s", "?index_sent\" TARGET=MAIL>Sent</a>");
	printf("%s", " ></td>");
	printf("%s", "</tr></table>");

	printf("%s", "</body>\n");
	printf("%s", "</html>\n");
	return;
}
void display_main_menu(struct plugin_data *info)
{

	printf("%s", "<html>\n");
	printf("%s", "<head>\n");
	printf("%s", "<title>Web E-Mail</title>\n");
	printf("%s", "</head>\n");

	printf("%s", "<body text=\"#000000\" bgcolor=\"#FFFFFF\" ");
	printf("%s", "link=\"#FFFFFF\" vlink=\"#FFFFFF\" alink=\"#FF0000\" ");
	printf("%s", "background=/email/bg1.gif>\n");

	printf("%s", "<table CELLSPACING=0 CELLPADDING=0 COLS=1 WIDTH=\"150\" ");
	printf("%s", "HEIGHT=\"300\" BACKGROUND=\"/email/bk.gif\" NOSAVE >\n");
	printf("%s", "<tr ALIGN=LEFT VALIGN=TOP NOSAVE >\n");
	printf("%s", "<td NOSAVE><br>\n");
	printf("%s", "<table CELLSPACING=0 CELLPADDING=0 BACKGROUND=\"/email/none.gif\" NOSAVE >\n");

	printf("%s", "<tr NOSAVE>\n");
	printf("%s", "<td ALIGN=CENTER VALIGN=CENTER NOSAVE>\n");
	printf("%s", "<a href=\"");
	printf("%s", info->url);
	printf("%s", "?write\" TARGET=_top>\n");
	printf("%s", "<img SRC=\"/email/compose.gif\" ALT=\"\" BORDER=0 height=17 ");
	printf("%s", "width=11 align=ABSCENTER></a></td>\n");
	printf("%s", "<td NOSAVE><b><font color=\"#FFFFFF\">\n");
	printf("%s", "<a href=\"");
	printf("%s", info->url);
	printf("%s", "?write\" TARGET=_top>"); 
	printf("%s", "Compose Mail");
	printf("%s", "</a></font></b></td>");
	printf("%s", "</tr>\n");

	printf("%s", "<tr NOSAVE>\n");
	printf("%s", "<td ALIGN=CENTER VALIGN=CENTER NOSAVE>\n");
	printf("%s", "<a href=\"");
	printf("%s", info->url);
	printf("%s", "?index_inbox\" TARGET=MAIL>");
	printf("%s", "<img SRC=\"/email/reload.gif\" ALT=\"\"\n");
	printf("%s", "BORDER=0 height=21 width=28 align=CENTER></a></td>\n");
	printf("%s", "<td><b><font color=\"#FFFFFF\">\n");
	printf("%s", "<a href=\"");
	printf("%s", info->url);
	printf("%s", "?index_inbox\" TARGET=MAIL>");
	printf("%s", "Rescan Mail Box</a></font></b></td>\n");
	printf("%s", "</tr>\n");

	printf("%s", "<tr NOSAVE>\n");
	printf("%s", "<td ALIGN=CENTER VALIGN=CENTER NOSAVE>\n");
	printf("%s", "<a href=\"");
	printf("%s", info->url);
	printf("%s", "?book\" TARGET=_top>\n");
	printf("%s", "<img SRC=\"/email/address.gif\" ALT=\"\"\n");
	printf("%s", "BORDER=0 height=16 width=22 align=CENTER></a></td>\n");
	printf("%s", "<td><b><font color=\"#FFFFFF\">");
	printf("%s", "<a href=\"");
	printf("%s", info->url);
	printf("%s", "?book\" TARGET=_top>");
	printf("%s", "Address Book</a>\n");
	printf("%s", "</font></b></td></tr>\n");

	printf("%s", "<tr NOSAVE>\n");
	printf("%s", "<td ALIGN=CENTER VALIGN=CENTER NOSAVE>\n");
	printf("%s", "<a href=\"");
	printf("%s", info->url);
	printf("%s", "?conf\" TARGET=_top>\n");
	printf("%s", "<img SRC=\"/email/conf.gif\" ALT=\"\"\n");
	printf("%s", "BORDER=0 height=16 width=22 align=CENTER></a></td>\n");
	printf("%s", "<td><b><font color=\"#FFFFFF\">");
	printf("%s", "<a href=\"");
	printf("%s", info->url);
	printf("%s", "?conf\" TARGET=_top>");
	printf("%s", "Configure</a>\n");
	printf("%s", "</font></b></td></tr>\n");

	printf("%s", "<tr NOSAVE>\n");
	printf("%s", "<td ALIGN=CENTER VALIGN=CENTER NOSAVE>\n");
	printf("%s", "<a href=\"");
	printf("%s", info->url);
	printf("%s", "?about\" TARGET=_top>\n");
	printf("%s", "<img SRC=\"/email/about.gif\" ALT=\"\"\n");
	printf("%s", "BORDER=0 height=16 width=22 align=CENTER></a></td>\n");
	printf("%s", "<td><b><font color=\"#FFFFFF\">\n");
	printf("%s", "<a href=\"");
	printf("%s", info->url);
	printf("%s", "?about\" TARGET=_top>");
	printf("%s", "About</a>");
	printf("%s", "</font></b></td></tr>\n");
	printf("%s", "<tr NOSAVE>\n");

	printf("%s", "<td ALIGN=CENTER VALIGN=CENTER NOSAVE>\n");
	printf("%s", "<a href=\"");
	printf("%s", info->url);
	printf("%s", "?reset\" TARGET=_top>\n");
	printf("%s", "<img SRC=\"/email/logout.gif\" ALT=\"\" ");
	printf("%s", "BORDER=0 height=22 width=15 align=CENTER>");
	printf("%s", "</a></td>\n");
	printf("%s", "<td><b><font color=\"#FFFFFF\">");
	printf("%s", "<a href=\"");
	printf("%s", info->url);
	printf("%s", "?reset\" TARGET=_top>\n");
	printf("%s", "Logout</a></font></b></td>\n");
	printf("%s", "</tr>\n");

	printf("%s", "</table></td></tr></table>\n");
	printf("%s", "</body></html>\n");
	return;
}

//-- Inbox ------------------------------------

void display_menu_head(struct plugin_data *info)
{
        printf("%s", "<html>\n");
        printf("%s", "<head>\n");
        printf("%s", "<title>Web E-Mail</title>\n");
        printf("%s", "<META HTTP-EQUIV=\"expires\" content=\"0\">\n");
        printf("%s", "</head>\n");
        printf("%s", "<body text=\"#000000\" bgcolor=\"#FFFFFF\" link=\"#0000FF\" ");
	printf("%s", "vlink=\"#0000FF\" alink=\"#FF0000\" background=/email/bg1.gif>\n");

	printf("%s", "<font size=\"+2\">InBox</font><hr>");

	printf("%s", "<form action=\"");
	printf("%s", info->url);

	printf("%s", "?multi_del_inbox\" method=\"post\">");
	return;

}
void display_menu_entree(struct plugin_data *info,
char *from, char *subject, char *status, int message_number)
//status, new, old, replied
{
	if (strcmp(status, "replied") == 0)
	{//replied need if
		printf("%s", "<img SRC=\"/email/replied.jpeg\" ALT=\"(Replied)\" ");
		printf("%s", "BORDER=0 height=14 width=20>\n");
	}
	else if (strcmp(status, "old") == 0)
	{//old need if
		printf("%s", "<img SRC=\"/email/old.jpeg\" ALT=\"(Old)\" ");
		printf("%s", "BORDER=0 height=14 width=20>\n");
	}
	else
	{//new
		printf("%s", "<img SRC=\"/email/new.jpeg\" ALT=\"(New)\" ");
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

	char *index = NULL;
	char temp[400];
	int divs;
	int extra;
	int lower;
	int upper;
	int i;


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


	printf("%s", "<Input TYPE=submit Value=\"");
        printf("%s", "Delete checked mail.\"></form>\n");

	if (info->use_externmail == 1)
        {
                //snprintf(poppath, 400, "%s/.fetchmailrc",info->pw_dir);
                //popfile = fopen(poppath, "r");
                //if (popfile != NULL)
                //{
                        printf("%s", "<a href=\"");
			printf("%s", info->url);
			printf("%s", "?popcheck\">");
                        printf("%s", "Check external mailbox");
                        printf("%s", "</a><br>");

                //       fclose(popfile);
                //}
        }

	printf("Showing message: %s<br>", index);
	printf("<a href=\"%s?index_inbox=1-%d\">", info->url, total_messages);
	printf("%s", "Show all mail</a>");

	printf("%s", "<br><br>");


        printf("%s", "E-Mail count: ");
	printf("%d", total_messages);
        printf("%s", "<br>");

	if (quota_on == 1)
	{
		printf("%s", "<hr width=\"100%\">");
		printf("%s", "<font color=red size=8>Alert:</font><font ");
       		printf("%s", "color=blue>Your Mailbox Has reached it's Maximum Size.<br>");
        	printf("%s", "&nbsp;&nbsp;While your Mailbox will continue to function, <br>");
        	printf("%s", "it is highly Suggested that you delete any old <br>");
        	printf("%s", "messages to prevent an administrator from removing <br>");
        	printf("%s", "messages that you may need.  Thanks for your Help.<br>");
        	printf("%s", "</font><p><br>");
	}

        printf("%s", "</body></html>\n");
	return;
}

//-- Inbox saved------------------------------------

void display_saved_menu_head(struct plugin_data *info)
{
        printf("%s", "<html>\n");
        printf("%s", "<head>\n");
        printf("%s", "<title>Web E-Mail</title>\n");
        printf("%s", "<META HTTP-EQUIV=\"expires\" content=\"0\">\n");
        printf("%s", "</head>\n");
        printf("%s", "<body text=\"#000000\" bgcolor=\"#FFFFFF\" link=\"#0000FF\" ");
	printf("%s", "vlink=\"#0000FF\" alink=\"#FF0000\" background=/email/bg1saved.gif>\n");

	printf("%s", "<font size=\"+2\">Saved Mail Box</font><hr>");

	printf("%s", "<form action=\"");
	printf("%s", info->url);

	printf("%s", "?multi_del_saved\" method=\"post\">");
	return;

}
void display_saved_menu_entree(struct plugin_data *info,
char *from, char *subject, char *status, int message_number)
//status, new, old, replied
{
	if (strcmp(status, "replied") == 0)
	{//replied need if
		printf("%s", "<img SRC=\"/email/replied.jpeg\" ALT=\"(Replied)\" ");
		printf("%s", "BORDER=0 height=14 width=20>\n");
	}
	else if (strcmp(status, "old") == 0)
	{//old need if
		printf("%s", "<img SRC=\"/email/old.jpeg\" ALT=\"(Old)\" ");
		printf("%s", "BORDER=0 height=14 width=20>\n");
	}
	else
	{//new
		printf("%s", "<img SRC=\"/email/new.jpeg\" ALT=\"(New)\" ");
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

	char *index = NULL;
	char temp[400];
	int divs;
	int extra;
	int lower;
	int upper;
	int i;


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



	printf("%s", "<Input TYPE=submit Value=\"");
        printf("%s", "Delete checked mail.\"></form>\n");


	printf("Showing message: %s<br>", index);
	printf("<a href=\"%s?index_saved=1-%d\">", info->url, total_messages);
	printf("%s", "Show all mail</a>");

	printf("%s", "<br><br>");


        printf("%s", "E-Mail count: ");
	printf("%d", total_messages);
        printf("%s", "<br>");

        printf("%s", "</body></html>\n");
	return;

}

//-- Inbox sent ------------------------------------

void display_sent_menu_head(struct plugin_data *info)
{
        printf("%s", "<html>\n");
        printf("%s", "<head>\n");
        printf("%s", "<title>Web E-Mail</title>\n");
        printf("%s", "<META HTTP-EQUIV=\"expires\" content=\"0\">\n");
        printf("%s", "</head>\n");
        printf("%s", "<body text=\"#000000\" bgcolor=\"#FFFFFF\" link=\"#0000FF\" ");
	printf("%s", "vlink=\"#0000FF\" alink=\"#FF0000\" background=/email/bg1sent.gif>\n");

	printf("%s", "<font size=\"+2\">Sent Mail Box</font><hr>");

	printf("%s", "<form action=\"");
	printf("%s", info->url);

	printf("%s", "?multi_del_sent\" method=\"post\">");
	return;
}

void display_sent_menu_entree(struct plugin_data *info,
char *from, char *subject, char *status, int message_number)
//status, new, old, replied
{

	if (strcmp(status, "replied") == 0)
	{//replied need if
		printf("%s", "<img SRC=\"/email/replied.jpeg\" ALT=\"(Replied)\" ");
		printf("%s", "BORDER=0 height=14 width=20>\n");
	}
	else if (strcmp(status, "old") == 0)
	{//old need if
		printf("%s", "<img SRC=\"/email/old.jpeg\" ALT=\"(Old)\" ");
		printf("%s", "BORDER=0 height=14 width=20>\n");
	}
	else
	{//new
		printf("%s", "<img SRC=\"/email/new.jpeg\" ALT=\"(New)\" ");
		printf("%s", "BORDER=0 height=14 width=20>\n");
	}

	printf("%s", "&nbsp;<input TYPE=\"checkbox\" NAME=\"m");
	printf("%d", message_number);
	printf("%s", "\">&nbsp;\n");
	printf("%s", "<a href=\"");
	printf("%s", info->url);
	printf("?read_sent=%d", message_number);
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
	printf("?delete_sent=%d", message_number);
	printf("%s", "\" TARGET=_top>");

	printf("%s", "Delete</a><br><br>\n");

	return;
}
void display_sent_menu_tail(struct plugin_data *info, int quota_on, int total_messages,
int current_start_mes, int current_block_size)
{

	char *index = NULL;
	char temp[400];
	int divs;
	int extra;
	int lower;
	int upper;
	int i;


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

	printf("%s", "<Input TYPE=submit Value=\"");
        printf("%s", "Delete checked mail.\"></form>\n");

	printf("Showing message: %s<br>", index);
	printf("<a href=\"%s?index_sent=1-%d\">", info->url, total_messages);
	printf("%s", "Show all mail</a>");

	printf("%s", "<br><br>");


        printf("%s", "E-Mail count: ");
	printf("%d", total_messages);
        printf("%s", "<br>");

        printf("%s", "</body></html>\n");
	return;
}


//-- Read -------------------------------------

//display everything up to message
void display_read_head(struct plugin_data *info, int message_number)
{
	printf("%s", "<html><head>\n");
	printf("%s", "<title>Web E-Mail</title>\n");
	printf("%s", "<META HTTP-EQUIV=\"expires\" content=\"0\">\n");
	printf("%s", "</head>\n");
	printf("%s", "<body text=\"#000000\" bgcolor=\"#FFFFFF\" ");
	printf("%s", "link=\"#0000EF\" vlink=\"#0000EF\" alink=\"#FF0000\" background=/email/bg1.gif>\n");

	display_header(info, "Read");

	printf("%s", "<table CELLSPACING=0 CELLPADDING=0 NOSAVE >\n");
	printf("%s", "<tr NOSAVE><td NOSAVE>\n");

	printf("%s", "<table CELLSPACING=0 CELLPADDING=0 WIDTH=\"100%\" NOSAVE >\n");
	printf("%s", "<tr NOSAVE><td ALIGN=LEFT VALIGN=BOTTOM NOSAVE>\n");
	printf("%s", "<a href=\"");
	printf("%s", info->url);
	printf("%s", "?add_name_inbox=");
	printf("%d", message_number);
	printf("%s", "\">Add address</a>&nbsp;&nbsp;"); 
	printf("%s", "<a href=\"");
	printf("%s", info->url);
	printf("%s", "?reply_inbox=");
	printf("%d", message_number);
	printf("%s", "\">Reply</a>&nbsp;&nbsp;");
	printf("%s", "<a href=\"");
	printf("%s", info->url);
	printf("%s", "?forward_inbox=");
	printf("%d", message_number);
	printf("%s", "\">Forward</a><br>");
	printf("%s", "<a href=\"");
	printf("%s", info->url);
	printf("%s", "?viewhtml_inbox=");
	printf("%d", message_number);
	printf("%s", "\">HTML View</a>&nbsp;&nbsp;");
	printf("%s", "<a href=\"");
	printf("%s", info->url);
	printf("%s", "?viewprint_inbox=");
	printf("%d",  message_number);
	printf("%s", "\">Printable view</a>&nbsp;&nbsp;");
	printf("%s", "</td>\n");
	printf("%s", "<td ALIGN=RIGHT VALIGN=BOTTOM NOSAVE>\n");
	printf("%s", "<a href=\"");
	printf("%s", info->url);
	printf("%s", "?save_inbox=");
	printf("%d", message_number);
	printf("%s", "\">Save</a>&nbsp;&nbsp;");
	printf("%s", "<a href=\"");
	printf("%s", info->url);
	printf("%s", "?delete_inbox=");
	printf("%d", message_number);
	printf("%s", "\">Delete</a>");
	printf("%s", "</td></tr></table></td></tr>\n");
	printf("%s", "<tr NOSAVE><td NOSAVE>\n");

	printf("%s", "<form><textarea wrap=virtual cols=80 rows=20>\n");
	return;
}

//display everything after message but before attachments
void display_read_middle(struct plugin_data *info, int message_number)
{

	printf("%s", "</textarea></form>\n");
	printf("%s", "</td></tr></table>\n");
	return;
}

void display_read_attachments_head(struct plugin_data *info, int message_number)
{
	printf("%s", "<font size=\"+2\"><u>");
 	printf("%s", "Attachments</u></font><br>");
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
	printf("%s", "<br>");
	return;
}

//display tail
void display_read_tail(struct plugin_data *info, int message_number)
{
	display_tail(info);
	printf("%s", "</body></html>");
	return;
}
//-- Read saved -------------------------------------

//display everything up to message
void display_saved_read_head(struct plugin_data *info, int message_number)
{
	printf("%s", "<html><head>\n");
	printf("%s", "<title>Web E-Mail</title>\n");
	printf("%s", "<META HTTP-EQUIV=\"expires\" content=\"0\">\n");
	printf("%s", "</head>\n");
	printf("%s", "<body text=\"#000000\" bgcolor=\"#FFFFFF\" ");
	printf("%s", "link=\"#0000EF\" vlink=\"#0000EF\" alink=\"#FF0000\" background=/email/bg1saved.gif>\n");

	display_header(info, "Saved Mail");

	printf("%s", "<table CELLSPACING=0 CELLPADDING=0 NOSAVE >\n");
	printf("%s", "<tr NOSAVE><td NOSAVE>\n");

	printf("%s", "<table CELLSPACING=0 CELLPADDING=0 WIDTH=\"100%\" NOSAVE >\n");
	printf("%s", "<tr NOSAVE><td ALIGN=LEFT VALIGN=BOTTOM NOSAVE>\n");

	printf("%s", "</td>\n");
        printf("%s", "<td ALIGN=RIGHT VALIGN=BOTTOM NOSAVE>\n");
        printf("%s", "<a href=\"");
	printf("%s", info->url);
	printf("?delete_saved=%d", message_number);
	printf("%s", "\">Delete</a>");
        printf("%s", "</td></tr></table></td></tr>\n");
        printf("%s", "<tr NOSAVE><td NOSAVE>\n");

	printf("%s", "<form><textarea wrap=virtual cols=80 rows=20>\n");
	return;
}

//display everything after message but before attachments
void display_saved_read_middle(struct plugin_data *info, int message_number)
{

	printf("%s", "</textarea></form>\n");
	printf("%s", "</td></tr></table>\n");
	return;
}

void display_saved_read_attachments_head(struct plugin_data *info, int message_number)
{
	printf("%s", "<font size=\"+2\"><u>");
 	printf("%s", "Attachments</u></font><br>");
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
	printf("%s", "<br>");
	return;
}

//display tail
void display_saved_read_tail(struct plugin_data *info, int message_number)
{
	display_tail(info);
	return;
}
//-- Read sent -------------------------------------

//display everything up to message
void display_sent_read_head(struct plugin_data *info, int message_number)
{
	printf("%s", "<html><head>\n");
	printf("%s", "<title>Web E-Mail</title>\n");
	printf("%s", "<META HTTP-EQUIV=\"expires\" content=\"0\">\n");
	printf("%s", "</head>\n");
	printf("%s", "<body text=\"#000000\" bgcolor=\"#FFFFFF\" ");
	printf("%s", "link=\"#0000EF\" vlink=\"#0000EF\" alink=\"#FF0000\" background=/email/bg1sent.gif>\n");

	display_header(info, "Sent Mail");

	printf("%s", "<table CELLSPACING=0 CELLPADDING=0 NOSAVE >\n");
	printf("%s", "<tr NOSAVE><td NOSAVE>\n");

	printf("%s", "<table CELLSPACING=0 CELLPADDING=0 WIDTH=\"100%\" NOSAVE >\n");
	printf("%s", "<tr NOSAVE><td ALIGN=LEFT VALIGN=BOTTOM NOSAVE>\n");
	printf("%s", "</td>\n");

	printf("%s", "<td ALIGN=RIGHT VALIGN=BOTTOM NOSAVE>\n");
        printf("%s", "<a href=\""); 
	printf("%s", info->url);
	printf("?delete_sent=%d", message_number);
	printf("%s", "\">Delete</a>");
        printf("%s", "</td></tr></table></td></tr>\n");
        printf("%s", "<tr NOSAVE><td NOSAVE>\n");

	printf("%s", "<form><textarea wrap=virtual cols=80 rows=20>\n");

	return;
}

//display everything after message but before attachments
void display_sent_read_middle(struct plugin_data *info, int message_number)
{

	printf("%s", "</textarea></form>\n");
	printf("%s", "</td></tr></table>\n");
	return;
}

void display_sent_read_attachments_head(struct plugin_data *info, int message_number)
{
	printf("%s", "<font size=\"+2\"><u>");
 	printf("%s", "Attachments</u></font><br>");
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
	printf("%s", "<br>");
	return;
}

//display tail
void display_sent_read_tail(struct plugin_data *info, int message_number)
{
	display_tail(info);
	return;
}

//-- Write -------------------------------------

//write every thing up to message
void display_write_head(struct plugin_data *info, struct wewrite_data *wewriteinfo) 
//what ever write gets + hidden tags
{

	printf("%s", "<html>\n");
	printf("%s", "<head>\n");
	printf("%s", "<title>Web E-Mail</title>\n");
	printf("%s", "<META HTTP-EQUIV=\"expires\" content=\"0\">\n");
	printf("%s", "</head>\n");
	printf("%s", "<body text=\"#000000\" bgcolor=\"#FFFFFF\" link=\"#0000EF\" ");
	printf("%s", "vlink=\"#0000EF\" alink=\"#FF0000\" background=/email/bg1.gif>\n");
	
	display_header(info, wewriteinfo->mode);

	printf("%s", "<form EncType=\"Multipart/form-data\""); 
	printf("%s", " Method=\"post\" action=\"");
	printf("%s", info->url); 
	printf("%s", "?send\">\n");

	printf("%s", "To:&nbsp;<input type=text name=to size=25 value=\"");
	printf("%s", wewriteinfo->to);
	printf("%s",  "\">");

	printf("%s", "&nbsp;Cc:&nbsp;<input type=text name=cc size=25>");
	//second set of send buttons for convinence
	printf("%s", "&nbsp;&nbsp;&nbsp;");
	printf("%s", "<input type=Submit value=\"Send\">");
	printf("%s", "&nbsp;<input type=reset value=\"Reset\">\n");

	printf("%s", "<br>Subject:&nbsp;<input type=text name=subject ");
	printf("%s", "size=50 value=\"");
	printf("%s", wewriteinfo->subject);
	printf("%s", "\">");

	printf("%s", "<br>Message:");
	printf("%s", "<br><textarea wrap=hard cols=80 rows=20 name=message>");

	return;
}


//write every thing after message 
void display_write_tail(struct plugin_data *info, struct wewrite_data *wewriteinfo)
{
	printf("%s", "</textarea>\n");

	if (info->use_attachments)
	{
		printf("%s", "<br>Attachment<br>");
		printf("%s", "<input type=\"File\" name=\"fileupload\" size=\"50\">");

		printf("%s", "<br>Attach Method:&nbsp;");
		printf("%s", "<input type=\"Radio\" name=\"attachtype\" value=\"Mime\" Checked>");
		printf("%s", "Mime&nbsp;");
		printf("%s", "<input type=\"Radio\" name=\"attachtype\" value=\"Uuencode\" >");
		printf("%s", "Uuencode");

		printf("%s", "<br>Uploading a file to the server may take several minutes.");
		printf("%s", "<br>");
	}

	printf("%s", "<br><input type=Submit value=\"Send\">");
	printf("%s", "&nbsp;<input type=reset value=\"Reset\">\n");
	printf("%s", "&nbsp;&nbsp;&nbsp;"); 
	printf("%s", "<input type=Checkbox name=receipt>&nbsp;Request Receipt\n");
	printf("%s", wewriteinfo->hidden);

	printf("%s", "<br></form>\n");
        display_tail(info);
        printf("%s", "</body>\n");
        printf("%s", "</html>\n");

	return;
}

//-- Config ------------------------------------

void display_config(struct plugin_data *info, struct config_data *configinfo)
{
	printf("%s", "<html><head>\n");
	printf("%s", "<title>Web E-Mail</title>\n");
	printf("%s", "<META HTTP-EQUIV=\"expires\" content=\"0\">\n");
	printf("%s", "</head>\n");
	printf("%s", "<body text=\"#000000\" bgcolor=\"#FFFFFF\"");
	printf("%s", " link=\"#0000EF\" vlink=\"#0000EF\" alink=\"#FF0000\"");
	printf("%s", " background=/email/bg1.gif>\n");

 	display_header(info, "Configure");

	printf("%s", "<form Method=\"post\"");
	printf("%s", " action=\"");
	printf("%s", info->url);
	printf("%s", "?process_conf\">\n");
  	printf("%s", "<table WIDTH=\"100%\" NOSAVE >\n");
 	printf("%s", "<tr NOSAVE>\n");
 
//assuming forwarding is always enabled.
//not the best idea and should be FIXED!!!!!!
	printf("%s", "<td ALIGN=LEFT VALIGN=TOP NOSAVE><b>Forwarding:</b>\n");
 	printf("%s", "<br><input type=text name=forward size=30 ");
 	printf("%s", "value=\"");
	printf("%s", configinfo->forwarding_address);
	printf("%s", "\"></td>\n");

	printf("%s", "<td ALIGN=LEFT VALIGN=TOP NOSAVE>\n");
	printf("%s", "<br>Enter an address, if you want your e-mail forwarded to another e-mail\n");
 	printf("%s", "box.&nbsp; If you do not want your mail forwarded leave the box blank.&nbsp;\n");
 	printf("%s", "(Note, forwarding must be enabled/supported by the server.&nbsp; Under\n");
	printf("%s", "linux this means procmail must be installed.)</td>\n");
	printf("%s", "</tr>\n");


//assuming Signature is always enabled.
//not the best idea and should be FIXED!!!!!!!
	printf("%s", "<tr NOSAVE>\n");
	printf("%s", "<td><b>Signature:</b>\n");
	printf("%s", "<br><textarea wrap=virtual cols=30 rows=5 name=sign>");
	printf("%s", configinfo->signature);
	printf("%s", "</textarea></td>\n");

	printf("%s", "<td ALIGN=LEFT VALIGN=TOP NOSAVE>\n");
	printf("%s", "<br>The signature gets attached to the botton of every out going message.&nbsp;\n");
	printf("%s", "If you would like to not have a signature, leave the box blank.</td>\n");
	printf("%s", "</tr>");

	if (configinfo->show_pop)
	{
		printf("%s", "<tr NOSAVE>\n");
		printf("%s", "<td ALIGN=LEFT VALIGN=TOP NOSAVE><b>Pop3 setup:</b>\n");

		printf("%s", "<br>User:&nbsp;<input type=text name=popuser size=25 value=\"");
		if (configinfo->nopop == 0)
			printf("%s", configinfo->user);
		printf("%s", "\">\n");

		printf("%s", "<br>Password:&nbsp;<input type=password name=poppass size=22 value=\"");
		if (configinfo->nopop == 0)
			printf("%s", configinfo->password);
		printf("%s", "\">\n");

		printf("%s", "<br>Popserver:&nbsp;<input type=text name=popserver size=22 value=\"");
		if (configinfo->nopop == 0)
			printf("%s", configinfo->server);
		printf("%s", "\"></td>\n");

		printf("%s", "<td ALIGN=LEFT VALIGN=TOP NOSAVE>\n");
		printf("%s", "<br>Fill in the boxes to the left if you wish to have web e-mail check\n");
		printf("%s", "another pop3 mail box.&nbsp; If you don't want webmail to get your pop\n");
		printf("%s", "mail, leave the first (user) box blank.&nbsp; (Note, Fetchmail must be\n");
		printf("%s", "installed on the server for this to work.)</td>\n");
		printf("%s", "</tr>\n");
	}

	if (configinfo->show_return)
	{
		printf("%s", "<tr NOSAVE>\n");
		printf("%s", "<td ALIGN=LEFT VALIGN=TOP NOSAVE><b>Return address:</b>\n");

		printf("%s", "<br><input type=text name=userreturn size=25 value=\"");
		printf("%s", configinfo->return_address);
		printf("%s", "\">\n");
		printf("%s", "</td>\n");

		printf("%s", "<td ALIGN=LEFT VALIGN=TOP NOSAVE>\n");
		printf("%s", "<br>Fill in the boxes to the left if you wish to use an e-mail address\n");
		printf("%s", "other then the default.\n");
		printf("%s", "</td></tr>\n");

	}

	printf("%s", "</table>\n");
	printf("%s", "<input type=Submit value=\"Save\">");
	printf("%s", "<input type=reset value=\"Reset\"></form>\n");

	display_tail(info);
	printf("%s", "</body>\n");
	printf("%s", "</html>\n");
	return;
}

//-- About -------------------------------------

void display_about(struct plugin_data *info)
{
	printf("%s", "<html>\n");
	printf("%s", "<head>\n");
	printf("%s", "<title>Web E-Mail</title>\n");
	printf("%s", "<meta http-equiv=\"expires\" content=\"0\">\n");
	printf("%s", "</head>\n");
	printf("%s", "<body text=\"#000000\" bgcolor=\"#FFFFFF\" ");
	printf("%s", "link=\"#0000EF\" vlink=\"#0000EF\" alink=\"#FF0000\" ");
	printf("%s", "background=/email/bg1.gif>");

	display_header(info, "About");

	printf("%s", "<center><font size=+2>Web E-Mail ");
	printf("%s", info->web_email_ver);
	printf("%s", "</font>\n");
	printf("%s", "<p>Written by Andrew Shaw.&nbsp; See ");
	printf("%s", "README&nbsp;for license information.");
	printf("%s", "<p>Copyright &copy; 1999 Andrew Shaw, ");
	printf("%s", "All rights reserved.</center>");

	display_tail(info);

	printf("%s", "</body>\n");
	printf("%s", "</html>\n");
	return;
}

//-- Login -------------------------------------

void display_login(struct plugin_data *info)
{
	printf("%s", "<html>\n");
	printf("%s", "<head>\n");
	printf("%s", "<title>Web E-Mail</title>\n");
	printf("%s", "<META HTTP-EQUIV=\"expires\" content=\"0\">\n");
	printf("%s", "</head>\n");
	printf("%s", "<body text=\"#000000\" bgcolor=\"#FFFFFF\" ");
	printf("%s", "link=\"#0000EF\" vlink=\"#0000EF\" alink=\"#FF0000\"");
 	printf("%s", " background=/email/bg1.gif>\n");

	printf("%s", "<table COLS=1 WIDTH=\"100%\" HEIGHT=\"100%\" NOSAVE>\n");
  	printf("%s", "<tr NOSAVE>\n");
	printf("%s", "<td ALIGN=CENTER VALIGN=CENTER NOSAVE>\n");

	printf("%s", "<center>\n");

	printf("%s", "<p><b><font size=\"+4\">Web E-Mail</font></b>\n");

	if (strncasecmp(info->server_mode, "define", 6)==0) //use defined name
	{
		printf("%s", "<font size=-1><br>(");
		printf("%s", info->server_name);
		printf("%s", ")<br></font>\n");
	}
	else if (strncasecmp(info->server_mode, "httpd", 5)==0)	//use httpds name
	{
 		printf("%s", "<font size=-1><br>(");
		printf("%s", info->server_name);
		printf("%s", ")<br></font>\n");
	}
	else	//display no name; sendmail
	{
	}

	printf("%s", "<form method=post action=\"");
	printf("%s", info->url);
	printf("%s", "\">User ID:<input type=text name=user size=15>&nbsp;\n");


	printf("%s", "Password:<input type=password name=pass size=15>\n");
	printf("%s", "<br><input type=submit value=Enter ></form>\n");

	if (info->main_url != 0)
	{
		if (info->link_text != 0)
		{
			printf("%s", "<a href=\"");
			printf("%s", info->main_url);
			printf("%s", "\">");
			printf("%s", info->link_text);
			printf("%s", "</a><br>");
		}
		else
		{
			printf("%s", "<a href=\"");
			printf("%s", info->main_url);
			printf("%s", "\">Click here</a> to return home.");
			printf("%s", "<br>");
		}
	}

	if (info->use_isp_help)
	{
		printf("%s", "<BR>");
		printf("%s", info->isp_url);
		printf("%s", "<br>");
	}
	if (info->use_authorinfo)
	{
		printf("%s", "<br>Written by <a href=\"http://ashaw.dyn.cheapnet.net/webmail\">");
		printf("%s", "Andrew Shaw</a>\n");
		printf("%s", "<br>Best viewed with a 4.0 browser with javascript enabled.\n");
	}

/* not implemented yet
	//warn if browser is incompatible
	if (strncmp(browsertype, "Konquerer/1", 11) == 0 ||
	strncmp(browsertype, "Lynx/2", 6) == 0 ||
	strncmp(browsertype, "Mozilla/3", 9) == 0 ||
	strncmp(browsertype, "Mozilla/2", 9) == 0 ||
	strncmp(browsertype, "Mozilla/1", 9) == 0 ||
	strncmp(browsertype, "Lynx/1", 6) == 0)
	{	
		cout << "<font color=\"#FF0000\">";
		cout << "<br><br>Your browser does not pass the current system test!!!";
		cout << "<br>Web e-mail may not work with your browser!!!";
		cout << "</font>";
	}
*/

	printf("%s", "</center>\n");
	
	printf("%s", "</td></tr></table>\n");

	printf("%s", "</body>\n");
	printf("%s", "</html>\n");
	return;
}

//-- Error -------------------------------------

void display_error(struct plugin_data *info, char *error_message)
{

	printf("%s", "<html>\n");
	printf("%s", "<head>\n");
	printf("%s", "<title>Web E-Mail</title>\n");
	printf("%s", "<META HTTP-EQUIV=\"expires\" content=\"0\">\n");
	printf("%s", "</head>\n");
	printf("%s", "<body text=\"#000000\" bgcolor=\"#FFFFFF\" link=\"#0000EF\"");
	printf("%s", " vlink=\"#0000EF\" alink=\"#FF0000\" background=/email/bg1.gif>\n");
 	printf("%s", "&nbsp;");

	display_header(info, "Error");

	printf("%s", "<br>An error has occurred.<br>");
	printf("%s", "Please contact your system administrator with the following information below.");
	printf("%s", "<center>\n");
	printf("%s", "<p><u><font color=\"#000000\">\n");

	printf("%s", error_message);

	printf("%s", "</font></u></center>\n");
	printf("%s", "<p>\n");

	display_tail(info);

	printf("%s", "</body>\n");
	printf("%s", "</html>\n");
	return;
}

//-- Address Book -----------------------------

void display_address_head(struct plugin_data *info)
{
	char inc;

        printf("%s", "<html>\n");
        printf("%s", "<head>\n");
        printf("%s", "<title>Web E-Mail</title>\n");
        printf("%s", "<META HTTP-EQUIV=\"expires\" content=\"0\">\n");
        printf("%s", "</head>");
        printf("%s", "<body text=\"#000000\" bgcolor=\"#FFFFFF\" ");
        printf("%s", "link=\"#0000EF\" vlink=\"#0000EF\" alink=\"#FF0000\" background=/email/bg1.gif>\n");

        display_header(info, "Address Book");

        //prints index
        for (inc = 'A'; inc <= 'Z'; inc++)
        {
                printf("%s", "<a href=\"");
		printf("%s", info->url);
		printf("%s", "?book#");
	 	printf("%c\">%c</a> ", inc, inc);
        }

        printf("%s", "<br><a href=\"");
	printf("%s", info->url);
	printf("%s", "?add_single_addr\">Add Entree</a>\n");
        printf("%s", "&nbsp;<a href=\"");
	printf("%s", info->url);
	printf("%s", "?add_group_addr\">Add Group</a><p>\n");

	return;
}

void display_address_letterbreak(struct plugin_data *info, char letter)
{

	printf("%s", "<a NAME=\"");
	printf("%c", letter);
	printf("%s", "\"></a><font size=+2>");
	printf("%c", letter);
	printf("%s", "</font><br>");

	return;
}

void display_single_entre(struct plugin_data *info, struct addr_single_entry *addressdata)
{
	printf("%s", "<table CELLSPACING=0 CELLPADDING=0 ");
	printf("%s", "WIDTH=\"100%\" BGCOLOR=\"#FF6666\" NOSAVE >");
	printf("%s", "<tr NOSAVE>\n");
	printf("%s", "<td WIDTH=\"50%\" NOSAVE>");

	display_escapeprint(addressdata->name);

	printf("%s", "&nbsp;</td>");
	printf("%s", "<td><a href=\"");
	printf("%s", info->url);
	printf("?s_sendto_addr=%d\">", addressdata->uid);

	display_escapeprint(addressdata->email_address);

	printf("%s", "</a>&nbsp;</td>");
	printf("%s", "</tr></table>\n");

	printf("%s", "<table CELLSPACING=0 CELLPADDING=0 WIDTH=\"100%\" BGCOLOR=\"#FFCCCC\" NOSAVE >");
	printf("%s", "<tr NOSAVE>\n");
	printf("%s", "<td WIDTH=\"10%\" BGCOLOR=\"#FFCC33\" NOSAVE>\n");
	printf("%s", "<div align=right>Phone:</div>\n");
	printf("%s", "</td>\n");

	printf("%s", "<td WIDTH=\"40%\" BGCOLOR=\"#FFCC99\" NOSAVE>");

	display_escapeprint(addressdata->phone);

	printf("%s", "&nbsp;</td>\n");
	printf("%s", "<td WIDTH=\"10%\" BGCOLOR=\"#66FFFF\" NOSAVE>\n");
	printf("%s", "<div align=right>Address:</div>");
	printf("%s", "</td>\n");
	printf("%s", "<td WIDTH=\"40%\" BGCOLOR=\"#CCFFFF\" NOSAVE>");

	display_escapeprint(addressdata->address_line_1);

	printf("%s", "&nbsp;</td>\n");
	printf("%s", "</tr>\n");
	printf("%s", "<tr NOSAVE>\n");
	printf("%s", "<td WIDTH=\"10%\" BGCOLOR=\"#FFCC33\" NOSAVE>\n");
	printf("%s", "<div align=right>Fax:</div>\n");
	printf("%s", "</td>\n");
	printf("%s", "<td WIDTH=\"40%\" BGCOLOR=\"#FFCC99\" NOSAVE>");

	display_escapeprint(addressdata->fax);

	printf("%s", "&nbsp;</td>\n");
	printf("%s", "<td WIDTH=\"10%\" BGCOLOR=\"#66FFFF\" NOSAVE>&nbsp;</td>\n");
	printf("%s", "<td WIDTH=\"40%\" BGCOLOR=\"#CCFFFF\" NOSAVE>");
	display_escapeprint(addressdata->address_line_2);

	printf("%s", "&nbsp;</td>\n");
	printf("%s", "</tr></table>\n");
	printf("%s", "<table CELLSPACING=0 CELLPADDING=0 WIDTH=\"100%\" NOSAVE >\n");
	printf("%s", "<tr NOSAVE>\n");
	printf("%s", "<td WIDTH=\"7%\" BGCOLOR=\"#FF9797\" NOSAVE>\n");
	printf("%s", "<div align=right>Notes:</div>");
	printf("%s", "</td>\n");
	printf("%s", "<td BGCOLOR=\"#FFCCCC\" NOSAVE>");

	display_escapeprint(addressdata->notes);

	printf("%s", "&nbsp;</td>\n");
	printf("%s", "<td WIDTH=\"10%\" BGCOLOR=\"#FF9797\" NOSAVE>\n");
	printf("%s", "<center><a href=\"");

	printf("%s", info->url);

	printf("?edit_single_addr=%d\">\n", addressdata->uid);
	printf("%s", "Edit</a></center>\n");
	printf("%s", "</td>\n");
	printf("%s", "<td WIDTH=\"10%\" BGCOLOR=\"#FF9797\" NOSAVE>\n");
	printf("%s", "<center><a href=\"");

	printf("%s", info->url);

	printf("?delete_addr=%d\">\n", addressdata->uid);
	printf("%s", "Delete</a></center>\n");
	printf("%s", "</td></tr></table>\n");

	return;

}

void display_group_entre(struct plugin_data *info, struct addr_group_entry *groupdata)
{

	printf("%s", "<table CELLSPACING=0 CELLPADDING=0 WIDTH=\"100%\" NOSAVE>\n");
	printf("%s", "<tr NOSAVE>\n");
	printf("%s", "<td WIDTH=\"20%\" BGCOLOR=\"#FF6666\" NOSAVE>\n");
	printf("%s", "<a href=\"");
	printf("%s", info->url);
	printf("?g_sendto_addr=%d\">\n", groupdata->uid);

	display_escapeprint(groupdata->name);

	printf("%s", "</a></td>\n");
        printf("%s", "<td BGCOLOR=\"#FFCCCC\" NOSAVE>Group List</td>\n");
	printf("%s", "<td WIDTH=\"10%\" BGCOLOR=\"#FF6666\" NOSAVE>\n");
        printf("%s", "<center><a href=\"");

	printf("%s", info->url);

	printf("?edit_group_addr=%d\">Edit</a></center>\n", groupdata->uid);
        printf("%s", "</td><td WIDTH=\"10%\" BGCOLOR=\"#FF6666\" NOSAVE>\n");
	printf("%s", "<center><a href=\"");
	printf("%s", info->url);
	printf("?delete_group_addr=%d\">Delete</a></center>\n", groupdata->uid);

	printf("%s", "</td></tr></table>\n");

	return;
}

void display_address_tail(struct plugin_data *info)
{
	char inc;

	printf("%s", "<br>\n");

        for (inc = 'A'; inc <= 'Z'; inc++)
        {
                printf("%s", "<a href=\"");
		printf("%s", info->url);
		printf("%s", "?book#");
		printf("%c\">%c</a> ", inc, inc);
        }


        printf("%s", "<br><a href=\"");
	printf("%s", info->url);
	printf("%s", "?add_single_addr\">Add Entree</a>\n");
        printf("%s", "&nbsp;<a href=\"");
	printf("%s", info->url);
	printf("%s", "?add_group_addr\">Add Group</a><p>\n");

        printf("%s", "<br>\n");

        display_tail(info);

        printf("%s", "</body>\n");
        printf("%s", "</html>\n");

	return;
}

void display_address_add_group_head(struct plugin_data *info, struct addr_group_entry *groupdata, int addedit)
{
	printf("%s", "<html>\n");
        printf("%s", "<head>\n");
        printf("%s", "<title>Web E-Mail</title>\n");
        printf("%s", "<META HTTP-EQUIV=\"expires\" content=\"0\">\n");
        printf("%s", "</head>\n");
        printf("%s", "<body text=\"#000000\" bgcolor=\"#FFFFFF\" link=\"#0000EF\" ");
        printf("%s", "vlink=\"#0000EF\" alink=\"#FF0000\" background=/email/bg1.gif>");

	if (addedit == 1)
                display_header(info, "Edit Group List");
        else
                display_header(info, "Add Group List");

       if (addedit == 1)
        {
		printf("%s", "<form Method=\"post\" action=\"");
		printf("%s", info->url);
		printf("?reprocess_group=%d\">Group Name:&nbsp;", groupdata->uid);
        }
        else
        {
                printf("%s", "<form Method=\"post\" action=\"");
		printf("%s", info->url);
		printf("%s", "?process_group\">Group Name:&nbsp;");
        }

	printf("%s", " <input type=text name=group size=25 value=\"");
	printf("%s", groupdata->name);
	printf("%s", "\">");

	printf("%s", "<br>E-Mail Address: (One e-mail address per line)\n");
        printf("%s", "<br><textarea wrap=virtual cols=80 rows=20 name=addresses>\n");
	return;
}

void display_address_add_group_tail(struct plugin_data *info, struct addr_group_entry *groupdata, int addedit)
{
	printf("%s", "</textarea>\n");

        printf("%s", "<br><input type=Submit value=\"Add Entree\"><input type=reset value=Reset >");
        printf("%s", "<br></form>\n");
        display_tail(info);
        printf("%s", "</body>\n");
        printf("%s", "</html>\n");
	return;
}


void display_address_add_single(struct plugin_data *info, struct addr_single_entry *addressdata, int addedit)
{
        printf("%s", "<html>\n");
        printf("%s", "<head>\n");
        printf("%s", "<title>Web E-Mail</title>\n");
        printf("%s", "<META HTTP-EQUIV=\"expires\" content=\"0\">\n");
        printf("%s", "</head>\n");
        printf("%s", "<body text=\"#000000\" bgcolor=\"#FFFFFF\" ");
        printf("%s", "link=\"#0000EF\" vlink=\"#0000EF\" alink=\"#FF0000\" background=/email/bg1.gif>");
        printf("%s", "<b></b>&nbsp;");

	if (addedit == 0)
                display_header(info, "Add Address Entree");

        else
        	display_header(info, "Edit Address Entree");

	if (addedit == 0)
        {
                printf("%s", "<form method=\"post\" action=\"");
		printf("%s", info->url);
		printf("%s", "?process_single\">");
        }
        else
        {
                printf("%s", "<form method=\"post\" action=\"");
		printf("%s", info->url);
		printf("?reprocess_single=%d\">", addressdata->uid);
        }

        printf("%s", "<table CELLSPACING=0 CELLPADDING=0 WIDTH=\"100%\" NOSAVE>");
        printf("%s", "<tr NOSAVE>\n");
        printf("%s", "<td NOSAVE>\n");
        printf("%s", "<div align=right>Name:</div>\n");
        printf("%s", "</td>\n");

	printf("%s", "<td><input type=text name=name size=25 value=\"");
	printf("%s", addressdata->name);
	printf("%s", "\"></td>");

        printf("%s", "<td>\n");
        printf("%s", "<div align=right>E-Mail:</div>\n");
        printf("%s", "</td>\n");

	printf("%s", "<td><input type=text name=email size=25 value=\"");
	printf("%s", addressdata->email_address);
	printf("%s", "\"></td>");


	printf("%s", "</tr>\n");
        printf("%s", "<tr NOSAVE>\n");
        printf("%s", "<td>\n");
        printf("%s", "<div align=right>Phone:&nbsp;</div>\n");
        printf("%s", "</td>\n");

	printf("%s", "<td><input type=text name=phone size=25 value=\"");
	printf("%s", addressdata->phone);
	printf("%s", "\"></td>");

        printf("%s", "<td>\n");
        printf("%s", "<div align=right>Address line 1:</div>\n");
        printf("%s", "</td>\n");

	printf("%s", "<td NOSAVE><input type=text name=addressline1 size=25 value=\"");
	printf("%s", addressdata->address_line_1);
	printf("%s", "\"></td>");

	printf("%s", "</tr>\n");
        printf("%s", "<tr>\n");
        printf("%s", "<td>\n");
        printf("%s", "<div align=right>Fax:&nbsp;</div>\n");
        printf("%s", "</td>\n");

	printf("%s", "<td><input type=text name=fax size=25 value=\"");
	printf("%s", addressdata->fax);
	printf("%s", "\"></td>");

        printf("%s", "<td>\n");
        printf("%s", "<div align=right>Address line 2:</div>\n");
        printf("%s", "</td>\n");

	printf("%s", "<td><input type=text name=addressline2 size=25 value=\"");
	printf("%s", addressdata->address_line_2);
	printf("%s", "\"></td>\n");

	printf("%s", "</tr>\n");
        printf("%s", "<tr NOSAVE>\n");
        printf("%s", "<td>\n");
        printf("%s", "<div align=right>Notes:</div>\n");
        printf("%s", "</td>\n");

	printf("%s", "<td COLSPAN=\"3\" NOSAVE><input type=text name=notes size=50 value=\"");
	printf("%s", addressdata->notes);
	printf("%s", "\"></td>");

	printf("%s", "<td></td>\n");
        printf("%s", "<td></td>\n");
        printf("%s", "</tr>\n");
        printf("%s", "</table>\n");
        printf("%s", "<input type=Submit value=\"Add Entree\"><input type=reset value=Reset ></form>");
        printf("%s", "<br>");

        display_tail(info);

        printf("%s", "</body>\n");
        printf("%s", "</html>\n");
	return;

}

//-- view html --------------------------------------------
void display_view_html_head(struct plugin_data *info, int message_number)
{

	printf("%s", "<html><head>\n");
	printf("%s", "<title>Web E-Mail</title>\n");
	printf("%s", "<META HTTP-EQUIV=\"expires\" content=\"0\">\n");
	printf("%s", "</head>\n");
	printf("%s", "<body text=\"#000000\" bgcolor=\"#FFFFFF\" link=\"#0000EF\" ");
	printf("%s", "vlink=\"#0000EF\" alink=\"#FF0000\" background=/email/bg1.gif>");

	display_header(info, "HTML View");

	printf("%s", "<a href=\"");
	printf("%s", info->url);
	printf("%s", "?read_inbox=");
	printf("%d", message_number);
	printf("%s", "\">Normal View</a>&nbsp;&nbsp;");
	printf("%s", "<a href=\"");
	printf("%s", info->url);
	printf("%s", "?viewprint_inbox=");
	printf("%d", message_number);
	printf("%s", "\">Printable view</a>&nbsp;&nbsp;");
	printf("%s", "<br><br>\n");
	return;
}
void display_saved_view_html_head(struct plugin_data *info, int message_number)
{

	printf("%s", "<html><head>\n");
	printf("%s", "<title>Web E-Mail</title>\n");
	printf("%s", "<META HTTP-EQUIV=\"expires\" content=\"0\">\n");
	printf("%s", "</head>\n");
	printf("%s", "<body text=\"#000000\" bgcolor=\"#FFFFFF\" link=\"#0000EF\" ");
	printf("%s", "vlink=\"#0000EF\" alink=\"#FF0000\" background=/email/bg1.gif>");

	display_header(info, "HTML View");

	printf("%s", "<a href=\"");
	printf("%s", info->url);
	printf("%s", "?read_saved=");
	printf("%d", message_number);
	printf("%s", "\">Normal View</a>&nbsp;&nbsp;");
	printf("%s", "<a href=\"");
	printf("%s", info->url);
	printf("%s", "?viewprint_saved=");
	printf("%d", message_number);
	printf("%s", "\">Printable view</a>&nbsp;&nbsp;");
	printf("%s", "<br><br>\n");
	return;
}
void display_sent_view_html_head(struct plugin_data *info, int message_number)
{

	printf("%s", "<html><head>\n");
	printf("%s", "<title>Web E-Mail</title>\n");
	printf("%s", "<META HTTP-EQUIV=\"expires\" content=\"0\">\n");
	printf("%s", "</head>\n");
	printf("%s", "<body text=\"#000000\" bgcolor=\"#FFFFFF\" link=\"#0000EF\" ");
	printf("%s", "vlink=\"#0000EF\" alink=\"#FF0000\" background=/email/bg1.gif>");

	display_header(info, "HTML View");

	printf("%s", "<a href=\"");
	printf("%s", info->url);
	printf("%s", "?read_sent=");
	printf("%d", message_number);
	printf("%s", "\">Normal View</a>&nbsp;&nbsp;");
	printf("%s", "<a href=\"");
	printf("%s", info->url);
	printf("%s", "?viewprint_sent=");
	printf("%d", message_number);
	printf("%s", "\">Printable view</a>&nbsp;&nbsp;");
	printf("%s", "<br><br>\n");
	return;
}


void display_view_html_tail(struct plugin_data *info, int message_number)
{
	printf("%s", "<br><br>\n");
        display_tail(info);
        printf("%s", "</body></HTML>\n");
	return;
}
void display_saved_view_html_tail(struct plugin_data *info, int message_number)
{
	printf("%s", "<br><br>\n");
        display_tail(info);
        printf("%s", "</body></HTML>\n");
	return;
}
void display_sent_view_html_tail(struct plugin_data *info, int message_number)
{
	printf("%s", "<br><br>\n");
        display_tail(info);
        printf("%s", "</body></HTML>\n");
	return;
}


void display_java_script_head(struct plugin_data *info)
{
	printf("%s", "<html>\n");
        printf("%s", "<head>\n");
        printf("%s", "<title>Web E-Mail</title>\n");
        printf("%s", "<META HTTP-EQUIV=\"expires\" content=\"0\">\n");
        printf("%s", "</head>\n");
        printf("%s", "<body text=\"#000000\" bgcolor=\"#FFFFFF\" link=\"#0000EF\" ");
	printf("%s", "vlink=\"#0000EF\" alink=\"#FF0000\"");
        printf("%s", " background=/email/bg1.gif>\n");


	//Java script will be displayed after this message, do not interfear with it!!!!!!
}
void display_java_script_tail(struct plugin_data *info)
{
	printf("%s", "</body>");
        printf("%s", "</html>");
}
