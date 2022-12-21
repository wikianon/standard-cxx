/*
	view_message.cpp
*/

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <time.h>

#include "environment.h"
#include "parse_form.h"
#include "main.h"
#include "err_page.h"
#include "parse_mail_headers.h"
#include "mail_process.h"
#include "headers.h"
#include "view_message.h"
#include "encoding_conv.h"
#include "config.h"

void view_message_header(struct mail_header*);
status_type view_message_body(FILE*, struct mail_header*,int,int*,char*);
void view_message_body_head(struct mail_header*);

status_type view_body_ct_text_plain(FILE*, mail_header*, char*);
status_type view_body_ct_multipart(FILE*, mail_header*, int, int*, char*);
status_type view_body_ct_multipart_alt(FILE*, mail_header*, int, int*, char*);
status_type view_body_ct_message_rfc822(FILE*, mail_header*, int, int*, char*);
status_type view_body_ct_attachment(FILE*,mail_header*,int,int*, char*);
status_type view_body_ct_image(FILE*, mail_header*, int, int*, char*);

/////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

int view_message_action(void)
{
   FILE* user_mail_file=NULL;
   long int m_num=0;
   cgi_t* cgi_data=NULL;
   int err=0;
   struct mail_header* current_header=NULL;
   int part_num=1;

   while(get_cgi_data(&cgi_data) == 0 )
   {
      if(strcmp(cgi_data->name,"m_num") == 0 && cgi_data->value != NULL)
      {
         m_num=strtol(cgi_data->value,NULL,10);

         free(cgi_data->name);
         cgi_data->name=NULL;
         free(cgi_data->value);
         cgi_data->value=NULL;
      }
   }

   if(m_num <= 0)
   {
      syslog(LOG_MAIL | LOG_ERR,"Cannot view message <= 0");
      err_page();
      return(-1);
   }
 
   setgid(egid);
   user_mail_file=open_user_mailfile("r");
   setgid(rgid);

   if(user_mail_file == NULL)
   {
      err_string_page(
		"Page has expired. Go back, hit refresh, and try again.");
      return(-1);
   }

   err=point_to_message(user_mail_file,m_num);

   if(err != 0)
   {
      err_string_page(
		"Page has expired. Go back, hit refresh, and try again.");
      return(-1);
   }

   current_header=fill_mail_h_struct(user_mail_file,m_num);

   if(current_header == NULL)
   {
      err_page();
      return(-1);
   }

   if(current_header->mail_status == m_new)
   {
      fclose(user_mail_file);

      setgid(egid);
      user_mail_file=open_user_mailfile("r+");
      setgid(rgid);

      if(user_mail_file == NULL)
      {
         err_page();
         return(-1);
      }

      err=point_to_message(user_mail_file,m_num);

      if(err == 0)
         update_status_read(user_mail_file,m_num);
      else
         fclose(user_mail_file);

      setgid(egid);
      user_mail_file=open_user_mailfile("r");
      setgid(rgid);

      if(user_mail_file == NULL)
         return(-1);

      err=point_to_message(user_mail_file,m_num);

      if(err != 0)
      {
         fclose(user_mail_file);
         return(-1);
      }

      free_mail_headers(current_header);

      current_header=fill_mail_h_struct(user_mail_file,m_num);
   }

   if(current_header->cont_type == ct_none)
      current_header->cont_type=ct_text_plain;

   content_html();
   html_header("View Message");

   printf("<TABLE BORDER=\"0\" ALIGN=\"CENTER\" WIDTH=\"75%%\" "
		"CELLPADDING=\"1\" CELLSPACING=\"1\">\n");

   view_message_header(current_header);
   view_message_body(user_mail_file,current_header,m_num,&part_num,NULL);

   fclose(user_mail_file);
   free_mail_headers(current_header);

   printf("\n</TABLE>\n");
   html_footer();

   return(0);
}

/////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

void view_message_header(struct mail_header* current_header)
{

//TOP LINE

   printf("<TR BGCOLOR=\"#002266\"><TD>");
   printf("<TABLE WIDTH=\"100%%\" CELLPADDING=\"0\" CELLSPACING=\"0\">");
   printf("<TR><TD WIDTH=\"50%%\" ALIGN=\"LEFT\">\n");
   printf("<FONT COLOR=\"#FFFFFF\" FACE=\"Arial, Helvetica\" SIZE=\"3\">\n");
   printf("<B>View Message</B></FONT></TD>\n");
   printf("<TD WIDTH=\"50%%\" ALIGN=\"RIGHT\">");
   printf("<FONT COLOR=\"#FFFFFF\" FACE=\"Arial, Helvetica\" SIZE=\"3\">\n");
   printf("<B>Message number: %d</B></FONT>",current_header->m_num);
   printf("</TD></TR></TABLE></TD></TR>\n\n");

//SECOND LINE
   
   printf("<TR BGCOLOR=\"#DCDCDC\"><TD>");
   printf("<TABLE BORDER=\"0\" WIDTH=\"100%%\">\n");
   printf("<TR><TD>");

   printf("<FONT FACE=\"Arial, Heveltica\" SIZE=\"2\">\n");

   printf("<A HREF=\"%s?action=view_inbox&sessionid=%s\">",
	user_env.script_name,
	user_ses.sessionid);
   printf("Inbox</A> | \n");

   printf("<A HREF=\"%s?action=compose_message&sessionid=%s\">",
	user_env.script_name,
	user_ses.sessionid);
   printf("Compose</A> | \n");

   printf("<A HREF=\"%s?action=reply_message&sessionid=%s&m_num=%d\">",
	user_env.script_name,
	user_ses.sessionid,
	current_header->m_num);
   printf("Reply</A> | \n");

   printf("<A HREF=\"%s?action=reply_all_message&sessionid=%s&m_num=%d\">",
	user_env.script_name,
	user_ses.sessionid,
	current_header->m_num);
   printf("Reply All</A> | \n");

   printf("<A HREF=\"%s?action=forward_message&sessionid=%s&m_num=%d\">",
	user_env.script_name,
	user_ses.sessionid,
	current_header->m_num);
   printf("Forward</A> | \n");

   printf("<A HREF=\"%s?action=delete_message&sessionid=%s&%d=%ld\">",
	user_env.script_name,
	user_ses.sessionid,
	current_header->m_num,
	current_header->date_received);
   printf("Delete</A> | \n");

   printf("<A HREF=\"%s?action=logout&sessionid=%s\">",
	user_env.script_name,
	user_ses.sessionid);
   printf("Logout</A>\n");

   printf("</FONT></TD></TR>\n\n");

   printf("</TABLE></TD></TR>");

   printf("<TR><TD><BR> </TD></TR>\n");

}

/////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

status_type view_message_body(FILE* user_file, 
			struct mail_header* current_header,
			int m_num,
			int* part_num,
			char* boundary)
{
   status_type status=st_pending;

   if(user_file == NULL || current_header == NULL)
      return(st_error);			//sanity check


   if(current_header->date_received != 0 || 
		current_header->subject != NULL ||
		current_header->from != NULL ||
		current_header->cc != NULL ||
		current_header->to != NULL)
      view_message_body_head(current_header);

   if(  current_header->cont_type == ct_multipart_mixed ||
	current_header->cont_type == ct_multipart_parallel ||
	current_header->cont_type == ct_multipart_digest ||
	current_header->cont_type == ct_multipart_other) 
      return(view_body_ct_multipart(user_file,current_header,
				m_num,part_num,boundary)); 

   if(current_header->cont_type == ct_multipart_alternative)
      return(view_body_ct_multipart_alt(user_file,current_header,
				m_num,part_num,boundary)); 

   if((current_header->cont_type == ct_text_plain ||
		current_header->cont_type == ct_text_other) &&
		current_header->cont_disposition != ctd_attachment)
      return(view_body_ct_text_plain(user_file, current_header, boundary));

   if(current_header->cont_type == ct_image_jpeg ||
		current_header->cont_type == ct_image_gif)
      return(view_body_ct_image(user_file, current_header, 
			m_num, part_num, boundary));

   if(current_header->cont_type == ct_message_rfc822)
      return(view_body_ct_message_rfc822(user_file, current_header, 
			m_num, part_num, boundary));

   status=view_body_ct_attachment(user_file,current_header, 
			 m_num, part_num, boundary);

   return(status);
}


/////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

void view_message_body_head(struct mail_header* current_header)
{
   struct tm* time_data=NULL;
   char time_string[100];
   int x=0;

   for(x=0;x<100;x++)
       time_string[x]=0;

   if(current_header != NULL)
   {
      printf("<TR><TD BGCOLOR=\"#DDDDDD\">\n");

      printf("<TABLE CELLPADDING=\"0\" CELLSPACING=\"0\">\n");

      if(current_header->date_received != 0)
      {
         time_data=localtime(&(current_header->date_received));
         strftime(time_string,100,
		"%A,&nbsp;&nbsp;%B %d, %Y&nbsp;&nbsp;%I:%M %p",
							time_data);

         printf("<TR><TD WIDTH=\"70\" VALIGN=\"TOP\"><B>Date:</B> </TD> ");
         printf("<TD>%s</TD></TR>\n",time_string);
      }

      if(current_header->from != NULL)
      {
         printf("<TR><TD VALIGN=\"TOP\"><B>From:</B> </TD><TD>");

         if(current_header->from[0].name != NULL)
         {
            printf("%s",current_header->from[0].name);

            if(current_header->from[0].email != NULL)
               printf(" &lt;%s&gt",current_header->from[0].email);
         }
         else if(current_header->from[0].email != NULL)
            printf("%s",current_header->from[0].email);

         printf("</TD></TR>\n");
      }

      if(current_header->to != NULL)
      {
         printf("<TR><TD VALIGN=\"TOP\"><B>To:</B> </TD><TD>");

         for(x=0; current_header->to[x].name || current_header->to[x].email; 
						x++)
         {
            if(x != 0)
               printf(", ");

            if(current_header->to[x].name != NULL)
            {
               printf("%s",current_header->to[x].name);

               if(current_header->to[x].email != NULL)
                  printf(" &lt;%s&gt",current_header->to[x].email);
            }
            else if(current_header->to[x].email != NULL)
               printf("%s",current_header->to[x].email);
         }

         printf("</TD></TR>\n");
      }

      if(current_header->cc != NULL)
      {
         printf("<TR><TD VALIGN=\"TOP\"><B>cc:</B> </TD><TD>");

         for(x=0; current_header->cc[x].name || current_header->cc[x].email; 
						x++)
         {
            if(x != 0)
               printf(", ");

            if(current_header->cc[x].name != NULL)
            {
               printf("%s",current_header->cc[x].name);

               if(current_header->cc[x].email != NULL)
                  printf(" &lt;%s&gt",current_header->cc[x].email);
            }
            else if(current_header->cc[x].email != NULL)
               printf("%s",current_header->cc[x].email);
         }

         printf("</TD></TR>\n");
      }

      if(current_header->subject != NULL)
      {
         printf("<TR><TD VALIGN=\"TOP\"><B>Subject: </B> </TD>");
         printf("<TD>%s</TD></TR>\n",current_header->subject);
      }

      printf("</TABLE>");
      printf("\n</TD></TR>\n\n");
   }
}

/////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

status_type view_body_ct_multipart(FILE* user_file, 
				struct mail_header* current_mh, 
				int m_num,
				int* part_num, 
				char* boundary)
{
   status_type status=st_pending;
   struct mail_header* next_header=NULL;


   if(current_mh == NULL || current_mh->cont_boundary == NULL)
      return(st_error);

   status=find_next_border(user_file, current_mh->cont_boundary);

   if(status == st_error || status == st_done)
      return(status);

   do
   {
      if(status != st_error && status != st_done)
         next_header=fill_mail_h_struct(user_file,(*part_num)++);

      if(next_header != NULL)
      {
         if(next_header->cont_type == ct_none)
         {
            if(current_mh->cont_type == ct_multipart_digest)
               next_header->cont_type=ct_message_rfc822;
            else
               next_header->cont_type=ct_text_plain;
         }

         status=view_message_body(user_file, next_header, m_num, part_num,
			current_mh->cont_boundary);
      }
  
      if(next_header != NULL)
         free_mail_headers(next_header);
   }
   while(status == st_pending || status == st_found);

   if(status != st_error && boundary != NULL)
      return(find_next_border(user_file,boundary));

   return(status);
}

/////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

status_type view_body_ct_multipart_alt(FILE* user_file, 
				struct mail_header* current_mh,
				int m_num,
				int* part_num, 
				char* boundary)
{
   struct cont_alternative { 
	struct mail_header* next_mh;
	long int file_ptr; 
   };

   status_type status=st_pending;
   cont_alternative* cont_alt=NULL;
   int x=0;
   int y=0;
   int best_part=0;
   void* temp_ptr=NULL;

   if(current_mh == NULL)
      return(st_error);			//sanity check

   status=find_next_border(user_file, current_mh->cont_boundary);

   if(status == st_error || status == st_done)
   { 
      if(boundary != NULL)
         return(find_next_border(user_file, boundary)); 

      return(st_error);
   }

//find different types of alternate multiparts and save headers in structure

   do
   {
      x++;
      temp_ptr=realloc(cont_alt,sizeof(cont_alternative)*x);
  
      if(temp_ptr == NULL)
      {
         if(cont_alt != NULL)
         {
            for(y=0;y<x-1;y++)
               if(cont_alt[y].next_mh != NULL)
                  free_mail_headers(cont_alt[y].next_mh);

            free(cont_alt);
         }
      
         return(st_error);
      }

      cont_alt=(cont_alternative*)temp_ptr;

      cont_alt[x-1].next_mh=fill_mail_h_struct(user_file,(*part_num)++);

      if(cont_alt[x-1].next_mh == NULL)
         status=st_error;

      cont_alt[x-1].file_ptr=ftell(user_file);
      status=find_next_border(user_file,current_mh->cont_boundary);
   }
   while(status== st_found || status==st_pending);

//find best part

   for(y=x-1;y<0;y--)
   {
      if(cont_alt[y].next_mh != NULL)
         if(cont_alt[y].next_mh->cont_type == ct_text_plain)
            best_part=y;
   }

//display best part

   if(cont_alt[best_part].file_ptr != -1)
      fseek(user_file,cont_alt[best_part].file_ptr,SEEK_SET);

   status=view_message_body(user_file,cont_alt[best_part].next_mh,
		m_num, part_num-x+best_part, current_mh->cont_boundary);


//clean up and exit

   for(y=0;y<x;y++)
      if(cont_alt[y].next_mh != NULL)
         free_mail_headers(cont_alt[y].next_mh);
      
   free(cont_alt);

   if(status != st_error && boundary != NULL)
         return(find_next_border(user_file, boundary)); 

   return(st_done);
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

status_type view_body_ct_message_rfc822(FILE* user_file,
				mail_header* current_header, 
				int m_num, 
				int* part_num, 
				char* boundary)
{
   mail_header* next_header=NULL;
   status_type status=st_pending;

   next_header=fill_mail_h_struct(user_file,(*part_num)++);

   if(next_header == NULL)
      return(st_error);

   status=view_message_body(user_file,
			next_header,m_num,part_num,boundary);

   free_mail_headers(next_header);

   return(status);
}

/////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

status_type view_body_ct_text_plain(FILE* user_file, 
				mail_header* mail_hd,
				char* boundary)
{
   char* file_buff=NULL;
   size_t file_buff_size=0;
   ssize_t b_read=0;
   int nl=0;

   status_type status=st_pending;

   printf("\n\n<TR><TD BGCOLOR=\"#EEEEEE\">");

   do
   {
      b_read=getline(&file_buff,&file_buff_size,user_file);

      if(b_read <= 0 || file_buff == NULL)
      {
         if(file_buff != NULL)
         {
            free(file_buff);
            file_buff=NULL;
         }
         status=st_error;
      }
 
      status=is_boundary(file_buff,boundary);

      if(status == st_pending || status == st_nl)
      {
         if(mail_hd != NULL && mail_hd->cont_type_enc == cte_quoted_printable)
            fix_quoted_to_text(file_buff);

         if(strchr(file_buff,'\n') != NULL)
            nl=-1;
         else
            nl=0;

         text_to_html(file_buff);

         if(nl)
            printf("<BR>");
      }
   }
   while(status == st_pending || status == st_nl);

   if(file_buff != NULL)
      free(file_buff);

   printf("\n</TD></TR>");

   return(status);
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////


status_type view_body_ct_image(FILE* user_file, 
				mail_header* current_header, 
				int m_num, 
				int* part_num, 
				char* boundary)
{

   if(user_file == NULL || current_header == NULL)
      return(st_error);


   printf("\n\n<TR><TD BGCOLOR=\"#EEEEEE\" ALIGN=\"CENTER\"><BR>");

   printf("\n<A HREF=\"%s/%s?action=view_attachment&sessionid=%s&m_num=%d&"
		"part_num=%d&disp=attachment\">\n",
		user_env.script_name,
		current_header->filename != NULL ?
			current_header->filename : "unknown",
		user_ses.sessionid,
		m_num,
		*part_num);
   printf("<IMG SRC=\"%s/%s?action=view_attachment&sessionid=%s&m_num=%d&"
		"part_num=%d&disp=inline\" BORDER=\"0\">\n</A>",
		user_env.script_name,
		current_header->filename != NULL ?
			current_header->filename : "unknown",
		user_ses.sessionid,
		m_num,
		*part_num);

   printf("\n<BR><BR></TD></TR>");

   if(boundary != NULL)
      return(find_next_border(user_file,boundary));

   return(st_error);

}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////


status_type view_body_ct_attachment(FILE* user_file, 
			struct mail_header* c_hd, 
			int m_num,
			int* part_num, 
			char* boundary)
{
   if(user_file == NULL || c_hd == NULL)
      return(st_error);

   printf("\n\n<TR><TD BGCOLOR=\"#EEEEEE\">");

   printf("\n<TABLE CELLSPACING=\"10\"><TR>\n<TD>");

   printf("\n<A HREF=\"%s/%s?action=view_attachment&sessionid=%s&m_num=%d&"
		"part_num=%d&disp=",
		user_env.script_name,
		c_hd->filename != NULL ? c_hd->filename : "unknown",
		user_ses.sessionid,
		m_num,
		*part_num);

   if(c_hd->cont_type == ct_application_pdf ||
      c_hd->cont_type == ct_application_zip ||
      c_hd->cont_type == ct_application_msword ||
      c_hd->cont_type == ct_application_msexcel ||
      c_hd->cont_type == ct_audio_other ||
      c_hd->cont_type == ct_video_other ||
      c_hd->cont_type == ct_text_plain ||
      c_hd->cont_type == ct_text_other ||
      c_hd->cont_type == ct_text_html ||
      c_hd->cont_type == ct_image_jpeg ||
      c_hd->cont_type == ct_image_gif ||
      c_hd->cont_type == ct_image_png ||
      c_hd->cont_type == ct_image_other)
   {
      printf("inline\">\n");
   }
   else
      printf("attachment\">\n");

//Display icon

   printf("\n   <IMG SRC=\"" NMHTTP "/icons/");

   if(c_hd->cont_type == ct_application_pdf)
      printf("acroread.gif");
   else if(c_hd->cont_type == ct_application_zip)
      printf("zip.gif");
   else if(c_hd->cont_type == ct_application_msword)
      printf("word.gif");
   else if(c_hd->cont_type == ct_application_msexcel)
      printf("excel.gif");
   else if(c_hd->cont_type == ct_application_other)
      printf("application.gif");
   else if(c_hd->cont_type == ct_audio_other)
      printf("audio.gif");
   else if(c_hd->cont_type == ct_video_other)
      printf("video.gif");
   else if(c_hd->cont_type == ct_image_other ||
		c_hd->cont_type == ct_image_jpeg ||
		c_hd->cont_type == ct_image_gif ||
		c_hd->cont_type == ct_image_png)
      printf("image.gif");
   else if(c_hd->cont_type == ct_text_plain ||
		c_hd->cont_type == ct_text_html ||
		c_hd->cont_type == ct_text_other)
      printf("text.gif");
   else
      printf("unknown.gif");

   printf("\" BORDER=\"0\"></A>\n");

//Print content type / filename

   printf("</TD>\n<TD>\nType: %s<BR>",
	c_hd->cont_type_string == NULL ? "uknown" : c_hd->cont_type_string);
   printf("Filename: \n");
   printf("   <A HREF=\"%s/%s?action=view_attachment&sessionid=%s&m_num=%d&"
		"part_num=%d&disp=attachment\">",
		user_env.script_name,
		c_hd->filename != NULL ? c_hd->filename : "unknown",
		user_ses.sessionid,
		m_num,
		*part_num);

   printf("%s</A>\n</TD></TR>\n</TABLE>",
        c_hd->filename == NULL ? "unknown" : c_hd->filename);


   printf("\n</TD></TR>");

   if(boundary != NULL)
      return(find_next_border(user_file,boundary));

   return(st_error);
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
//Will find next content border or next message
//   returns st_error if next message found "\nFrom "
//   returns st_found if boundary found
//   returns st_done if last boundary found

status_type find_next_border(FILE* user_file, char* boundary)
{
   char* file_buff=NULL;
   size_t file_buff_size=0;
   ssize_t b_read=0;
   status_type status=st_pending;

   if(user_file == NULL)
      return(st_error);

   status=st_pending;

   do
   {      
      b_read=getline(&file_buff,&file_buff_size,user_file);

      if(b_read < 0 || file_buff == NULL)
         status=st_error;

      status=is_boundary(file_buff,boundary);
   }
   while(status == st_pending || status == st_nl);

   if(file_buff != NULL)
      free(file_buff);

   return(status);
}


/////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
//Checks if line in buff is boundary
//   returns st_error if new message is found "From "
//   returns st_found if boundary found
//   returns st_done if end boundary found
//   returns st_pending if no border found
//   returns st_nl if nl found

status_type is_boundary(char* buff, char* boundary)
{

//enum status_type { st_pending, st_nl, st_done, st_error, st_found};

   if(buff == NULL)
      return(st_error);

   if(*buff == '\n')
      return(st_nl);

   if(strncmp(buff,"From ",sizeof("From ")-1) == 0)
      return(st_error);

   if(boundary == NULL)
      return(st_pending);

   if(strlen(buff) >= 3 && strncmp(buff,"--",2) == 0)
   {
      if(strncmp(buff+2, boundary, strlen(boundary)) == 0)
      {
         if( strlen(buff) >= 4+strlen(boundary) &&
		strncmp(buff+2+strlen(boundary),"--",2) == 0)
            return(st_done);
         else
            return(st_found);
      }
   }
     
   return(st_pending); 
}


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
