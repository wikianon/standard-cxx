/*
	send_message.cpp
*/



#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <signal.h>

#include "parse_form.h"
#include "environment.h"
#include "config.h"
#include "session.h"
#include "main.h"
#include "random.h"
#include "inbox_page.h"
#include "err_page.h"
#include "mail_process.h"
#include "parse_mail_headers.h"
#include "view_message.h"
#include "preferences.h"
#include "headers.h"

struct send_info {
   char* from;
   addr_t* to;
   addr_t* cc;
   addr_t* bcc;
   char* subject;
   char* body;
   FILE* att;
   char* filename;
   char* att_cont_type;
   char* new_boundary;
   int refw;
   FILE* refw_file;
   mail_header* mail_h;
};

int init_send_info_struct(send_info*);
int delete_send_info_struct(send_info*);
int fill_send_info_struct(send_info*);
int cleanup_send_info_struct(send_info*);
int gen_header(char*, char**);
int send_attachment(struct send_info*, FILE*);
int conv_to_base64(int*, char*);
int base64_char_conv(int);

int sendmail_fork_child_process(int*);
int sendmail_fork_parent_process(int*, pid_t, send_info*);
int init_refw_msg(struct send_info*);
int send_refw_message(FILE*, struct send_info*);
int send_addr(FILE*, addr_t*);
int err_addr(send_info*);


////////////////////////////////////////////////////////////////////
// This function gets send_mail info from stdin, generates a mail
//    message, and forks a sendmail process to deliver the message

int send_message_action(void)
{
   int err=0;
   pid_t pid=0;
   send_info info;
   int fdes[2];

   init_send_info_struct(&info);
   err=fill_send_info_struct(&info);

   if(err != 0 && err != 1)
   {
      syslog(LOG_MAIL | LOG_ERR,"Error setting up mail to send. "
                                "User abort?");
      err_page();
      return(-1);
   }

   if( (cleanup_send_info_struct(&info)) != 0)
   {
      delete_send_info_struct(&info);
      return(-1);
   }

   if(info.refw != -1)			//if this is a reply/forward message and we need to attach
   {					//   a previous message (non-text)
      if(init_refw_msg(&info) != 0) 
      {
         syslog(LOG_MAIL | LOG_ERR,"Could not attach refw message.");
         err_string_page("Send message failed.");
         delete_send_info_struct(&info);
         return(-1);
      }
   }

   fflush(stdout);

   if(pipe(fdes) != 0)
   {
      syslog(LOG_MAIL | LOG_ERR,"pipe failed (%s)",strerror(errno));
      err_page();
      delete_send_info_struct(&info);
      return(-1);
   }

   pid=fork();

   if(pid == 0)
   {
      sendmail_fork_child_process(fdes);
      exit(-1);
   }
   else if(pid > 0)
      err=sendmail_fork_parent_process(fdes,pid,&info);
   else
   {
      syslog(LOG_MAIL | LOG_ERR,"fork failed (%s)",strerror(errno));
      err_page();
      delete_send_info_struct(&info);
      return(-1);
   }

   delete_send_info_struct(&info);

   if(err == 0)
      inbox_action();
   else
      err_string_page("Send message failed.");

   return(0);
}

///////////////////////////////////////////////////////////////////

int sendmail_fork_parent_process(int* fdes, 
				pid_t child_pid, 
				send_info* info)
{
   FILE* out_pipe=NULL;
   int status=0;
   int err=0;

   close(fdes[0]);

   out_pipe=fdopen(fdes[1],"w");

   if(info->att != NULL && info->new_boundary == NULL)
   {
      info->new_boundary=get_rand_string(20);

      if(info->new_boundary == NULL)
      {
         syslog(LOG_MAIL | LOG_ERR,"Out of memory.");
         kill(child_pid,SIGKILL);
         return(-1);
      }
   }

   if(info->from != NULL)
      fprintf(out_pipe,"%s\n",info->from);
   if(info->to != NULL)
   {
      fprintf(out_pipe,"To: ");
      send_addr(out_pipe,info->to);
   }
   if(info->cc != NULL)
   {
      fprintf(out_pipe,"cc: ");
      send_addr(out_pipe,info->cc);
   }
   if(info->bcc != NULL)
   {
      fprintf(out_pipe,"bcc: ");
      send_addr(out_pipe,info->bcc);
   }

   if(info->subject != NULL)
      fprintf(out_pipe,"%s\n",info->subject);

   fprintf(out_pipe,"X-Mailer: Nmail " VERSION "\n");
   fprintf(out_pipe,"MIME-Version: 1.0\n");

   if(info->new_boundary != NULL)
      fprintf(out_pipe,"Content-Type: multipart/mixed;\n\tboundary=\"%s\"\n",
				info->new_boundary);
   fprintf(out_pipe,"\n");  
 
   if(info->body != NULL)
   {
      if(info->new_boundary != NULL)
      {
         fprintf(out_pipe,"\n--%s\n",info->new_boundary);
         fprintf(out_pipe,"Content-Type: text/plain\n\n");
      }

      fprintf(out_pipe,"%s\n",info->body);
   }

   if(info->att != NULL)
   {
      send_attachment(info,out_pipe);
   }

   if(info->refw_file != NULL)
      if(send_refw_message(out_pipe,info) != 0)
      {
         kill(child_pid,SIGKILL);
         syslog(LOG_MAIL | LOG_ERR,"Error sending reply/forward.");
         return(-1);
      }

   if(info->mail_h != NULL)
      if(info->mail_h->cont_type == ct_multipart_alternative)
         fprintf(out_pipe,"\n--%s--\n",info->mail_h->cont_boundary);

   if(info->new_boundary != NULL)
      fprintf(out_pipe,"\n--%s--\n",info->new_boundary);

   fclose(out_pipe);

   err=waitpid(child_pid,&status,WUNTRACED);

   return(status);
}

///////////////////////////////////////////////////////////////////

int sendmail_fork_child_process(int* fdes)
{
   close(fdes[1]);
   dup2(fdes[0],0);
   close(fdes[0]);
   dup2(2,1);			//redirect stdout to stderr
				//   sendmail should not stdout
   char* from={"-f"};
#ifndef SENDMAIL_N_U
   char* new_argv[]={SENDMAIL,"-bm","-i","-t","-U",NULL,NULL,NULL,NULL};
   #define START_ARG	5
#else
   char* new_argv[]={SENDMAIL,"-bm","-i","-t",NULL,NULL,NULL,NULL};
   #define START_ARG	4
#endif


   if( strncmp(user_pref.reply_to.email,
               user_ses.loginname,
               strlen(user_ses.loginname)) == 0)
   {
      new_argv[START_ARG]=from;
      new_argv[START_ARG+1]=user_pref.reply_to.email;
   }
      
   setreuid(euid,euid);
   execv(SENDMAIL,new_argv);

   syslog(LOG_MAIL | LOG_ERR,"sendmail exec failed (%s)",strerror(errno));

   exit(-1);
}

///////////////////////////////////////////////////////////////////

int send_addr(FILE* out_pipe, addr_t* addr)
{
   int x=0;

   if(addr == NULL)
      return(-1);

   for(x=0; addr[x].name || addr[x].email; x++)
   {
      if(addr[x].email != NULL)
      {
         if(x != 0)
            fprintf(out_pipe,"\t, ");

         if(addr[x].name != NULL)
            fprintf(out_pipe,"\"%s\" <%s>\n",addr[x].name,addr[x].email);
         else
            fprintf(out_pipe, "%s\n",addr[x].email);
      }
   }

   if(x == 0)
   {
      fprintf(out_pipe,"\n");
      return(-1);
   }

   return(0);
}

///////////////////////////////////////////////////////////////////
// This function will parse all the header lines and make them
//     fit the 76 char max by folding fields as necessary

int cleanup_send_info_struct(send_info* info)
{
   int flag=0;
   int x=0;
   int y=0;

   if(info == NULL)
      return(-1);

   if(info->from != NULL)			
      if(gen_header("From: ",&(info->from)) != 0)
         return(-1);

   if(info->subject != NULL)
      if(gen_header("Subject: ",&(info->subject)) != 0)
         return(-1);

   if(info->to == NULL && info->cc == NULL && info->bcc == NULL)
   {					//if no recepients, this is an error
      err_addr(info);
      return(-1);
   }

   /* if names without email, need to check address book */
   if(info->to != NULL)
      for(x=0; info->to[x].name || info->to[x].email; x++)
         if(info->to[x].email == NULL)
            flag=-1;

   /* if names without email, need to check address book */
   if(info->cc != NULL)			
      for(x=0; info->cc[x].name || info->cc[x].email; x++)
         if(info->cc[x].email == NULL)
            flag=-1;

   /* if names without email, need to check address book */
   if(info->bcc != NULL)
      for(x=0; info->bcc[x].name || info->bcc[x].email; x++)
         if(info->bcc[x].email == NULL)
            flag=-1;

   /* if names without email and no address book */
   if(flag != 0)			
   {
      /* we cannot find name in address book, so error */
      if(user_pref.addr == NULL)	
      {
         err_addr(info);
         return(-1);
      }
   }

   flag=0;

   if(info->to != NULL)
      for(x=0; info->to[x].name || info->to[x].email; x++)	
      {					//for every name in TO line...

         /* if no email just name, try to find it in address book */
         if(info->to[x].email == NULL)
         {
            for(y=0; user_pref.addr[y].name || user_pref.addr[y].email; y++)
            {				
               if(user_pref.addr[y].name != NULL && 
                  user_pref.addr[y].email != NULL)
                  if(strcasecmp(user_pref.addr[y].name,info->to[x].name) == 0)
                  {
                     info->to[x].email=strdup(user_pref.addr[y].email);
                     break;
                  }
            }
            if(info->to[x].email == NULL)
               flag=-1;
         }
      }

   if(info->cc != NULL)
      for(x=0; info->cc[x].name || info->cc[x].email; x++)
      {					//for every name in CC line...

         /* if no email just name, try to find it in address book */
         if(info->cc[x].email == NULL)
         {
            for(y=0; user_pref.addr[y].name || user_pref.addr[y].email; y++)
            {
               if(user_pref.addr[y].name != NULL && 
                  user_pref.addr[y].email != NULL)
                  if(strcasecmp(user_pref.addr[y].name,info->cc[x].name) 
					== 0)
                     info->cc[x].email=strdup(user_pref.addr[y].email);
            }
            if(info->cc[x].email == NULL)
               flag=-1;
         }
      }

   if(info->bcc != NULL)
      for(x=0; info->bcc[x].name || info->bcc[x].email; x++)
      {					//for every name in BCC line...

         /* if no email just name, try to find it in address book */
         if(info->bcc[x].email == NULL)
         {
            for(y=0; user_pref.addr[y].name || user_pref.addr[y].email; y++)
            {
               if(user_pref.addr[y].name != NULL && 
                  user_pref.addr[y].email != NULL)
                  if(strcasecmp(user_pref.addr[y].name,info->bcc[x].name) == 0)
                     info->bcc[x].email=strdup(user_pref.addr[y].email);
            }
            if(info->bcc[x].email == NULL)
               flag=-1;
         }
      }

   if(flag != 0)	//if there were any names without email and we
   {			//   could not find them in address book, error
      err_addr(info);
      return(-1);
   }

   return(0);
}

////////////////////////////////////////////////////////////////////
// This function will parse the cgi name/values and fill the send_info struct

int fill_send_info_struct(send_info* info)
{
   char* tmp_ptr=NULL;
   cgi_t* cgi_data=NULL;

   while( get_cgi_data(&cgi_data) == 0)
   {
      if(strcmp(cgi_data->name,"from") == 0)
      {
         if(info->from == NULL)
         {
            info->from=cgi_data->value;
            cgi_data->value=NULL;
            free(cgi_data->name);
            cgi_data->name=NULL;
         }
      }
      else if(strcmp(cgi_data->name,"to") == 0)
      {
         if(info->to == NULL)
         {
            info->to=parse_addr(&(cgi_data->value));
            free(cgi_data->name);
            cgi_data->name=NULL;
         }
      }
      else if(strcmp(cgi_data->name,"cc") == 0)
      {
         if(info->cc == NULL)
         {
            info->cc=parse_addr(&(cgi_data->value));
            free(cgi_data->name);
            cgi_data->name=NULL;
         }
           
      }
      else if(strcmp(cgi_data->name,"bcc") == 0)
      {
         if(info->bcc == NULL)
         {
            info->bcc=parse_addr(&(cgi_data->value));
            free(cgi_data->name);
            cgi_data->name=NULL;
         }
      }
      else if(strcmp(cgi_data->name,"subject") == 0)
      {
         if(info->subject == NULL)
         {
            info->subject=cgi_data->value;
            cgi_data->value=NULL;
            free(cgi_data->name);
            cgi_data->name=NULL;
         }
      }
      else if(strcmp(cgi_data->name,"body") == 0)
      {
         if(info->body == NULL)
         {
            info->body=cgi_data->value;
            cgi_data->value=NULL;
            free(cgi_data->name);
            cgi_data->name=NULL;
         }
      }
      else if(strcmp(cgi_data->name,"attachment") == 0)
      {
         if(cgi_data->filename != NULL && *(cgi_data->filename) != '\0')
         {
            tmp_ptr=strrchr(cgi_data->filename,'\\');

            if(tmp_ptr != NULL)
               tmp_ptr++;
            else
               tmp_ptr=cgi_data->filename;

            info->filename=strdup(tmp_ptr);
         }

         info->att=cgi_data->tmp_file;

         info->att_cont_type=cgi_data->cont_type;
         cgi_data->cont_type=NULL;

         free(cgi_data->name);
         cgi_data->name=NULL;

      }
      else if(strcmp(cgi_data->name,"fwmsg") == 0)
      {
         if(cgi_data->value != NULL)
            info->refw=atol(cgi_data->value);
      }
   }

   return(0);
}

/////////////////////////////////////////////////////////////////////
//This function will generate a header file. 
//   buff will be reallocated, head prepended to it, and any
//   necessary '\n\t' folding will be done

int gen_header(char* head, char** buff)
{
   char* new_buff=NULL;
   char* tmp_ptr=NULL;
   size_t new_size=0;
   char char1=0;
   char char2=0;
   char char3=0;

   if(head == NULL || buff == NULL || *buff == NULL)
      return(-1);

   new_size=strlen(head)+strlen(*buff)+1;
   new_buff=(char*)malloc(new_size);

   if(new_buff == NULL)
   {
      syslog(LOG_MAIL | LOG_ERR,"mem allocation failed (%s)",
			strerror(errno));
      return(-1);
   }

   strcpy(new_buff,head);
   strcat(new_buff,*buff);

   free(*buff);
   *buff=new_buff;

   while(strlen(new_buff) > 76)
   {
      tmp_ptr=new_buff+76;

      while(tmp_ptr > new_buff && *tmp_ptr != ' ' && *tmp_ptr != '\t')
         tmp_ptr--;
      
      if(tmp_ptr <= new_buff)
      {
         tmp_ptr=new_buff+76;

         while(*tmp_ptr != ' ' && *tmp_ptr != '\t' && *tmp_ptr != '\0')
            tmp_ptr++;
      }

      if(tmp_ptr != '\0')
      {
         new_buff=(char*)realloc(*buff,strlen(*buff)+sizeof("\n\t"));

         if(new_buff == NULL)
         {
            syslog(LOG_MAIL | LOG_ERR,"mem allocation failed (%s)",
			strerror(errno));
            return(-1);
         }
         
         *buff=new_buff;

         new_buff=tmp_ptr+2;

         char1=tmp_ptr[0];
         char2=tmp_ptr[1]; 

         tmp_ptr[0]='\n';
         tmp_ptr[1]='\t';

         tmp_ptr+=2;

         while(char1 != '\0' && char2 != '\0')
         {
            char3=tmp_ptr[0];
            tmp_ptr[0]=char1;
            char1=char3;

            char3=tmp_ptr[1];
            tmp_ptr[1]=char2;
            char2=char3;

            tmp_ptr+=2;
         }

         tmp_ptr[0]=char1;

         if(char1 != '\0')
            tmp_ptr[1]='\0';
      }
      else
         new_buff=tmp_ptr;
   }

   return(0);
}

////////////////////////////////////////////////////////////////////

int send_attachment(struct send_info* info, FILE* out_pipe)
{
   int input[3];
   char output[4];
   int x=0;
   int boundary=0;
   int char_read=0;

   rewind(info->att);

   fprintf(out_pipe,"\n--%s\n",info->new_boundary);
   fprintf(out_pipe,"Content-Type: %s;\n"
		    "\tname=\"%s\"\n",
			info->att_cont_type,info->filename);
   fprintf(out_pipe,"Content-Transfer-Encoding: base64\n");
   fprintf(out_pipe,"Content-Disposition: attachment;\n"
		    "\tfilename=\"%s\"\n\n",
			info->filename);

   do
   {
      for(x=0;x<3;x++)
      {
         char_read=getc(info->att);

         if(char_read != EOF)
            input[x]=char_read;
         else
            break;
      }
      if(x == 0)
         break;

      for(;x<3;x++)
         input[x]=0xff00;

      conv_to_base64(input,output);

      for(x=0;x<4;x++)
         putc(output[x],out_pipe);

      boundary++;

      if(boundary > 18)
      {
         putc('\n',out_pipe);
         boundary=0;
      }
   }
   while(char_read != EOF);

   putc('\n',out_pipe);

   return(0);
}

////////////////////////////////////////////////////////////////////
//this will convert the 3 binary bytes (input) to 4 6 bit characters
//      converted to base 64
//   if EOF is found, remaining characters should be 0xff00

int conv_to_base64(int input[3], char output[4])
{
   int x=0;

   output[0]=(input[0] & 0x00fc) >> 2;
   output[1]=((input[0] & 0x0003) << 4) | ((input[1] & 0x00f0) >> 4);
   output[2]=((input[1] & 0x000f) << 2) | ( (input[2] & 0x00c0) >> 6);
   output[3]=input[2] & 0x003f;

   for(x=0;x<4;x++)
      output[x]=base64_char_conv(output[x]);

   if(input[0] == 0xff00)
   {
      output[0]='=';
      output[1]='=';
      output[2]='=';
      output[3]='=';
   }
   if(input[1] == 0xff00)
   {
      output[2]='=';
      output[3]='=';
   }
   if(input[2] == 0xff00)
   {
      output[3]='=';
   }

   return(' ');
}

////////////////////////////////////////////////////////////////////

int base64_char_conv(int c)
{
   if(c >= 0 && c <= 25)
      return(c + 'A');

   if(c >= 26 && c <= 51)
      return(c - 26 + 'a');

   if(c >= 52 && c <= 61)
      return(c - 52 + '0');

   if(c == 62)
      return('+');

   if(c == 63)
      return('/');

   return(0);
}

////////////////////////////////////////////////////////////////////
// This initializes the send_info structure

int init_send_info_struct(send_info* info)
{
   if(info == NULL)
      return(-1);

   info->from=NULL;
   info->to=NULL;
   info->cc=NULL;
   info->bcc=NULL;
   info->subject=NULL;
   info->body=NULL;
   info->att=NULL;
   info->filename=NULL;
   info->att_cont_type=NULL;
   info->new_boundary=NULL;
   info->refw=-1;
   info->refw_file=NULL;
   info->mail_h=NULL;

   return(0);
}

////////////////////////////////////////////////////////////////////
// This will free any memory allocated to the send_info structure and
//    set the freed variables to NULL. Also deletes any temporary
//    files that may have been used to send message (attachments).

int delete_send_info_struct(send_info* info)
{
   if(info == NULL)
      return(-1);

   if(info->from != NULL)
   {
      free(info->from);
      info->from=NULL;
   }

   if(info->to != NULL)
      destroy_addr_arr(&(info->to));

   if(info->cc != NULL)
      destroy_addr_arr(&(info->cc));

   if(info->bcc != NULL)
      destroy_addr_arr(&(info->bcc));

   if(info->subject != NULL)
   {
      free(info->subject);
      info->subject=NULL;
   }

   if(info->body != NULL)
   {
      free(info->body);
      info->body=NULL;
   }

   if(info->filename != NULL)
   {
      free(info->filename);
      info->filename=NULL;
   }

   if(info->att_cont_type != NULL)
   {
      free(info->att_cont_type);
      info->att_cont_type=NULL;
   }

   if(info->att != NULL)
   {
      /* temp file will be closed and deleted by destroy_cgi_env() function */
      info->att=NULL;
   }

   if(info->new_boundary != NULL)
   {
      free(info->new_boundary);
      info->new_boundary=NULL;
   }

   if(info->refw_file != NULL)
   {
      fclose(info->refw_file);
      info->refw_file=NULL;
   }

   if(info->mail_h != NULL)
   {
      free_mail_headers(info->mail_h);
      info->mail_h=NULL;
   }

   return(0);
}

////////////////////////////////////////////////////////////////////
// This only gets called if the user replys/forwards a message
//    that has attachments. Text messages are reply'd/forward'd to
//    in the body of the message, only attachments happen here,
//    which means the message MUST be multipart/*, although a
//    multipart/* message does not necessairly need to call
//    this function (message/alternative, or message/mixed that
//    only has one plain/text part). The compose message file
//    handles these situations and sets things up so this function
//    is only called if necessary


int init_refw_msg(struct send_info* info)
{
   setgid(egid);
   info->refw_file=open_user_mailfile("r");
   setgid(rgid);

   if( info->refw_file == NULL)
      return(-1);

   if( point_to_message(info->refw_file,info->refw) != 0)
      return(-1);

   info->mail_h=fill_mail_h_struct(info->refw_file,info->refw);

   if(info->mail_h == NULL)
      return(-1);

   if(info->mail_h->cont_type != ct_multipart_alternative)
   {
      if(info->mail_h->cont_boundary != NULL)
      {
         info->new_boundary=strdup(info->mail_h->cont_boundary);

         if(info->new_boundary == NULL)
            return(-1);
      }
      else
      {
         info->new_boundary=get_rand_string(20);

         if(info->new_boundary == NULL)
            return(-1);
      }
   }
   else
   {
      info->new_boundary=get_rand_string(20);
      if(info->new_boundary == NULL)
         return(-1);
   }

   return(0);
}

////////////////////////////////////////////////////////////////////

int send_refw_message(FILE* out_pipe, struct send_info* info)
{
   char* file_buff=NULL;
   size_t file_buff_size=0;
   ssize_t b_read=0;
   status_type status=st_nl;
   long int file_ptr1=0;
   long int file_ptr2=0;
   mail_header* mail_h_next1=NULL;
   mail_header* mail_h_next2=NULL;

   if(out_pipe == NULL || info == NULL)
      return(-1);

   if(info->mail_h == NULL || info->refw_file == NULL)
      return(-1);

   fprintf(out_pipe,"\n--%s\n",info->new_boundary);

   if(info->mail_h->cont_type == ct_multipart_mixed)
   {
      status=find_next_border(info->refw_file,info->new_boundary);

      if(status == st_error || status == st_done)
         return(-1);

      file_ptr1=ftell(info->refw_file);

      if( (mail_h_next1=fill_mail_h_struct(info->refw_file,0)) == NULL)
         return(-1);

      if(mail_h_next1->cont_type == ct_text_plain || 
			mail_h_next1->cont_type == ct_none)
      {
         free_mail_headers(mail_h_next1);

         status=find_next_border(info->refw_file,info->new_boundary);

         if(status == st_error || status == st_done)
            return(-1);
      }
      else if(mail_h_next1->cont_type == ct_multipart_alternative)
      {
         file_ptr2=ftell(info->refw_file);

         if( (mail_h_next2=fill_mail_h_struct(info->refw_file,0)) == NULL)
            return(-1);

         if(mail_h_next2->cont_type == ct_text_plain ||
			mail_h_next2->cont_type == ct_none)
         {
            status=find_next_border(info->refw_file,
					mail_h_next1->cont_boundary);

            if(status == st_error)
            {
               free_mail_headers(mail_h_next1);
               free_mail_headers(mail_h_next2);
               return(-1);
            }

            if(status == st_done)
            {
               status=find_next_border(info->refw_file,info->new_boundary);

               if(status == st_error || status == st_done)
               {
                  free_mail_headers(mail_h_next1);
                  free_mail_headers(mail_h_next2);
                  return(-1);
               }
            }
         }
         else
         {
            fseek(info->refw_file,file_ptr2,SEEK_SET);
         }

         free_mail_headers(mail_h_next1);
         free_mail_headers(mail_h_next2);
      }
      else
      {
         free_mail_headers(mail_h_next1);
         fseek(info->refw_file,file_ptr1,SEEK_SET);
      }
   }   
   else
   {
      if(info->mail_h->cont_type_string != NULL)
      {
         fprintf(out_pipe,"Content-Type: %s",info->mail_h->cont_type_string);

         if(info->mail_h->cont_boundary != NULL)
            fprintf(out_pipe,";\n\tboundary=\"%s\"",
			info->mail_h->cont_type_string);
         if(info->mail_h->filename != NULL)
            fprintf(out_pipe,";\n\tname=\"%s\"",
			info->mail_h->filename);
         fprintf(out_pipe,"\n");
      }
      else
         fprintf(out_pipe,"Content-Type: text/plain\n");

      if(info->mail_h->cont_type_enc == cte_base64)
         fprintf(out_pipe,"Content-Transfer-Encoding: base64\n");
      if(info->mail_h->cont_type_enc == cte_quoted_printable)
         fprintf(out_pipe,"Content-Transfer-Encoding: quoted-printable\n");
      if(info->mail_h->cont_type_enc == cte_binary)
         fprintf(out_pipe,"Content-Transfer-Encoding: binary\n");
      if(info->mail_h->cont_type_enc == cte_8bit)
         fprintf(out_pipe,"Content-Transfer-Encoding: 8bit\n");
      if(info->mail_h->cont_type_enc == cte_7bit)
         fprintf(out_pipe,"Content-Transfer-Encoding: 7bit\n");

      if(info->mail_h->filename != NULL)
         fprintf(out_pipe,"Content-Disposition: attachment;\n"
			  "\tname=\"%s\"\n",info->mail_h->filename);

      fprintf(out_pipe,"\n");
   }

   do
   {
      b_read=getline(&file_buff,&file_buff_size,info->refw_file);

      if(b_read <= 0 || file_buff == NULL)
      {
         syslog(LOG_MAIL | LOG_ERR,"getline failed (%s)",strerror(errno));

         if(file_buff != NULL)
            free(file_buff);

         return(-1);
      }

      status=is_boundary(file_buff,info->mail_h->cont_boundary);

      if(status == st_error && info->mail_h->cont_boundary == NULL)
         status=st_done;

      if(status != st_error && status != st_done)
         b_read=fprintf(out_pipe,"%s",file_buff);

   }
   while(status != st_error && status != st_done && b_read > 0);

   if(file_buff != NULL)
      free(file_buff);

   if(status == st_error || b_read <= 0)
   {
      syslog(LOG_MAIL | LOG_ERR,"send_refw_message could not find ending "
			"boundary");
      return(-1);
   }

   return(0);
}

////////////////////////////////////////////////////////////////////
// This function generates a page that displays addresses that are
//    invalid (don't appear to be full email addresses)

int err_addr(send_info* info)
{
   int x=0;

   content_html();
   html_header("ERROR - invalid addresses");

   printf("<TABLE BORDER=\"0\" ALIGN=\"CENTER\" WIDTH=\"80%%\" "
          "CELLPADDING=\"1\" CELLSPACING=\"1\">\n"
          "   <TR>\n"
          "      <TD BGCOLOR=\"#002266\" ALIGN=\"LEFT\">\n"
          "      <FONT COLOR=\"#FFFFFF\" FACE=\"Arial, Helvetica\" "
          "SIZE=\"3\">\n"
          "      <B>ERROR: Invalid addresses</B>\n"
          "      </FONT>\n"
          "      </TD>\n"
          "   </TR>\n"
          "   <TR>\n"
          "      <TD BGCOLOR=\"#EEEEEE\">\n"
          "         <BR>\n");

   if(info->to == NULL && info->cc == NULL && info->bcc == NULL)
      printf("No recipients specified.<BR>");
   else
   {
      if(info->to != NULL)
         for(x=0; info->to[x].name || info->to[x].email; x++)
            if(info->to[x].email == NULL)
               printf("         <B>%s</B> is not in address book and does not "
                      "appear to be a valid email address.<BR>\n",
			info->to[x].name);

      if(info->cc != NULL)
         for(x=0; info->cc[x].name || info->cc[x].email; x++)
            if(info->cc[x].email == NULL)
               printf("         <B>%s</B> is not in address book and does not "
                      "appear to be a valid email address.<BR>\n",
			info->cc[x].name);

      if(info->bcc != NULL)
         for(x=0; info->bcc[x].name || info->bcc[x].email; x++)
            if(info->bcc[x].email == NULL)
               printf("         <B>%s</B> is not in address book and does not "
		      "appear to be a valid email address.<BR>\n",
			info->bcc[x].email);

   }
   printf("         <BR>\n"
          "      </TD>\n"
          "   </TR>\n");

   html_footer();

   return(0);
}

////////////////////////////////////////////////////////////////////


