/*
	login_validate.cpp
*/

#define _GNU_SOURCE
#define BUFFSIZE	50
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <shadow.h>
#include <syslog.h>
#include <pwd.h>
#include <string.h>

int destroy_str(char*);
int get_stdin(char*);

////////////////////////////////////////////////////////////////////

int main(void)
{
   char name[BUFFSIZE];
   char password[BUFFSIZE];

   int x=0;
   char* crypto=NULL;
   struct spwd* login_info=NULL;

   for(x=0; x<BUFFSIZE; x++)
   {
      name[x]='\0';
      password[x]='\0';
   }

   if( get_stdin(name) != 0)
      return(-1);

   if( get_stdin(password) != 0)
   {
      destroy_str(password);
      return(-1);
   }

//Got name and password, validate

   login_info=getspnam(name);

   if(login_info == NULL || login_info->sp_pwdp == NULL)
   {
      destroy_str(password);
      while(sleep(2) != 0);

      syslog(LOG_MAIL | LOG_INFO,"User %s authentication failed",name);
      return(-1);
   }

   crypto=crypt(password,login_info->sp_pwdp);

   destroy_str(password);

   if(crypto == NULL)
      return(-1);

   if(strcmp(crypto,login_info->sp_pwdp) == 0)
   {
      syslog(LOG_MAIL | LOG_INFO,"User %s authentication successful",name);
      return(0);
   }

   while(sleep(1) != 0);
   syslog(LOG_MAIL | LOG_INFO,"User %s authentication failed",name);
   return(-1);
}

////////////////////////////////////////////////////////////////////

int get_stdin(char* buff)
{
   int x=0;
   int c=0;

   for(x=0; x<BUFFSIZE; x++)
      buff[x]='\0';

   for(x=0; x<(BUFFSIZE-1); x++)
   {
      c=getchar();

      if(c == '\n')
         return(0);

      if(c == EOF)
         return(-1);
      
      buff[x]=c;
   }

   return(-1);
}

////////////////////////////////////////////////////////////////////

int destroy_str(char* buff)
{
   int x=0;

   if(buff == NULL)
      return(-1);

   for(x=0; buff[x]; x++)
      buff[x]=0;

   return(0);
}

////////////////////////////////////////////////////////////////////


