/*
	conv_csv.cpp
*/

#define _GNU_SOURCE

#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

#include "custom_string.h"
#include "addr.h"


int parse_csv_header(FILE*, int*, int*, int*, int*);
int get_addr_from_csv(FILE*, addr_t**, int, int, int, int);
int skip_lwsp(char**);

//////////////////////////////////////////////////////////////////////

int convert_from_csv(FILE* file, addr_t** addr)
{
   int name=0;
   int first_name=0;
   int last_name=0;
   int email=0;

   int err=0;

   addr_t* new_addr=NULL;

   if(file == NULL || addr == NULL)
      return(-1);

   rewind(file);

   err=parse_csv_header(file, &name, &first_name, &last_name, &email);

   if(err != 0)
   {
      syslog(LOG_MAIL | LOG_INFO,"Error during csv import, bad headers");
      return(err); 
   }

   err=get_addr_from_csv(file, &new_addr, name, first_name, last_name, email);

   if(err != 0)
   {
      syslog(LOG_MAIL | LOG_ERR,"Error during cvs import, bad file");
      return(err);
   }

   *addr=new_addr;

   return(0);
}

//////////////////////////////////////////////////////////////////////
// This will search the header fields for:
//     "Name", "Email", "First Name", "Last Name"
//   first name and last name will only be used if name does not exist
//   sets the value to the number of commas before that entry
//   returns 0 on success, 1 on no name or first/last name or no email
//   returns -1 on fatal error

int parse_csv_header(FILE* file, 
		     int* name, 
		     int* first_name, 
		     int* last_name, 
		     int* email)
{
   char* file_buff=NULL;
   size_t file_buff_size=0;
   ssize_t b_read=0;

   char* start_ptr=NULL;
   char* end_ptr=NULL;
   char* next_ptr=NULL;

   int comma_num=0;

   *name=-1;
   *first_name=-1;
   *last_name=-1;
   *email=-1;

   b_read=getline(&file_buff,&file_buff_size,file);

   if(b_read < 0 || file_buff == NULL)
   {
      if(file_buff != NULL)
         free(file_buff);
      syslog(LOG_MAIL | LOG_ERR,"Could not read csv header");
      return(-1);
   }

   start_ptr=file_buff;

   while(start_ptr != NULL)
   {
      next_ptr=strchr(start_ptr,',');

      if(next_ptr != '\0')
      {
         *next_ptr='\0';
         next_ptr++;
      }

      start_ptr=strpcbrk(start_ptr," \t\"'");

      if(start_ptr != NULL)
      {
         end_ptr=strrpcbrk(start_ptr," \t\"'");

         if(end_ptr != NULL)
            *(end_ptr+1)='\0';

         if(strcasecmp(start_ptr,"Name") == 0 && *name == -1)
            *name=comma_num;
         else if(strcasecmp(start_ptr,"First Name") == 0 && *first_name == -1)
            *first_name=comma_num;
         else if(strcasecmp(start_ptr,"Last Name") == 0 && *last_name == -1)
            *last_name=comma_num;
         else if(strncasecmp(start_ptr,"E-mail",sizeof("E-mail")-1)
		 == 0 && *email == -1)
            *email=comma_num;
      }
      comma_num++;

      start_ptr=next_ptr;
   } 

   free(file_buff);

   if( *email == -1)
   {
      syslog(LOG_MAIL | LOG_ERR,"Error during csv import, no email in header");
      return(1);
   }

   return(0);

}

//////////////////////////////////////////////////////////////////////


int get_addr_from_csv(FILE* file, 
		     addr_t** new_addr,
		     int name, 
		     int first_name, 
		     int last_name, 
		     int email)
{
   char* file_buff=NULL;
   size_t file_buff_size=0;
   ssize_t b_read=0;

   char* c_name=NULL;
   char* c_first_name=NULL;
   char* c_last_name=NULL;
   char* c_email=NULL;

   char* tmp_ptr=NULL;
   int comma_num=0;
   int x=0;

   addr_t* addr=NULL;

   do
   {
      b_read=getline(&file_buff, &file_buff_size, file);

      if(b_read >= 0 && file_buff != NULL)
      {
         tmp_ptr=file_buff;
         comma_num=0;

         while(tmp_ptr != NULL)
         {
            if(name == comma_num)
               c_name=tmp_ptr;
            else if(first_name == comma_num)
               c_first_name=tmp_ptr;
            else if(last_name == comma_num)
               c_last_name=tmp_ptr;
            else if(email == comma_num)
               c_email=tmp_ptr;

            tmp_ptr=strchr(tmp_ptr,',');

            if(tmp_ptr != NULL)
            {
               *tmp_ptr='\0';
               tmp_ptr++;
               comma_num++;
            }
         }

         skip_lwsp(&c_name);
         skip_lwsp(&c_first_name);
         skip_lwsp(&c_last_name);
         skip_lwsp(&c_email);

         if(c_email != NULL)
         {
            if(inc_addr(&addr) != 0)
            {
               free(file_buff);
               destroy_addr_arr(&addr);
               return(-1);
            }

            for(x=0; addr[x].name || addr[x].email; x++);
 
            if(c_name != NULL)
            {
               if( (addr[x].name=strdup(c_name)) == NULL)
               {
                  free(file_buff);
                  destroy_addr_arr(&addr);
                  syslog(LOG_MAIL | LOG_ERR,"Out of memory (%s)",
				strerror(errno));
                  return(-1);
               }
            }
            else if(c_first_name != NULL && c_last_name != NULL)
            {
               addr[x].name=(char*)malloc(
			strlen(c_first_name)+strlen(c_last_name)+sizeof(" "));
               if(addr[x].name == NULL)
               {
                  free(file_buff);
                  destroy_addr_arr(&addr);
                  syslog(LOG_MAIL | LOG_ERR,"Out of memory (%s)",
				strerror(errno));
                  return(-1);
               }

               strcpy(addr[x].name,c_first_name);
               strcat(addr[x].name," ");
               strcat(addr[x].name,c_last_name);
            }
            else if(c_first_name != NULL)
            {
               addr[x].name=strdup(c_first_name);

               if(addr[x].name == NULL)
               {
                  free(file_buff);
                  destroy_addr_arr(&addr);
                  syslog(LOG_MAIL | LOG_ERR,"Out of memory (%s)",
				strerror(errno));
                  return(-1);
               }
            }
            else if(c_last_name != NULL)
            {
               addr[x].name=strdup(c_last_name);

               if(addr[x].name == NULL)
               {
                  free(file_buff);
                  destroy_addr_arr(&addr);
                  syslog(LOG_MAIL | LOG_ERR,"Out of memory (%s)",
				strerror(errno));
                  return(-1);
               }
            }

            if(c_email != NULL)
            {
               if( (addr[x].email=strdup(c_email)) == NULL)
               {
                  free(file_buff);
                  destroy_addr_arr(&addr);
                  syslog(LOG_MAIL | LOG_ERR,"Out of memory (%s)",
				strerror(errno));
                  return(-1);
               }
            }
         }
      }
   }
   while(b_read >= 0);

   if(file_buff != NULL)
      free(file_buff);

   *new_addr=addr;

   return(0);
}

//////////////////////////////////////////////////////////////////////

int skip_lwsp(char** buff)
{
   char* tmp_ptr=NULL;

   if(buff == NULL || *buff == NULL)
      return(-1);

   tmp_ptr=strrpcbrk(*buff," \t\"'\n\x0d");

   if(tmp_ptr != NULL)
      *(tmp_ptr+1)='\0';

   *buff=strpcbrk(*buff," \t\"'");   

   return(0);
}

//////////////////////////////////////////////////////////////////////


