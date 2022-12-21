/*
	compose_message_page.cpp
*/

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <errno.h>

#include "headers.h"
#include "session.h"
#include "main.h"
#include "addr.h"
#include "parse_form.h"
#include "err_page.h"
#include "parse_mail_headers.h"
#include "mail_process.h"
#include "view_message.h"
#include "encoding_conv.h"
#include "config.h"
#include "preferences.h"

int compose_message_header(void);
int compose_message_form(mail_header*, FILE*);
int to_line_value(mail_header*);
int cc_line_value(mail_header*);
int subject_line_value(mail_header*);
status_type body_value(mail_header*, FILE*);
int disp_addr(addr_t*, int*);
status_type refw_body_send(FILE*, mail_header*);
status_type send_body(FILE*, char*, content_type_encoding);
int find_email_in_book(addr_t*, addr_t*, addr_t*, addr_t*);

///////////////////////////////////////////////////////////////////

int compose_message_action(void)
{
   int m_num=0;
   cgi_t* cgi_data=NULL;
   mail_header* mail_h=NULL;
   FILE* user_mail_file=NULL;

   /* if not compose message, this is a reply or forward, m_num should
      be in cgi environment */
   if(mail_action != compose_message)
   {
      while(get_cgi_data(&cgi_data) == 0)
      {
         if(strcmp(cgi_data->name,"m_num") == 0 && cgi_data->value != NULL)
            m_num=atol(cgi_data->value);
      }

      if(m_num <= 0)
      {
         syslog(LOG_MAIL | LOG_ERR,"replay/forward without m_num");
         err_page();
         return(-1);
      }

      setgid(egid);
      user_mail_file=open_user_mailfile("r");
      setgid(rgid);

      if(user_mail_file == NULL)
      {
         syslog(LOG_MAIL | LOG_ERR,"Could not open user mail file %s (%s)",
			user_ses.loginname,strerror(errno));
         err_page();
         return(-1);
      }

      if(point_to_message(user_mail_file,m_num) != 0)
      {
         fclose(user_mail_file);
         err_string_page("Error: page must have been expired.");
         return(-1);
      }

      mail_h=fill_mail_h_struct(user_mail_file,m_num);

      if(mail_h == NULL)
      {
         err_page();
         fclose(user_mail_file);
         return(-1);
      }
   }

   content_html();
   html_header("Compose Message");

   compose_message_header();
   compose_message_form(mail_h,user_mail_file);

   html_footer();

   free_mail_headers(mail_h);

   if(user_mail_file != NULL)
      fclose(user_mail_file);

   return(0);
}

///////////////////////////////////////////////////////////////////

int compose_message_header(void)
{
   if(user_ses.sessionid == NULL || user_env.script_name == NULL)
      return(-1);

   printf("<FORM METHOD=\"POST\" ENCTYPE=\"multipart/form-data\" "
	  "NAME=\"composeform\" ACTION=\"%s\">\n",user_env.script_name);
   printf("   <INPUT TYPE=\"hidden\" NAME=\"action\" "
          "VALUE=\"send_message\">\n"
          "   <INPUT TYPE=\"hidden\" NAME=\"sessionid\" "
          "VALUE=\"%s\">\n\n",user_ses.sessionid);

   printf("<TABLE BORDER=\"0\" ALIGN=\"CENTER\" "
	  "CELLPADDING=\"1\" CELLSPACING=\"1\">\n"
          "   <TR>\n"
	  "      <TD COLSPAN=\"2\" BGCOLOR=\"#002266\" ALIGN=\"LEFT\">\n"
          "         <FONT COLOR=\"#FFFFFF\" FACE=\"Arial, Helvetica\" "
	  "SIZE=\"3\">\n"
	  "         <B>COMPOSE MESSAGE</B></FONT>\n"
          "      </TD>\n"
	  "   </TR>\n"
          "   <TR>\n"
          "      <TD COLSPAN=\"2\" BGCOLOR=\"#DCDCDC\" ALIGN=\"LEFT\">\n"
          "         <TABLE BORDER=\"0\" WIDTH=\"100%%\">\n"
	  "            <TR>\n"
	  "               <TD>\n"
          "                  <A HREF=\"%s?action=view_inbox&sessionid=%s\">\n",
	  		user_env.script_name,user_ses.sessionid);

   printf("                  Inbox</A> |\n"
	  "                  <A HREF=\"%s?action=logout&sessionid=%s\">\n",
			user_env.script_name,user_ses.sessionid);
   printf("                  Logout</A>\n"
	  "               </TD>\n"
	  "            </TR>\n"
	  "         </TABLE>\n"
	  "      </TD>\n"
          "   </TR>\n"
	  "   <TR>\n"
	  "      <TD COLSPAN=\"2\">\n"
	  "         <BR>\n"
	  "      </TD>\n"
	  "   </TR>\n");

   return(0);
}

///////////////////////////////////////////////////////////////////

int compose_message_form(mail_header* mail_h,
                         FILE* user_mail_file)
{
   status_type status=st_done;

   if(user_env.script_name == NULL || user_ses.sessionid == NULL)
      return(-1);

   printf("   <TR BGCOLOR=\"#DDDDDD\">\n"
          "      <TD VALIGN=\"MIDDLE\" ALIGN=\"RIGHT\">\n"
	  "         <B>From:</B>\n"
	  "      </TD>\n"
	  "      <TD VALIGN=\"MIDDLE\" ALIGN=\"LEFT\">\n");

   if(user_pref.reply_to.name != NULL)
      printf("         %s &lt;%s&gt;\n",
			user_pref.reply_to.name,user_pref.reply_to.email);
   else
      printf("         %s\n",user_pref.reply_to.email);

   printf("         <INPUT TYPE=\"hidden\" NAME=\"from\" VALUE=");

   if(user_pref.reply_to.name != NULL)
      printf("\"&#34;%s&#34; &lt;%s&gt;\">\n",
		user_pref.reply_to.name,user_pref.reply_to.email);
   else
      printf("\"%s\">\n",user_pref.reply_to.email);

   printf("      </TD>\n"
          "   </TR>\n"
	  "   <TR BGCOLOR=\"#DDDDDD\">\n"
	  "      <TD VALIGN=\"MIDDLE\" ALIGN=\"RIGHT\">\n"
	  "         <A HREF=\"javascript:goaddresswindow('to')\">"
	  "<B>To:</B></A>\n"
	  "      </TD>\n"
	  "      <TD VALIGN=\"MIDDLE\" ALIGN=\"LEFT\">\n"
          "         <FONT FACE=\"monospace\">\n"
	  "         <INPUT TYPE=\"text\" NAME=\"to\" SIZE=\"70\"");

  to_line_value(mail_h);
  printf(">\n"
          "         </FONT>\n"
	  "      </TD>\n"
	  "   </TR>\n"
	  "   <TR BGCOLOR=\"#DDDDDD\">\n"
	  "      <TD VALIGN=\"MIDDLE\" ALIGN=\"RIGHT\">\n"
	  "         <A HREF=\"javascript:goaddresswindow('cc')\">"
	  "<B>cc:</B></A>\n"
	  "      </TD>\n"
	  "      <TD VALIGN=\"MIDDLE\" ALIGN=\"LEFT\">\n"
          "         <FONT FACE=\"monospace\">\n"
	  "         <INPUT TYPE=\"text\" NAME=\"cc\" SIZE=\"70\"");

   cc_line_value(mail_h);
   printf(">\n"
          "         </FONT>\n"
	  "      </TD>\n"
	  "   </TR>\n"
	  "   <TR BGCOLOR=\"#DDDDDD\">\n"
	  "      <TD VALIGN=\"MIDDLE\" ALIGN=\"RIGHT\">\n"
	  "         <A HREF=\"javascript:goaddresswindow('bcc')\">"
	  "<B>bcc:</B></A>\n"
	  "      </TD>\n"
	  "      <TD VALIGN=\"MIDDLE\" ALIGN=\"LEFT\">\n"
          "         <FONT FACE=\"monospace\">\n"
	  "         <INPUT TYPE=\"text\" NAME=\"bcc\" SIZE=\"70\">\n"
          "         </FONT>\n"
	  "      </TD>\n"
	  "   </TR>\n"
	  "   <TR BGCOLOR=\"#DDDDDD\">\n"
	  "      <TD VALIGN=\"MIDDLE\" ALIGN=\"RIGHT\">\n"
	  "         <B>Subject:</B>\n"
	  "      </TD>\n"
	  "      <TD VALIGN=\"MIDDLE\" ALIGN=\"LEFT\">\n"
          "         <FONT FACE=\"monospace\">\n"
	  "         <INPUT TYPE=\"text\" NAME=\"subject\" SIZE=\"70\"");

   subject_line_value(mail_h);
   printf(">\n"
          "         </FONT>\n"
	  "      </TD>\n"
	  "   </TR>\n"
	  "   <TR BGCOLOR=\"#EEEEEE\">\n"
	  "      <TD COLSPAN=\"2\">\n"
          "         <FONT FACE=\"monospace\">\n"
	  "         <TEXTAREA NAME=\"body\" ROWS=\"20\" COLS=\"76\" "
	  "WRAP=\"hard\">\n");

   status=body_value(mail_h,user_mail_file);
   printf("</TEXTAREA>\n"
          "         </FONT>\n"
	  "      </TD>\n"
	  "   </TR>\n"
	  "   <TR BGCOLOR=\"#DDDDDD\">\n"
	  "      <TD VALIGN=\"MIDDLE\" COLSPAN=\"2\">\n"
	  "         <B>Attachment:</B>\n"
          "         <FONT FACE=\"monospace\">\n" 
	  "         <INPUT TYPE=\"file\" NAME=\"attachment\" SIZE=\"55\">\n"
          "         </FONT>\n"
	  "      </TD>\n"
	  "   </TR>\n");

   if(status != st_error && status != st_done)
   {
      printf("   <TR BGCOLOR=\"#DDDDDD\">\n"
             "      <TD ALIGN=\"RIGHT\" VALIGN=\"MIDDLE\">\n"
             "         <IMG SRC=\"" NMHTTP "/envelope.gif\" "
             "WIDTH=\"28\" HEIGHT=\"21\">\n"
             "      </TD>\n"
             "      <TD ALIGN=\"LEFT\" VALIGN=\"MIDDLE\">\n"
             "         Attached message.\n"
             "         <INPUT TYPE=\"hidden\" NAME=\"fwmsg\" VALUE=\"%d\">\n"
             "      </TD>\n"
             "   </TR>\n",mail_h->m_num);
   }

   printf("   <TR>\n"
	  "      <TD ALIGN=\"LEFT\" VALIGN=\"TOP\">\n"
	  "         <INPUT TYPE=\"submit\" VALUE=\"Send\">\n"
	  "      </TD>\n"
	  "   </TR>\n");

   printf("</TABLE>\n"
          "</FORM>\n\n");

   printf("<SCRIPT LANGUAGE=\"javascript\">\n"
	  "<!--\n"
	  "   function goaddresswindow(toccbcc)\n"
	  "   {\n"
	  "      var url = \"%s?action=view_addresses&sessionid=%s&field=\" "
	  "+ toccbcc;\n",
		user_env.script_name,user_ses.sessionid);
   printf("      if (toccbcc == \"to\")\n"
	  "         url += \"&preexisting=\" + escape "
	  "(document.composeform.to.value);\n"
	  "      else if (toccbcc == \"cc\")\n"
	  "         url += \"&preexisting=\" + escape "
	  "(document.composeform.cc.value);\n"
	  "      else if (toccbcc == \"bcc\")\n"
	  "         url += \"&preexisting=\" + escape "
	  "(document.composeform.bcc.value);\n\n"
	  "      var hWnd = window.open(url,\"HelpWindow\","
	  "\"width=300,height=360,resizable=yes,scrollbars=yes\");\n"
	  "      if ((document.window != null) && (!hWnd.opener))\n"
	  "         hWnd.opener = document.window;\n"
	  "   }\n\n"
	  "//-->\n"
	  "</SCRIPT>\n");

   return(0);
}

///////////////////////////////////////////////////////////////////

int to_line_value(mail_header* mail_h)
{
   int printed=0;
   addr_t* from=NULL;

   if(mail_h == NULL)
      return(0);

   if(mail_action != reply_message && mail_action != reply_all_message)
      return(0);

   printf(" VALUE=\"");

   if(mail_h->reply_to != NULL)
      from=mail_h->reply_to;
   else if(mail_h->from != NULL)
      from=mail_h->from;


   if(mail_action == reply_all_message)
      find_email_in_book(mail_h->to,user_pref.addr,&(user_pref.reply_to),from);
   else if(mail_action == reply_message)
      find_email_in_book(NULL,user_pref.addr,NULL,from);

   disp_addr(from,&printed);
   disp_addr(user_pref.addr,&printed);

   if(mail_action == reply_all_message)
      disp_addr(mail_h->to,&printed);


   printf("\"");

   return(0);

}

///////////////////////////////////////////////////////////////////

int cc_line_value(mail_header* mail_h)
{
   int printed=0;

   if(mail_h == NULL)
      return(0);

   if(mail_action != reply_all_message)
      return(0);

   if(mail_h->cc != NULL)
   {
      find_email_in_book(mail_h->cc,user_pref.addr,&(user_pref.reply_to),NULL);

      printf(" VALUE=\"");

      disp_addr(user_pref.addr,&printed);
      disp_addr(mail_h->cc,&printed);

      printf("\"");
   }

  return(0);
}

///////////////////////////////////////////////////////////////////
// 
// Addr status & 0x00 == display full
//      status & 0x01 == hide email if name != NULL
//      status & 0x02 == dont display anything

int disp_addr(addr_t* addr, int* printed)
{
   int x=0;

   if(addr == NULL)
      return(0);

   for(x=0; addr[x].name || addr[x].email; x++)
   {
      if( ((addr[x].status)&0x02) == 0)
      {
         if(printed != NULL)
         {
            if(*printed)
               printf(", ");
            else
               *printed=-1;
         }

         if(addr[x].name != NULL)
         {
            if( strchr(addr[x].name,',') != NULL )
               printf("&#34;%s&#34;",addr[x].name);
            else
               printf("%s",addr[x].name);

            if( (((addr[x].status)&0x01)==0) && (addr[x].email != NULL) )
               printf(" &lt;%s&gt",addr[x].email);
         }
         else
         {
            if(addr[x].email != NULL)
               printf("%s",addr[x].email);
         }
      }
   }

   return(0);
}

///////////////////////////////////////////////////////////////////

int subject_line_value(mail_header* mail_h)
{
   if(mail_h == NULL)
      return(0);

   if(mail_h->subject == NULL)
      return(0);

   if(mail_action != reply_message &&
          mail_action != reply_all_message &&
          mail_action != forward_message )
      return(0);

   printf(" VALUE=\"");

   if(mail_action == reply_message || mail_action == reply_all_message)
   {
      if( strncasecmp(mail_h->subject,"Re:",sizeof("Re:")-1) != 0)
         printf("Re: ");
   }
   else if(mail_action == forward_message)
   {
      if( strncasecmp(mail_h->subject,"Fw:",sizeof("Fw:")-1) != 0)
         printf("Fw: ");
   }

   printf("%s\"",mail_h->subject);

   return(0);
}

///////////////////////////////////////////////////////////////////

status_type body_value(mail_header* mail_h, FILE* user_mail_file)
{
   struct tm* time_data=NULL;
   char time_string[50];
   int x=0;
   int printed=0;
   status_type status=st_done;

   if(mail_h == NULL)
      return(st_done);

   if(mail_action != reply_message &&
      mail_action != reply_all_message &&
      mail_action != forward_message)
      return(st_done);

   for(x=0;x<50;x++)
      time_string[x]='\0';

   if(mail_h->from != NULL)
      mail_h->from[0].status=0;

   if(mail_h->to != NULL)
      for(x=0; mail_h->to[x].name || mail_h->to[x].email; x++)
         mail_h->to[x].status=0;

   if(mail_h->cc != NULL)
      for(x=0; mail_h->cc[x].name || mail_h->cc[x].email; x++)
         mail_h->cc[x].status=0;

   printf("\n\n\n"
          "----- Original Message -----\n");
   if(mail_h->date_received != 0)
   {
      time_data=localtime(&(mail_h->date_received));
      strftime(time_string,50,"%a  %B %d, %Y  %I:%M %p",time_data);
      printf("Date: %s\n",time_string);
   }
   
   if(mail_h->from != NULL)
   {
      printf("From: ");   
      disp_addr(mail_h->from,&printed);
      printf("\n");
   }

   printed=0;

   if(mail_h->to != NULL)
   {
      printf("To: ");
      disp_addr(mail_h->to,&printed);
      printf("\n");
   }

   printed=0;

   if(mail_h->cc != NULL)
   {
      printf("cc: ");
      disp_addr(mail_h->cc,&printed);
      printf("\n");
   }

   if(mail_h->subject != NULL)
      printf("Subject: %s\n",mail_h->subject);

   printf("\n");

   status=refw_body_send(user_mail_file,mail_h);

   return(status);
}

///////////////////////////////////////////////////////////////////

status_type refw_body_send(FILE* user_mail_file, mail_header* mail_h)
{
   mail_header* next_head_multi=NULL;
   mail_header* next_head_alt=NULL;

   status_type status=st_pending;

   if(user_mail_file == NULL || mail_h == NULL)
      return(st_error);

   if(mail_h->cont_type == ct_text_plain || mail_h->cont_type == ct_none)
   {
      send_body(user_mail_file,
                NULL,
                mail_h->cont_type_enc);
      return(st_done);
   }
   else if(mail_h->cont_type == ct_multipart_alternative)
   {
      status=find_next_border(user_mail_file,mail_h->cont_boundary);

      if(status == st_error || status == st_done)
         return(status);

      next_head_alt=fill_mail_h_struct(user_mail_file,0);

      if(next_head_alt == NULL)
         return(st_error);

      if(next_head_alt->cont_type == ct_text_plain)
      {
         send_body(user_mail_file,
                   mail_h->cont_boundary,
                   next_head_alt->cont_type_enc);

         free_mail_headers(next_head_alt);
         return(st_done);
      }
      else
      {
         free_mail_headers(next_head_alt);
         return(st_pending);
      }
   }
   else if(mail_h->cont_type == ct_multipart_mixed)
   {
      status=find_next_border(user_mail_file,mail_h->cont_boundary);

      if(status == st_error || status == st_done)
         return(status);

      next_head_multi=fill_mail_h_struct(user_mail_file,0);

      if(next_head_multi == NULL)
         return(st_error);

      if(next_head_multi->cont_type == ct_text_plain || 
			next_head_multi->cont_type == ct_none)
      {
         status=send_body(user_mail_file,
                   mail_h->cont_boundary,
                   next_head_multi->cont_type_enc);

         free_mail_headers(next_head_multi);
         return(status);
      }
      else if(next_head_multi->cont_type == ct_multipart_alternative)
      {
         status=find_next_border(user_mail_file,
                                 next_head_multi->cont_boundary);

         if(status == st_error || status == st_done)
         {
            free_mail_headers(next_head_multi);

            if(status != st_error)
               status=find_next_border(user_mail_file,mail_h->cont_boundary);

            return(status);
         }

         next_head_alt=fill_mail_h_struct(user_mail_file,0);

         if(next_head_alt == NULL)
         {
            free_mail_headers(next_head_multi);
            return(st_error);
         }

         if(next_head_alt->cont_type == ct_text_plain || 
			next_head_alt->cont_type == ct_none)
         {
            status=send_body(user_mail_file,
                      next_head_multi->cont_boundary,
                      next_head_alt->cont_type_enc);

            status=find_next_border(user_mail_file,mail_h->cont_boundary);

            free_mail_headers(next_head_alt);
            free_mail_headers(next_head_multi);

            return(status);
         }
         else
         {
            free_mail_headers(next_head_multi);
            free_mail_headers(next_head_alt);
            return(st_pending);
         }
      }
      else
      {
         free_mail_headers(next_head_multi);
         return(st_pending);
      }
   }
   else
   {
     return(st_pending);
   }

   return(st_error);
}

///////////////////////////////////////////////////////////////////

status_type send_body(FILE* user_mail_file, 
                      char* boundary, 
                      content_type_encoding encoding)
{
   char* file_buff=NULL;
   size_t file_buff_size=0;
   ssize_t b_read=0;

   status_type status=st_pending;

   do
   {
      b_read=getline(&file_buff,&file_buff_size,user_mail_file);

      if(b_read <= 0 || file_buff == NULL)
      {
         if(file_buff != NULL)
            free(file_buff);
         status=st_error;
      }

      else if(status == st_nl)
         status=is_boundary(file_buff,boundary);

      if(status == st_pending)
      {
         if(encoding == cte_quoted_printable)
            fix_quoted_to_text(file_buff);

         text_to_html(file_buff);

         if(*file_buff == '\n')
            status=st_nl;
      }
   }
   while(status == st_pending || status == st_nl);


   if(file_buff != NULL)
      free(file_buff);

   return(status);
}

///////////////////////////////////////////////////////////////////
// This will compare the reply_to emails to address book. If found
//    in address book, will remove from real and set address book
//    to display name only
// 
// Addr status & 0x00 == display full
//      status & 0x01 == hide email if name != NULL
//      status & 0x02 == dont display anything


int find_email_in_book(addr_t* real, addr_t* book, addr_t* skip, addr_t* from)
{
   int x=0;
   int y=0;

   if(real != NULL)
      for(x=0; real[x].name || real[x].email; x++)
         real[x].status=0x00;

   if(book != NULL)
      for(y=0; book[y].name || book[y].email; y++)
         book[y].status=0x03;

   if(real != NULL && book != NULL)
   {
      for(x=0; real[x].name || real[x].email; x++)
      {
         for(y=0; book[y].name || book[y].email; y++)
         {
            if(real[x].email != NULL && book[y].email != NULL)
               if(strcasecmp(real[x].email,book[y].email) == 0)
               {
                  real[x].status=0x02;
                  book[y].status=0x01;
               }
         }
      }
   }

   if(skip != NULL && skip->email != NULL)
   {
      if(real != NULL)
      {
         for(x=0; real[x].name || real[x].email; x++)
            if(real[x].email != NULL)
               if(strcasecmp(real[x].email,skip->email) == 0)
                  real[x].status=0x02;
      }

      if(book != NULL)
      {
         for(y=0; book[y].name || book[y].email; y++)
            if(book[y].email != NULL)
               if(strcasecmp(book[y].email,skip->email) == 0)
                  book[y].status=0x02;
      }
   }

   if(from != NULL && from[0].email != NULL )
   {
      if(real != NULL)
      {
         for(x=0; real[x].name || real[x].email; x++)
            if(real[x].email != NULL)
               if(strcasecmp(real[x].email,from[0].email) == 0)
                  real[x].status=0x02;
      }

      if(book != NULL)
      {
         for(y=0; book[y].name || book[y].email; y++)
            if(book[x].email != NULL)
               if(strcasecmp(book[y].email,from[0].email) == 0)
               {
                  from[0].status=0x02;
                  book[y].status=0x01;
               }
      }
   }

   return(0);
}

///////////////////////////////////////////////////////////////////


