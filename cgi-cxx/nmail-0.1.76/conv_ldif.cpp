/*
	conv_ldif.cpp
*/

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <errno.h>

#include "addr.h"
#include "custom_string.h"

int get_ldif_entry(FILE*, char**, char**);

//////////////////////////////////////////////////////////////


int convert_from_ldif(FILE* file, addr_t** addr)
{
   int x=0;
   int done=0;

   addr_t* new_addr=NULL;

   if(file == NULL || addr == NULL)
      return(-1);

   rewind(file);

   do
   {
      if(inc_addr(&new_addr) != 0)
         return(-1);

      for(x=0; new_addr[x].name || new_addr[x].email; x++);

      done=get_ldif_entry(file, &(new_addr[x].name), &(new_addr[x].email));
   }
   while(done == 0);

   *addr=new_addr;

   return(0);
}

//////////////////////////////////////////////////////////////

int get_ldif_entry(FILE* file, char** name, char** email)
{
   char* file_buff=NULL;
   size_t file_buff_size=0;
   ssize_t b_read=0;

   int done=1;
   int person=0;
   char* tmp_ptr=NULL;
   char* end_ptr=NULL;


   do
   {
      b_read=getline(&file_buff, &file_buff_size, file);

      if(b_read < 0 || file_buff == NULL)
         done=-1;

      else if(*file_buff == 0x0a || *file_buff == 0x0d)
         done=0;

      else if(strncasecmp(file_buff,"cn:",sizeof("cn:")-1) == 0)
      {
         if( (tmp_ptr=strchr(file_buff,':')) != NULL)
         {
            if( (end_ptr=strrpcbrk(tmp_ptr," \t\"'\n\x0d")) != NULL)
               *(end_ptr+1)='\0';

            if( (tmp_ptr=strpcbrk(tmp_ptr+1," \t\"'")) != NULL)
            {
               *name=strdup(tmp_ptr);

               if(*name == NULL)
               {
                  syslog(LOG_MAIL | LOG_ERR,"Out of memory (%s)",
				strerror(errno));
                  done=-1;
               }
            }
         }
      }
      else if(strncasecmp(file_buff,"mail:",sizeof("mail:")-1) == 0)
      {
         if( (tmp_ptr=strchr(file_buff,':')) != NULL)
         {
            if( (end_ptr=strrpcbrk(tmp_ptr," \t\"'\n\x0d")) != NULL)
               *(end_ptr+1)='\0';

            if( (tmp_ptr=strpcbrk(tmp_ptr+1," \t\"'")) != NULL)
            {
               *email=strdup(tmp_ptr);

               if(*email == NULL)
               {
                  syslog(LOG_MAIL | LOG_ERR,"Out of memory (%s)",
				strerror(errno));
                  done=-1;
               }
            }
         }
      }
      else if(strncasecmp(file_buff,"objectclass:",
			sizeof("objectclass:")-1) == 0)
      {
         if( (tmp_ptr=strchr(file_buff,':')) != NULL)
         {
            tmp_ptr=strpcbrk(tmp_ptr+1," \t\"'");

            if(tmp_ptr != NULL)
               if(strncasecmp(tmp_ptr,"person",sizeof("person")-1) == 0)
                  person=1;
         }
      }
   }
   while(done == 1);

   if(file_buff != NULL)
      free(file_buff);

   if(person == 0)
   {
      if(*name != NULL)
      {
         free(*name);
         *name=NULL;
      }
      if(*email != NULL)
      {
         free(*email);
         *email=NULL;
      }
   }

   if(*email == NULL && *name != NULL)
   {
      free(*name);
      *name=NULL;
   }

   return(done);
}

//////////////////////////////////////////////////////////////

