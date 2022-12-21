/*
	addr.cpp
*/

#include <string.h>
#include <stdlib.h>
#include <syslog.h>
#include <errno.h>

#include "addr.h"
#include "custom_string.h"

////////////////////////////////////////////////////////////////////
// This will take buff, separate into name/email pairs in an
//    array of address structures
// NOTE: will free buff upon exit


addr_t* parse_addr(char** buff)
{
   addr_t* addr=NULL;
   char* next_ptr=NULL;
   int x=0;

   if(buff == NULL || *buff == NULL)
      return(NULL);
   
   next_ptr=*buff;

   while(next_ptr != NULL)
   {
      if( inc_addr(&addr) != 0)
      {
         free(*buff);
         *buff=NULL;
         return(NULL);
      }

      next_ptr=fill_addr(&(addr[x]),next_ptr);

      if(addr[x].name != NULL || addr[x].email != NULL)
         x++;
   }

   free(*buff);
   *buff=NULL;

   if(x == 0)
   {
      destroy_addr_arr(&addr);
      return(NULL);
   }

   return(addr);
}

////////////////////////////////////////////////////////////////////
// This will take buff, separate it into name/email pairs, and return a
//    pointer in buff that points to any remaining name/email addresses
// NOTE: will modify buff and insert NULL characters where appropriate
//    

char* fill_addr(addr_t* addr, char* buff)
{
   int email_found=0;

   char* start_ptr=NULL;
   char* return_ptr=NULL;

   char* tmp_start_ptr=NULL;
   char* tmp_end_ptr=NULL;

   if(buff == NULL || addr == NULL)
      return(NULL);
 
   addr->name=NULL; 
   addr->email=NULL;
 
// Find delimiter ',' or ';', set = '\0' if found, return_ptr=delimiter+1
//   I know ';' isn't a valid delimiter, but we are going to handle it anyway

   tmp_start_ptr=strchr(buff,'"');

   if(tmp_start_ptr != NULL)
      tmp_end_ptr=strchr(tmp_start_ptr+1,'"');

   return_ptr=strpbrk(buff,",;");

   if(tmp_start_ptr != NULL && tmp_end_ptr != NULL && return_ptr != NULL)
      if( (return_ptr < tmp_end_ptr) && (return_ptr > tmp_start_ptr) )
         return_ptr=strpbrk(tmp_end_ptr,",;");

   if(return_ptr != NULL) 
   {
      *return_ptr='\0';
      return_ptr++;
   }

// find "<email>"

   tmp_end_ptr=strrchr(buff,'>');

   if(tmp_end_ptr != NULL)
      tmp_start_ptr=strrchr(buff,'<');


// verify "<email>", null '<' '>' and eliminate LWSP and any quotes

   if( (tmp_end_ptr == NULL || tmp_start_ptr == NULL) ||
          (tmp_start_ptr >= tmp_end_ptr) ) 
   {
      email_found=0;

      tmp_end_ptr=NULL;
      tmp_start_ptr=NULL;
   }
   else
   {
      email_found=-1;

      *tmp_start_ptr='\0';
      *tmp_end_ptr='\0';
      tmp_start_ptr++;


      tmp_end_ptr=strrpcbrk(tmp_start_ptr," \t\"'");

      if(tmp_end_ptr != NULL)
      {
         tmp_end_ptr++;
         *tmp_end_ptr='\0';
      }

      tmp_start_ptr=strpcbrk(tmp_start_ptr," \t\"'");
   }

//find end of name, set = '\0'

   tmp_end_ptr=strrpcbrk(buff," \t\"'\n");

   if(tmp_end_ptr != NULL)
   {
      tmp_end_ptr++;
      *tmp_end_ptr='\0';
   } 
 
   start_ptr=strpcbrk(buff," \t\"'");

// copy email and names
    
   if(email_found)
   {
      if(tmp_start_ptr != NULL)
         addr->email=strdup(tmp_start_ptr);

      if(start_ptr != NULL)
         addr->name=strdup(start_ptr);
   }
   else if(start_ptr != NULL)
   {
      if( valid_email(start_ptr) )
         addr->email=strdup(start_ptr);
      else
         addr->name=strdup(start_ptr);
   }

   return(return_ptr);
}

////////////////////////////////////////////////////////////////////
// This will increase the array addr by one structure
//    also inits new entry .name=NULL & .email=NULL

int inc_addr(addr_t** addr)
{
   addr_t* new_addr=NULL;
   int x=0;

   if(addr == NULL)
      return(-1);

   if(*addr == NULL)
   {
      if( (new_addr=(addr_t*) malloc(sizeof(addr_t)*2)) == NULL)
      {
         syslog(LOG_MAIL | LOG_ERR,"Out of memory (%s)",strerror(errno));
         return(-1);
      }


      new_addr[0].name=NULL;
      new_addr[0].email=NULL;
      new_addr[0].status=0;

      new_addr[1].name=NULL;
      new_addr[1].email=NULL;
      new_addr[1].status=0;

      *addr=new_addr;
   }
   else
   {
      for(x=0; (*addr)[x].email || (*addr)[x].name; x++);

      if( (new_addr=(addr_t*) realloc(*addr,sizeof(addr_t)*(x+2))) == NULL)
      {
         syslog(LOG_MAIL | LOG_ERR,"Out of memory (%s)",strerror(errno));
         destroy_addr(*addr);
         return(-1);
      }

      new_addr[x+1].name=NULL;
      new_addr[x+1].email=NULL;
      new_addr[x+1].status=0;

      *addr=new_addr;
   }

   return(0);
}

////////////////////////////////////////////////////////////////////
// This will free an addr array
//    array is terminated by a NULL name and email entry

int destroy_addr_arr(addr_t** addr)
{
   int x=0;

   if(addr == NULL || *addr == NULL)
      return(-1);

   for(x=0; (*addr)[x].name || (*addr)[x].email; x++)
   {
      if( (*addr)[x].name != NULL)
      {
         free( (*addr)[x].name );
         (*addr)[x].name=NULL;
      }
      if( (*addr)[x].email != NULL)
      {
         free( (*addr)[x].email );
         (*addr)[x].email=NULL;
      }
   }

   free(*addr);
   *addr=NULL;

   return(0);
}

////////////////////////////////////////////////////////////////////

int destroy_addr(addr_t* addr)
{
   if(addr == NULL)
      return(-1);

   if(addr->name != NULL)
   {
      free(addr->name);
      addr->name=NULL;
   }

   if(addr->email != NULL)
   {
      free(addr->email);
      addr->email=NULL;
   }

   return(0);
}

////////////////////////////////////////////////////////////////////
// This will check the email pointed to by buff to see if it is a valid
//   email address
// Returns non-zero if valid


int valid_email(char* buff)
{
   char* tmp_ptr=NULL;

   if(buff == NULL)
      return(0);

   if(strpbrk(buff,",;: \t\n'\"<>()[]\\") != NULL)
      return(0);

   if(*buff == '@' || *buff == '.')
      return(0);

   if( (tmp_ptr=strchr(buff,'@')) == NULL)
      return(0);

   if( strchr(tmp_ptr+1,'@') != NULL)
      return(0);

   if(*(tmp_ptr+1) == '.')
      return(0);

   tmp_ptr=strchr(tmp_ptr,'.');

   if(tmp_ptr == NULL)
      return(0);

   if(*(tmp_ptr+1) == '\0' || *(tmp_ptr+1) =='.')
      return(0);

   while( (tmp_ptr=strchr(tmp_ptr+1,'.')) != NULL)
   {
      if(*(tmp_ptr+1) == '\0' || *(tmp_ptr+1) == '.')
         return(0);
   }

   return(-1);
}

////////////////////////////////////////////////////////////////////


