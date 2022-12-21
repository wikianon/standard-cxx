/*
	preferences.cpp
*/

#define _GNU_SOURCE

#include <errno.h>
#include <stdio.h>
#include <syslog.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "mail_process.h"
#include "preferences.h"
#include "main.h"
#include "config.h"
#include "addr.h"
#include "custom_string.h"
#include "session.h"

pref_t user_pref;

enum pref_status { ps_pending, ps_done, ps_error, ps_nl, ps_addr, ps_sig };

int fill_user_pref_struct(FILE*);
pref_status parse_pref_line(char*, pref_status);
int find_addr_book_start(FILE*);
long int find_next_email_order(FILE*, char*, char*);
int find_email_position(FILE*, char*, char*, long int*, long int*);
int get_reply_to(void);

//////////////////////////////////////////////////////////////////
//This will create and fill the user_preferences struct
//   returns 0 on success
//   returns -1 on failure

int get_user_pref(void)
{
   FILE* user_pref_file=NULL;
   int status=0;

   if(user_ses.loginname == NULL)
      return(-1);

   init_user_pref();

   setuid(euid);
   user_pref_file=open_user_pref_file("r",&status);
   setuid(ruid);

   if(user_pref_file == NULL)
   {
      if(status == 0)
      {
         user_pref_file=open_user_pref_file("w+",&status);

         if(user_pref_file == NULL)
            syslog(LOG_MAIL | LOG_ERR,"user preference file creation "
                   "failed (%s)",strerror(errno));
      }
   }

   if(user_pref_file != NULL)
   {
      fill_user_pref_struct(user_pref_file);
      fclose(user_pref_file);
   }

   if(user_pref.reply_to.email == NULL)
      get_reply_to();

   return(0);
}

//////////////////////////////////////////////////////////////////
//This will open the users preference file
//   returns file stream if open successful
//   returns null if fail

FILE* open_user_pref_file(char* open_mode, int* err)
{
   char* user_pref_filename=NULL;
   FILE* user_pref_file=NULL;
   size_t file_name_length=0;
   mode_t old_mask=0;

   if(user_ses.loginname == NULL || open_mode == NULL)
      return(NULL);

   if(err != NULL)
      *err=0;

   file_name_length=sizeof(PREFDIR "/")+
			strlen(user_ses.loginname);

   if( (user_pref_filename=(char*) malloc(file_name_length)) == NULL)
   {
      syslog(LOG_MAIL | LOG_ERR,"Out of memory (%s)",strerror(errno));

      if(err != NULL)
         *err=-1;

      return(NULL);
   }

   strcpy(user_pref_filename,PREFDIR "/" );
   strcat(user_pref_filename,user_ses.loginname);

   old_mask=umask(0177);

   setuid(euid);
   user_pref_file=fopen(user_pref_filename,open_mode);
   setuid(ruid);

   if(user_pref_file == NULL)
   {

      if(err != NULL)
      {
         if(errno == ENOENT)
            *err=0;
         else
            *err=-1;
      }
 
      umask(old_mask);

      syslog(LOG_MAIL | LOG_ERR,"could not open %s pref file (%s)",
				user_ses.loginname,strerror(errno));
      free(user_pref_filename);
      return(NULL);
   }

   umask(old_mask);

   free(user_pref_filename);

   return(user_pref_file);
}

//////////////////////////////////////////////////////////////////
//This will fill a user_preference structure

int fill_user_pref_struct(FILE* user_pref_file)
{
   char* file_buff=NULL;
   size_t file_buff_size=0;
   ssize_t b_read=0;

   pref_status status=ps_pending;

   if(user_pref_file == NULL)
      return(-1);

   do
   {
      b_read=getline(&file_buff, &file_buff_size, user_pref_file);

      if(b_read > 0 && file_buff != NULL)
         status=parse_pref_line(file_buff, status);
      else
         status=ps_done;
   }
   while(status != ps_error && status != ps_done);

   if(file_buff != NULL)
      free(file_buff);

   return(0);
}

//////////////////////////////////////////////////////////////////

pref_status parse_pref_line(char* buff, pref_status status)
{
   char* temp_ptr=NULL;
   char* temp_ptr2=NULL;
   size_t temp_size=0;
   int x=0;

   if(buff == NULL)
      return(ps_error);

   if(*buff == '\n')
      return(ps_nl);

   if(status == ps_addr && *buff == '\t')
   {
      if( (inc_addr(&(user_pref.addr))) != 0)
         return(ps_error);

      for(x=0; user_pref.addr[x].name || user_pref.addr[x].email; x++);

      fill_addr(&(user_pref.addr[x]),buff);

      return(ps_addr);
   }

   if(status == ps_sig && *buff == '\t')
   {
      temp_size=0;

      temp_size=strlen(buff+1)+1;

      if(user_pref.signature != NULL)
         temp_size+=strlen(user_pref.signature);

      if( (temp_ptr2=(char*)realloc(user_pref.signature,temp_size)) == NULL)
      {
         syslog(LOG_MAIL | LOG_ERR,"Out of memory (%s)",strerror(errno));
         return(ps_error);
      }

      if(user_pref.signature == NULL)
         *temp_ptr2='\0';

      user_pref.signature=temp_ptr2;

      strcat(user_pref.signature,buff+1);

      return(ps_pending);
   }

   if(strncmp(buff,"signature",sizeof("signature")-1) == 0)
      return(ps_sig);


   if(strncmp(buff,"address_book(main)",sizeof("address_book(main)")-1) == 0)
      return(ps_addr);

   if(strncmp(buff,"reply_to",sizeof("reply_to")-1) == 0)
   {
      if(user_pref.reply_to.name != NULL || user_pref.reply_to.email != NULL)
         return(ps_pending);

      temp_ptr=strchr(buff,'=');

      if(temp_ptr == NULL)
         return(ps_error);

      temp_ptr++;

      fill_addr( &(user_pref.reply_to), temp_ptr);

      return(ps_pending);
   }

   if(strncmp(buff,"default_sort",sizeof("default_sort")-1) == 0)
   {
      temp_ptr=strchr(buff,'=');

      if(temp_ptr == NULL)
      {
         syslog(LOG_MAIL | LOG_ERR,"error in user preference file %s"
			" (invalid default_sort)",user_ses.loginname);
         return(ps_pending);
      }

      if(user_pref.sort_by != ms_none)
      {
         syslog(LOG_MAIL | LOG_ERR,"error in user preference file %s"
			" (duplicate default_sort)",user_ses.loginname);
         return(ps_pending);
      }

      if(strncmp(temp_ptr,"date_a",sizeof("date_a")-1) == 0)
         user_pref.sort_by=ms_date_a;
      else if(strncmp(temp_ptr,"date_d",sizeof("date_d")-1) == 0)
         user_pref.sort_by=ms_date_d;
      else if(strncmp(temp_ptr,"sender_a",sizeof("sender_a")-1) == 0)
         user_pref.sort_by=ms_sender_a;
      else if(strncmp(temp_ptr,"sender_d",sizeof("sender_d")-1) == 0)
         user_pref.sort_by=ms_sender_d;
      else if(strncmp(temp_ptr,"size_a",sizeof("size_a")-1) == 0)
         user_pref.sort_by=ms_size_a;
      else if(strncmp(temp_ptr,"size_d",sizeof("size_d")-1) == 0)
         user_pref.sort_by=ms_size_d;
      else if(strncmp(temp_ptr,"subject_a",sizeof("subj_a")-1) == 0)
         user_pref.sort_by=ms_subj_a;
      else if(strncmp(temp_ptr,"subj_d",sizeof("subject_d")-1) == 0)
         user_pref.sort_by=ms_subj_d;
      else
         syslog(LOG_MAIL | LOG_ERR,"error in user preference file %s"
			" (invalid default_sort)",user_ses.loginname);

      return(ps_pending);
   }

   if(strncmp(buff,"cookie",sizeof("cookie")-1 ) == 0)
   {
      temp_ptr=strchr(buff,'=');

      if(temp_ptr!=NULL)
         temp_ptr2=strchr(temp_ptr,';');

      if(temp_ptr == NULL || temp_ptr2 == NULL)
      {
         syslog(LOG_MAIL | LOG_ERR,"error in user preference file %s"
			" (invalid cookie defination)",user_ses.loginname);
         return(ps_pending);
      }      

      *temp_ptr2='\0';
      temp_ptr2++;

      user_pref.cookie_expire=strtol(temp_ptr,NULL,10);

      temp_ptr=strchr(temp_ptr2,'\n');

      if(temp_ptr != NULL)
         *temp_ptr='\0';

      user_pref.cookie_crypt=strdup(temp_ptr2);

      return(ps_pending);
   }

   return(ps_pending);
}

//////////////////////////////////////////////////////////////////

int init_user_pref(void)
{
   user_pref.addr=NULL;
   user_pref.reply_to.name=NULL;
   user_pref.reply_to.email=NULL;
   user_pref.sort_by=ms_date_a;
   user_pref.signature=NULL;
   user_pref.cookie_expire=0;
   user_pref.cookie_crypt=NULL;

   return(0);
}

//////////////////////////////////////////////////////////////////

int destroy_user_pref(void)
{
   if(user_pref.addr != NULL)
      destroy_addr_arr(&(user_pref.addr));

   if(user_pref.reply_to.name != NULL)
   {
      free(user_pref.reply_to.name);
      user_pref.reply_to.name=NULL;
   }

   if(user_pref.reply_to.email != NULL)
   {
      free(user_pref.reply_to.email);
      user_pref.reply_to.email=NULL;
   }

   if(user_pref.signature != NULL)
   {
      free(user_pref.signature);
      user_pref.signature=NULL;
   }

   if(user_pref.cookie_crypt != NULL)
   {
      free(user_pref.cookie_crypt);
      user_pref.cookie_crypt=NULL;
   }

   return(0);
}

//////////////////////////////////////////////////////////////////

int add_pref_email(FILE* file, char* name, char* email)
{
   long int file_ptr=-1;
   char* buff=NULL;
   size_t buff_size=0;

   if(file == NULL || email == NULL)
      return(-1);

//create address string

   if(name != NULL)
      buff_size=strlen(name);

   buff_size+=strlen(email) + sizeof("\t\"\" <>\n");

   buff=(char*)malloc(buff_size);

   if(buff == NULL)
   {
      syslog(LOG_MAIL | LOG_ERR,"out of memory (%s)",strerror(errno));
      return(-1);
   }

   strcpy(buff,"\t\"");
   if(name != NULL)
      strcat(buff,name);
   strcat(buff,"\" <");
   strcat(buff,email);
   strcat(buff,">\n");

//setup to write line

   if( find_addr_book_start(file) != 0)
   {
      if(feof(file) == 0)
      {
         syslog(LOG_MAIL | LOG_ERR,"could not find addr book start");
         free(buff);
         return(-1);
      }
      else
         fprintf(file,"address_book(main)\n");
   }

   file_ptr=find_next_email_order(file,name,email);

//write email into file

   if(file_ptr != -1)
      if( insert_line_in_file(file,buff,file_ptr,file_ptr) != 0)
      {
         syslog(LOG_MAIL | LOG_ERR,"could not add email to pref");
         free(buff);
         return(-1);
      }

   free(buff);

   return(0);
}

//////////////////////////////////////////////////////////////////

int delete_pref_email(FILE* file, char* name, char* email)
{
   long int file_ptr_read=-1;
   long int file_ptr_write=-1;

   if(file == NULL || email == NULL)
      return(-1);

//setup to delete email

   if( find_addr_book_start(file) != 0)
   {
      syslog(LOG_MAIL | LOG_ERR,"find address_book to delete email failed");
      return(-1);
   }

   if(find_email_position(file,name,email,&file_ptr_write,&file_ptr_read) != 0)
   {
      syslog(LOG_MAIL | LOG_ERR,"find email position for delete failed.");
      return(-1);
   }

   if( insert_line_in_file(file,NULL,file_ptr_read,file_ptr_write) != 0)
   {
      syslog(LOG_MAIL | LOG_ERR,"error deleting pref email");
      return(-1);
   }

   return(0);
}

//////////////////////////////////////////////////////////////////

int modify_pref_email(FILE* file, char* old_name, char* old_email, 
			char* new_name, char* new_email)
{
   long int file_ptr_read=-1;
   long int file_ptr_write=-1;
   char* buff=NULL;
   size_t buff_size=0;

   if(file == NULL || old_email == NULL || new_email == NULL)
      return(-1);

   if(new_name != NULL)
      buff_size=strlen(new_name);

   buff_size+=strlen(new_email) + sizeof("\t\"\" <>\n");

   buff=(char*)malloc(buff_size);

   if(buff == NULL)
      return(-1);

   strcpy(buff,"\t\"");
   if(new_name != NULL)
      strcat(buff,new_name);
   strcat(buff,"\" <");
   strcat(buff,new_email);
   strcat(buff,">\n");

//setup to delete email

   if( (find_addr_book_start(file)) != 0)
   {
      free(buff);
      return(-1);
   }

   if(find_email_position(file,old_name,old_email,
			&file_ptr_write,&file_ptr_read) != 0)
   {
      syslog(LOG_MAIL | LOG_ERR,"find email position for delete failed.");
      free(buff);
      return(-1);
   }

   if( insert_line_in_file(file,buff,file_ptr_read,file_ptr_write) != 0)
   {
      syslog(LOG_MAIL | LOG_ERR,"error modifying email");
      free(buff);
      return(-1);
   }

   free(buff);
   return(0);
}

//////////////////////////////////////////////////////////////////
//This function will point to the first email entry following
//   "address_book(main)"
//   return 0 if found
//   return -1 on error


int find_addr_book_start(FILE* file)
{
   char* file_buff=NULL;
   size_t file_buff_size=0;
   ssize_t b_read=0;
   int return_value=0;

   rewind(file);

   do
   {
      b_read=getline(&file_buff,&file_buff_size,file);

      if(b_read <= 0 || file_buff == NULL)
         return_value=-1;
   }
   while(strcmp(file_buff,"address_book(main)\n") != 0 && return_value == 0);

   if(file_buff != NULL)
      free(file_buff);

   return(return_value);
}

//////////////////////////////////////////////////////////////////
//Returns file pointer to position where email should be inserted into
//      preference file.
//   Should have file pointer already pointing to first email entry.
//   name can be null
//   returns -1 on error

long int find_next_email_order(FILE* file,char* name,char* email)
{
   char* file_buff=NULL;
   size_t file_buff_size=0;
   ssize_t b_read=0;
   long int file_ptr=-1;
   status_type status=st_pending;
   addr_t addr;

   addr.name=NULL;
   addr.email=NULL;

   if(file == NULL || email == NULL)
      return(-1);

   do
   {
      file_ptr=ftell(file);

      b_read=getline(&file_buff,&file_buff_size,file);

      if(b_read <= 0 || file_buff == NULL)
         status=st_found;
      else
      {
         if(*file_buff == '\t')
         {
            if(fill_addr(&addr,file_buff) == 0)
            {
               if(name != NULL)
               {
                  if(addr.name != NULL)
                  {
                     if(strcasecmp(addr.name,name) > 0)
                        status=st_found;
                     else if(strcasecmp(addr.name,name) == 0)
                     {
                        if(strcasecmp(addr.email,email) > 0)
                           status=st_found;
                     }
                  }
               }
               else if(addr.name != NULL)
                  status=st_found;
               else
               {
                  if(strcasecmp(addr.email,email) > 0)
                     status=st_found;
               }

               if(addr.name != NULL)
                  free(addr.name);
               free(addr.email);
            }
            else
               status=st_error;
         }
         else
            status=st_found;
      }
   }
   while(status == st_pending);

   if(status == st_error)
      file_ptr=-1;

   return(file_ptr);
}

//////////////////////////////////////////////////////////////////

int find_email_position(FILE* file, char* name, char* email, 
			long int* file_ptr_start, long int* file_ptr_end)
{
   char* file_buff=NULL;
   size_t file_buff_size=0;
   ssize_t b_read=0;

   status_type status=st_pending;
   addr_t addr;

   addr.name=NULL;
   addr.email=NULL;

   *file_ptr_start=-1;
   *file_ptr_end=-1;

   if(file == NULL || email == NULL)
      return(-1);

   do
   {
      *file_ptr_start=ftell(file);

      b_read=getline(&file_buff,&file_buff_size,file);

      if(b_read <= 0 || file_buff == NULL)
         status=st_error;
      else
      {
         if(*file_buff == '\t')
         {
            fill_addr(&addr,file_buff);

            if(name != NULL)
            {
               if(addr.name != NULL)
               {
                  if(strcasecmp(addr.name,name) == 0)
                  {
                     if(strcasecmp(addr.email,email) == 0)
                        status=st_found;
                  }
               }
            }
            else if(addr.name == NULL)
            {
               if(strcasecmp(addr.email,email) == 0)
                  status=st_found;
            }

            if(addr.name != NULL)
            {
               free(addr.name);
               addr.name=NULL;
            }
            if(addr.email != NULL)
            {
               free(addr.email);
               addr.email=NULL;
            }
         }
         else
            status=st_done;
      }
   }
   while(status == st_pending);

   if(status != st_found)
   {
      *file_ptr_start=-1;
      *file_ptr_end=-1;
      return(-1);
   }

   *file_ptr_end=ftell(file);

   return(0);
}

//////////////////////////////////////////////////////////////////

char* get_default_hostname(void)
{

#define HOSTNAMESIZE	80

   char* file_buff=NULL;
   size_t file_buff_size=0;
   ssize_t b_read=0;

   FILE* default_pref_file=NULL;
   char* hostname=NULL;
   char* tmp_ptr=NULL;

   default_pref_file=fopen(PREFDIR "/default","r");

   if(default_pref_file != NULL)
   {
      do
      {
         b_read=getline(&file_buff,&file_buff_size,default_pref_file);

         if(b_read <= 0 || file_buff == NULL)
         {
            if(file_buff != NULL)
               free(file_buff);

            fclose(default_pref_file);
            default_pref_file=NULL;
         }
         else if(strncasecmp(file_buff,"hostname",sizeof("hostname")-1) == 0)
         {
            tmp_ptr=strrpcbrk(file_buff," \t'\"\n");
   
            if(tmp_ptr != NULL)
               *(tmp_ptr+1)='\0';
  
            tmp_ptr=strchr(file_buff,'=');

            if(tmp_ptr != NULL)
            {
               tmp_ptr=strpcbrk(tmp_ptr+1," \t'\"\n");

               if(tmp_ptr != NULL)
               {
                  if( (hostname=strdup(tmp_ptr)) == NULL)
                  {
                     syslog(LOG_MAIL | LOG_ERR,"Out of memory (%s)",
						strerror(errno));
                     fclose(default_pref_file);
                     default_pref_file=NULL;
                     return(NULL);
                  }
               }
            }
         }  
      }
      while(hostname == NULL && default_pref_file != NULL);

      if(file_buff != NULL)
         free(file_buff);

      fclose(default_pref_file);
      default_pref_file=NULL;
   }

   if(hostname != NULL)
      return(hostname);

   hostname=(char*) malloc(HOSTNAMESIZE);

   if(hostname == NULL)
   {
      syslog(LOG_MAIL | LOG_ERR,"Out of memory(%s)",strerror(errno));
      return(NULL);
   }

   if(gethostname(hostname,HOSTNAMESIZE) != 0)
      syslog(LOG_MAIL | LOG_ERR,"Hostname too big (%s)",strerror(errno));
      
   return(hostname); 
}

//////////////////////////////////////////////////////////////////

int get_reply_to(void)
{
   char* hostname=NULL;

   if(user_pref.reply_to.email != NULL)
      return(0);

   if(user_ses.loginname == NULL)
      return(-1);

   if(user_pref.reply_to.name == NULL && user_ses.name != NULL)
   {
      user_pref.reply_to.name=strdup(user_ses.name);

      if(user_pref.reply_to.name == NULL)
      {
         syslog(LOG_MAIL | LOG_ERR,"Out of memory (%s)",strerror(errno));
         return(-1);
      }
   }

   if(user_pref.reply_to.email == NULL)
   {
      if( (hostname=get_default_hostname()) == NULL)
      {
         syslog(LOG_MAIL | LOG_ERR,"get hostname failed");
         return(-1);
      }

      user_pref.reply_to.email=(char*) 
		malloc(strlen(user_ses.loginname)+strlen(hostname)+2);

      if(user_pref.reply_to.email == NULL)
      {
         syslog(LOG_MAIL | LOG_ERR,"Out of memory (%s)",strerror(errno));
         free(hostname);
         return(-1);
      }

      strcpy(user_pref.reply_to.email,user_ses.loginname);
      strcat(user_pref.reply_to.email,"@");
      strcat(user_pref.reply_to.email,hostname);

      free(hostname);
   }

   return(0);
}

//////////////////////////////////////////////////////////////////

int save_pref_entry(char* key, char* value)
{
   FILE* pref_file=NULL;
   long int start_ptr=0;
   long int end_ptr=0;

   char* file_buff=NULL;
   size_t file_buff_size=0;
   ssize_t b_read=0;

   char* new_value=NULL;
   size_t key_size=0;
   int status=0;

   if(key == NULL || value == NULL)
      return(-1);

   new_value=(char*) malloc(strlen(key)+strlen(value)+sizeof("=\n"));

   if(new_value == NULL)
   {
      syslog(LOG_MAIL | LOG_ERR,"Out of memory (%s)",strerror(errno));
      return(-1);
   }

   strcpy(new_value,key);
   strcat(new_value,"=");
   strcat(new_value,value);
   strcat(new_value,"\n");

   pref_file=open_user_pref_file("r+",&status);

   if(pref_file == NULL && status == 0)
      pref_file=open_user_pref_file("w+",NULL);

   if(pref_file == NULL)
   {
      free(new_value);
      new_value=NULL;
      return(-1);
   }

   key_size=strlen(key);

   status=0;

   do
   {
      start_ptr=ftell(pref_file);
      b_read=getline(&file_buff, &file_buff_size, pref_file);
      end_ptr=ftell(pref_file);

      if(b_read >= 0 && file_buff != NULL)
         if( strncasecmp(key,file_buff,key_size) == 0)
            status=1;
   }
   while(b_read >= 0 && status == 0);

   if(file_buff != NULL)
   {
      free(file_buff);
      file_buff=NULL;
   }

   if(start_ptr == -1 || end_ptr == -1)
   {
      free(new_value);
      fclose(pref_file);
      syslog(LOG_MAIL | LOG_ERR,"ftell failed");
      return(-1);
   }

   if(status == 1)
   {
      if( insert_line_in_file(pref_file,new_value,end_ptr,start_ptr) != 0)
      {
         free(new_value);
         fclose(pref_file);
         syslog(LOG_MAIL | LOG_ERR,"could not add line to pref file");
         return(-1);
      }
   }
   else
   {
      if( fseek(pref_file,0,SEEK_END) != 0)
      {
         free(new_value);
         fclose(pref_file);
         syslog(LOG_MAIL | LOG_ERR,"fseek failed");
         return(-1);
      }
      if( fprintf(pref_file,"%s",new_value) < 0)
      {
         free(new_value);
         fclose(pref_file);
         syslog(LOG_MAIL | LOG_ERR,"error writing to pref file %s",
			user_ses.loginname);
         return(-1);
      }
   }

   free(new_value);
   fclose(pref_file);

   return(0);
}

//////////////////////////////////////////////////////////////////


