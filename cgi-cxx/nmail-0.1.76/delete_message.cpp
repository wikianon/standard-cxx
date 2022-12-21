/*
	delete_message.cpp
*/

#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <errno.h>
#include <string.h>

#include "mail_process.h"
#include "environment.h"
#include "parse_form.h"
#include "main.h"
#include "inbox_page.h"
#include "parse_mail_headers.h"
#include "err_page.h"

////////////////////////////////////////////////////////////////////////

struct message_id_s {
   long int m_num;
   long int m_date;
};

int get_msgs_to_delete(message_id_s**);
void sort_msg_ids(struct message_id_s*, int);
int delete_msg_from_file(FILE*, message_id_s);



////////////////////////////////////////////////////////////////////////

int delete_msg(void)
{
   int x=0;
   message_id_s* msg_id=NULL;
   int msg_id_size=0;
   FILE* user_mail_file=NULL;

   msg_id_size=get_msgs_to_delete(&msg_id);

   if(msg_id_size > 0)
      sort_msg_ids(msg_id,msg_id_size);

   if(msg_id_size > 0 && msg_id != NULL)
   {
      setgid(egid);
      user_mail_file=open_user_mailfile("r+");
      setgid(rgid);

      if(user_mail_file == NULL)
      {
         syslog(LOG_MAIL | LOG_ERR,"could not open %s mailfile",
			user_ses.loginname);
         err_page();
         return(-1);
      }

      for(x=0;x<msg_id_size;x++)
      {
         if(delete_msg_from_file(user_mail_file,msg_id[x]) != 0)
         {
            syslog(LOG_MAIL | LOG_ERR,"error deleting message for user %s",
				user_ses.loginname);
            fclose(user_mail_file);
            err_page();
            return(-1);
         }
      }

      fclose(user_mail_file);
   }

   if(msg_id != NULL)
      free(msg_id);

   inbox_action();

   return(0);
}

////////////////////////////////////////////////////////////////////////
//Fills message_id structure, returns number of array elements

int get_msgs_to_delete(message_id_s** msg_id)
{
   int msg_id_size=0;
   cgi_t* cgi_data=NULL;
   
   while(get_cgi_data(&cgi_data) == 0)
   {
      msg_id_size++;

      *msg_id=(struct message_id_s*) realloc(*msg_id,
			(size_t) ( sizeof(struct message_id_s) 
			* msg_id_size ) );

      if(*msg_id != NULL && cgi_data->name != NULL && cgi_data->value != NULL)
      {
         (*msg_id)[msg_id_size-1].m_num=strtol(cgi_data->name,NULL,10);
         (*msg_id)[msg_id_size-1].m_date=strtol(cgi_data->value,NULL,10);

         free(cgi_data->name);
         cgi_data->name=NULL;
         free(cgi_data->value);
         cgi_data->value=NULL;
      }
   }

   return(msg_id_size);
}

////////////////////////////////////////////////////////////////////////

void sort_msg_ids(struct message_id_s* msg_id,int msg_id_size)
{
   message_id_s temp_msg_id;
   int x;
   int y;


   if(msg_id_size > 1)
   {
      for(x=0;x<msg_id_size-1;x++)
      {
         for(y=x+1;y<msg_id_size;y++)
         {
            if( msg_id[x].m_num < msg_id[y].m_num )
            {
               temp_msg_id.m_num=msg_id[x].m_num;
               temp_msg_id.m_date=msg_id[x].m_date;
               msg_id[x].m_num=msg_id[y].m_num;
               msg_id[x].m_date=msg_id[y].m_date;
               msg_id[y].m_num=temp_msg_id.m_num;
               msg_id[y].m_date=temp_msg_id.m_date;
            }
         }
      }
   }
}

////////////////////////////////////////////////////////////////////////
//This will delete message from file

int delete_msg_from_file(FILE* user_mail_file, message_id_s msg_id)
{
   long int file_ptr_read=0;
   long int file_ptr_write=0;
   struct mail_header* current_mail_header=NULL;

   if(user_mail_file == NULL)
      return(-1);

   if(msg_id.m_num <= 0)
   {
      syslog(LOG_MAIL | LOG_ERR,"delete message failed. "
			"Tried to delete message <= 0 ");
      return(-1);
   }

   if( point_to_message(user_mail_file, msg_id.m_num) != 0)
   {
      syslog(LOG_MAIL | LOG_ERR,"Could not find message to delete");
      return(-1);
   }

   if( (file_ptr_write=ftell(user_mail_file)) == -1)
   {
      syslog(LOG_MAIL | LOG_ERR,"ftell failed (%s)",strerror(errno));
      return(-1);
   }

   current_mail_header=fill_mail_h_struct(user_mail_file,msg_id.m_num);

   if(current_mail_header == NULL)
   {
      syslog(LOG_MAIL | LOG_ERR,"could not read message header");
      return(-1);
   }

   if(current_mail_header->date_received != msg_id.m_date)
   {
      syslog(LOG_MAIL | LOG_ERR,"delete message failed because of "
			"invalid message id");
      free_mail_headers(current_mail_header);
      return(-1);
   }

   free_mail_headers(current_mail_header);

   point_to_next_message(user_mail_file,NULL);

   if( (file_ptr_read=ftell(user_mail_file)) == -1)
   {
      syslog(LOG_MAIL | LOG_ERR,"ftell failed (%s)",strerror(errno));
      return(-1);
   }

   if( insert_line_in_file(user_mail_file,NULL,file_ptr_read,file_ptr_write)
		!= 0)
   {
      syslog(LOG_MAIL | LOG_ERR,"error deleting message");
      return(-1);
   }

   return(0);
}

////////////////////////////////////////////////////////////////////////


