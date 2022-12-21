/*
	login_authenticate.cpp
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <errno.h>

#include "parse_form.h"
#include "session.h"
#include "main.h"
#include "err_page.h"

int initial_login_attempt(void);
int verify_login_session(void);


/////////////////////////////////////////////////////////////////////


int login_authenticate(void)
{
   if(cleanup_sessions())
   {
      syslog(LOG_MAIL | LOG_ERR,"Error trying to cleanup old sessions");
      return(-1);
   }

   if(cleanup_tmp_files())
   {
      syslog(LOG_MAIL | LOG_ERR,"Error trying to cleanup stale tmp files");
      return(-1);
   }

   if(mail_action == login)
      return(initial_login_attempt());

   return(verify_login_session());
}

/////////////////////////////////////////////////////////////////////
//This will get the userid/password from cgi parameters and attemp
//   to create a session if user/password okay. 

int initial_login_attempt(void)
{
   char* password=NULL;

   cgi_t* cgi_data=NULL;

   while( get_cgi_data(&cgi_data) == 0 )
   {
      if( strcasecmp(cgi_data->name,"userid") == 0 && cgi_data->value != NULL)
      {
         user_ses.loginname=cgi_data->value;
         cgi_data->value=NULL;
         free(cgi_data->name);
         cgi_data->name=NULL;
      }
      else if( strcasecmp(cgi_data->name,"password") == 0)
      {
         password=cgi_data->value;
         cgi_data->value=NULL;
         free(cgi_data->name);
         cgi_data->name=NULL;
      }
   }


   if( user_ses.loginname == NULL || password == NULL)
   {
      syslog(LOG_MAIL | LOG_ERR,"invalid CGI header during initial login");
      return(-1);
   }

   return(create_session(password));

}

/////////////////////////////////////////////////////////////////////

int verify_login_session(void)
{
   return(check_session());
}

/////////////////////////////////////////////////////////////////////
