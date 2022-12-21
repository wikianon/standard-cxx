/*
	inbox_page.cpp
*/



#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <syslog.h>

#include "main.h"
#include "parse_mail_headers.h"
#include "inbox_page.h"
#include "parse_form.h"
#include "headers.h"
#include "config.h"
#include "err_page.h"

enum m_sort mail_sort;

int inbox_page(struct mail_header**, int);
int display_headers(struct mail_header**, int);
int upper_table(void);
int subject_headers(void);

//////////////////////////////////////////////////////////////////////

int inbox_action(void)
{
   struct mail_header* mh_data=NULL;
   struct mail_header** mh_array=NULL;
   int num_messages=0;
   cgi_t* cgi_data=NULL;

   mail_sort=ms_date_d;

   /* read cgi values and see if you wanted to sort messages */
   while( get_cgi_data(&cgi_data) == 0)
   {
      if( strcmp(cgi_data->name,"sort") == 0 && cgi_data->value != NULL)
      {
         if(strcmp(cgi_data->value,"date_a") == 0)
            mail_sort=ms_date_a;
         else if(strcmp(cgi_data->value,"date_d") == 0)
            mail_sort=ms_date_d;
         else if(strcmp(cgi_data->value,"sender_a") == 0)
            mail_sort=ms_sender_a;
         else if(strcmp(cgi_data->value,"sender_d") == 0)
            mail_sort=ms_sender_d;
         else if(strcmp(cgi_data->value,"subj_a") == 0)
            mail_sort=ms_subj_a;
         else if(strcmp(cgi_data->value,"subj_d") == 0)
            mail_sort=ms_subj_d;
         else if(strcmp(cgi_data->value,"size_a") == 0)
            mail_sort=ms_size_a;
         else if(strcmp(cgi_data->value,"size_d") == 0)
            mail_sort=ms_size_d;

         free(cgi_data->name);
         cgi_data->name=NULL; 
         free(cgi_data->value);
         cgi_data->value=NULL;
      }
   }

   /* read mail headers from users mailfile */
   mh_data=get_mail_headers();

   if(mh_data == NULL && errno != 0)
   {
      syslog(LOG_MAIL | LOG_ERR,"error reading messages from mailfile");
      err_page();
      return(-1);
   }

   /* sort messages */
   num_messages=sort_mh(mh_data,&mh_array);

   /* display inbox page */
   inbox_page(mh_array,num_messages);

   free_mail_headers(mh_data); 
   free(mh_array);

   return(0);
}

//////////////////////////////////////////////////////////////////////

int inbox_page(struct mail_header** mh_array, int num_messages)
{

   content_html();
   html_header("Inbox");

   printf("<FORM METHOD=\"POST\" ACTION=\"%s\">\n" ,user_env.script_name);
   printf("   <INPUT TYPE=\"HIDDEN\" "
		"NAME=\"action\" VALUE=\"delete_messages\">\n");
   printf("   <INPUT TYPE=\"HIDDEN\" "
		"NAME=\"sessionid\" VALUE=\"%s\">\n\n",
		user_ses.sessionid);


   printf("<TABLE BORDER=\"0\" ALIGN=\"CENTER\" WIDTH=\"95%%\" "
          "CELLPADDING=\"1\" CELLSPACING=\"1\" >\n");


   upper_table();
   subject_headers();
   display_headers(mh_array,num_messages);

   printf("\n   <TR>\n"
          "      <TD COLSPAN=\"6\">\n"
          "         <INPUT TYPE=\"SUBMIT\" VALUE=\"Delete Selected\">\n"
          "      </TD>\n"
          "   </TR>\n\n"
          "</TABLE>\n"
          "</FORM>\n");

   html_footer();

   return(0);
}


//////////////////////////////////////////////////////////////////////

int upper_table(void)
{

//TOP LINE

   printf("   <TR>\n"
          "      <TD COLSPAN=\"6\" BGCOLOR=\"#002266\" ALIGN=\"CENTER\">\n"
          "         <TABLE BORDER=\"0\" WIDTH=\"100%%\" CELLPADDING=\"0\" "
                    "CELLSPACING=\"0\">\n"
          "            <TR>\n"
          "               <TD ALIGN=\"LEFT\" WIDTH=\"33%%\">\n"
          "                  <FONT COLOR=\"#FFFFFF\" "
                             "FACE=\"Arial, Helvetica\" SIZE=\"3\">\n"
          "                  <B>Inbox");

   if(user_ses.name != NULL)
      printf(" - %s",user_ses.name);
   else if(user_ses.loginname != NULL)
      printf(" - %s",user_ses.loginname);

   printf("</B>\n"
          "                  </FONT>\n"
          "               </TD>\n"
          "            </TR>\n"
          "         </TABLE>\n"
          "      </TD>\n"
          "   </TR>\n");


//SELECTION LINE


   printf("   <TR>\n"
          "      <TD BGCOLOR=\"#DCDCDC\" COLSPAN=\"6\">\n"
          "         <TABLE BORDER=\"0\" WIDTH=\"100%%\">\n"
          "            <TR>\n"
          "               <TD VALIGN=\"MIDDLE\" ALIGN=\"LEFT\">\n"
          "                  <FONT FACE=\"Arial, Helvetica\" SIZE=\"2\">\n");
   printf("                  <A "
          "HREF=\"%s?action=compose_message&sessionid=%s\">\n",
		user_env.script_name,user_ses.sessionid);
   printf("                  Compose</A> | \n");

   printf("                  <A "
          "HREF=\"%s?action=edit_pref&sessionid=%s\">\n",
		user_env.script_name,user_ses.sessionid);
   printf("                  User prefs</A> | \n");
   printf("                  <A "
		"HREF=\"%s?action=view_addr_book&sessionid=%s\">\n",
		user_env.script_name,user_ses.sessionid);
   printf("                  Address book</A> | \n");

   printf("                  <A "
		"HREF=\"%s?action=view_inbox&sessionid=%s\">\n",
		user_env.script_name,user_ses.sessionid);
   printf("                  Refresh</A> | \n");

   printf("                  <A "
		"HREF=\"%s?action=logout&sessionid=%s\">\n",
		user_env.script_name,user_ses.sessionid);
   printf("                  Logout</A>\n"
          "                  </FONT>\n"
          "               </TD>\n"
          "            </TR>\n");

   printf("         </TABLE>\n"
          "      </TD>\n"
          "   </TR>\n");

		
   printf("   <TR>\n"
          "      <TD COLSPAN=\"6\">\n"
          "         <BR>\n"
          "      </TD>\n"
          "   </TR>\n\n");

   return(0);
}

//////////////////////////////////////////////////////////////////////

int subject_headers(void)
{

//select
   printf("   <TR BGCOLOR=\"#D3DCE3\">\n"
          "      <TD WIDTH=\"50\" ALIGN=\"CENTER\">\n"
          "         <B>Select</B>\n"
          "      </TD>");

//status
   printf("      <TD ALIGN=\"CENTER\" WIDTH=\"50\">\n"
          "         <B>Status</B>\n"
          "      </TD>\n");
//date
   printf("      <TD VALIGN=\"CENTER\" WIDTH=\"130\">\n");
   printf("         <A HREF=\"%s?action=view_inbox&sessionid=%s&"
			"sort=%s\">\n",
			user_env.script_name,
			user_ses.sessionid,
			mail_sort != ms_date_d ? "date_d" : "date_a");
   printf("         <B>Date</B></A>\n");

   if(mail_sort == ms_date_a)
      printf("         <IMG SRC=\"" NMHTTP "/up.gif\" "
			"HEIGHT=\"12\" WIDTH=\"12\">\n");
   else if(mail_sort == ms_date_d)
      printf("         <IMG SRC=\"" NMHTTP "/down.gif\" "
			"HEIGHT=\"12\" WIDTH=\"12\">\n");

   printf("      </TD>\n");

//from
   printf("      <TD WIDTH=\"150\">\n");
   printf("         <A HREF=\"%s?action=view_inbox&sessionid=%s&"
			"sort=%s\">\n",
			user_env.script_name,
			user_ses.sessionid,
			mail_sort != ms_sender_d ? "sender_d" : "sender_a");
   printf("         <B>Sender</B></A>\n");

   if(mail_sort == ms_sender_a)
      printf("         <IMG SRC=\"" NMHTTP "/up.gif\" "
			"HEIGHT=\"12\" WIDTH=\"12\">\n");
   else if(mail_sort == ms_sender_d)
      printf("         <IMG SRC=\"" NMHTTP "/down.gif\" "
			"HEIGHT=\"12\" WIDTH=\"12\">\n");

   printf("      </TD>\n");

//subject
   printf("      <TD>\n");
   printf("         <A HREF=\"%s?action=view_inbox&sessionid=%s&"
			"sort=%s\">\n",
			user_env.script_name,
			user_ses.sessionid,
			mail_sort != ms_subj_d ? "subj_d" : "subj_a" );
   printf("         <B>Subject</B></A>\n");

   if(mail_sort == ms_subj_a)
      printf("         <IMG SRC=\"" NMHTTP "/up.gif\" "
			"HEIGHT=\"12\" WIDTH=\"12\">\n");
   else if(mail_sort == ms_subj_d)
      printf("         <IMG SRC=\"" NMHTTP "/down.gif\" "
			"HEIGHT=\"12\" WIDTH=\"12\">");

   printf("      </TD>\n");
//size
   printf("      <TD WIDTH=\"50\">\n");
   printf("         <A HREF=\"%s?action=view_inbox&sessionid=%s&"
			"sort=%s\">\n",
			user_env.script_name,
			user_ses.sessionid,
			mail_sort != ms_size_d ? "size_d" : "size_a" );
   printf("         <B>Size</B></A>\n");

   if(mail_sort == ms_size_a)
      printf("         <IMG SRC=\"" NMHTTP "/up.gif\" "
			"HEIGHT=\"12\" WIDTH=\"12\">\n");
   else if (mail_sort == ms_size_d)
      printf("         <IMG SRC=\"" NMHTTP "/down.gif\" "
			"HEIGHT=\"12\" WIDTH=\"12\">\n");

   printf("      </TD>\n"
          "   </TR>\n\n");

   return(0);
}


//////////////////////////////////////////////////////////////////////


int display_headers(struct mail_header** mh_array,int num_messages)
{
   int x;
   struct tm* time_data=NULL;

   char time_string[50];

   for(x=0;x<50;x++)
      time_string[x]=0;

   for(x=0;x<num_messages;x++)
   {

      printf("\n   <TR BGCOLOR=\"%s\">\n",
		x%2 == 0 ? "#DDDDDD" : "#EEEEEE");
//select
      printf("      <TD VALIGN=\"TOP\" ALIGN=\"CENTER\">\n");
      printf("         <INPUT TYPE=\"CHECKBOX\" NAME=\"%d\" VALUE=\"%ld\">\n", 
			mh_array[x]->m_num,
			mh_array[x]->date_received);
      printf("      </TD>\n");

//status
      printf("      <TD VALIGN=\"TOP\" ALIGN=\"CENTER\">\n");

      printf("         <A "
		"HREF=\"%s?action=view_message&sessionid=%s&m_num=%d\">\n",
		user_env.script_name,
		user_ses.sessionid,
		mh_array[x]->m_num);
	
      printf("         <IMG SRC=\"" NMHTTP "/%s\" WIDTH=\"14\" "
			"HEIGHT=\"12\" BORDER=\"0\">",
			mh_array[x]->mail_status==m_new ? 
			"mailnew.gif" : "mailopened.gif");

      printf("</A>\n"
             "         <IMG SRC=\"" NMHTTP);

      if(mh_array[x]->cont_type == ct_multipart_mixed ||
		mh_array[x]->cont_type == ct_multipart_parallel ||
		mh_array[x]->cont_type == ct_multipart_digest ||
		mh_array[x]->cont_type == ct_multipart_other)
         printf("/paperclip.gif");
      else
         printf("/blankclip.gif");
      printf("\" WIDTH=\"6\" HEIGHT=\"11\">\n");

//date
      printf("      </TD>\n"
             "      <TD VALIGN=\"TOP\">\n"
             "         ");

      time_data=localtime(&(mh_array[x]->date_received));

      strftime(time_string,50,"%m/%d/%y&nbsp;&nbsp;%I:%M %p",time_data);
//      strftime(time_string,50,"%x&nbsp;&nbsp;%X",time_data);

      if(mh_array[x]->mail_status == m_new)
         printf("<B>");

      printf("%s",time_string);

      if(mh_array[x]->mail_status == m_new)
         printf("</B>\n");
      else
         printf("\n");
      printf("      </TD>\n");

//sender
      printf("      <TD VALIGN=\"TOP\">\n"
             "         ");

      if(mh_array[x]->mail_status == m_new)
         printf("<B>");

      if(mh_array[x]->from != NULL)
      {
         if(mh_array[x]->from[0].name != NULL)
            printf("%s",mh_array[x]->from[0].name);
         else if(mh_array[x]->from[0].email != NULL)
            printf("%s",mh_array[x]->from[0].email);
      }
      else
         printf(" ");

      if(mh_array[x]->mail_status == m_new)
         printf("</B>\n");
      else
         printf("\n");

//subject
      printf("      </TD>\n"
             "      <TD VALIGN=\"TOP\">\n"
             "         ");

      if(mh_array[x]->mail_status == m_new)
         printf("<B>");

      printf("<A HREF=\"%s?action=view_message&sessionid=%s&m_num=%d\">\n",
		user_env.script_name,
		user_ses.sessionid,
		mh_array[x]->m_num);
		

      if(mh_array[x]->subject != NULL)
         printf("         %s</A>",mh_array[x]->subject);
      else
         printf("         &nbsp</A>");

      if(mh_array[x]->mail_status == m_new)
         printf("</B>\n");
      else
         printf("\n");

//size
      printf("      </TD>\n"
             "      <TD VALIGN=\"TOP\">\n"
             "         ");

      if(mh_array[x]->mail_status == m_new)
         printf("<B>");

      if(mh_array[x]->size_of_message < 1024)
      {
         printf("%d",mh_array[x]->size_of_message);
      }
      else if(mh_array[x]->size_of_message < 1024*1024)
      {
         printf("%d KB",mh_array[x]->size_of_message/1024);
      }
      else
      {
         printf("%d MB",mh_array[x]->size_of_message/(1024*1024));
      }

      if(mh_array[x]->mail_status == m_new)
         printf("</B>\n");
      else
         printf("\n");

      printf("      </TD>\n"
             "   </TR>\n");
   }   

   return(0);
}


//////////////////////////////////////////////////////////////////////






