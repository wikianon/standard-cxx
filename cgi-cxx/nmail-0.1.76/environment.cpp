/*
	environment.cpp
*/

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>	//getenv,free 
#include <errno.h>
#include <string.h>	//strdup,
#include <syslog.h>

#include "parse_form.h"
#include "environment.h"
#include "main.h"
#include "action.h"
#include "custom_string.h"

env_t user_env = {
   NULL, 		//server_name
   NULL, 		//server_addr
   NULL, 		//remote_addr
   NULL, 		//doc_root
   NULL,		//script_name
   cgi_not_specified,	//r_method
   0,			//content_length
   NULL,		//query_string
   NULL			//boundary
};

web_action_t parse_action(char*);

////////////////////////////////////////////////////////////////////////////

int init_user_env(void)
{

//SERVER_ADDR

   if(getenv("SERVER_ADDR") != NULL)
   {
      if( (user_env.server_addr=strdup(getenv("SERVER_ADDR"))) == NULL)
      {
         syslog(LOG_MAIL | LOG_ERR,"Out of memory (%s)",strerror(errno));
         return(-1);
      }
   }

//SERVER_NAME

   if(getenv("SERVER_NAME") != NULL)
   {
      if( (user_env.server_name=strdup(getenv("SERVER_NAME"))) == NULL)
      {
         syslog(LOG_MAIL | LOG_ERR,"Out of memory (%s)",strerror(errno));
         return(-1);
      }
   }

//REMOTE_ADDR

   if(getenv("REMOTE_ADDR") != NULL)
   {
      if( (user_env.remote_addr=strdup(getenv("REMOTE_ADDR"))) == NULL)
      {
         syslog(LOG_MAIL | LOG_ERR,"Out of memory (%s)",strerror(errno));
         return(-1);
      }
   }
   else
   {
      syslog(LOG_MAIL | LOG_ERR,"no REMOTE_ADDR in cgi env. "
                                "Required for authentication purposes.");
      return(-1);
   }

//DOCUMENT_ROOT
 
   if(getenv("DOCUMENT_ROOT") != NULL)
   {
      if( (user_env.doc_root=strdup(getenv("DOCUMENT_ROOT"))) == NULL)
      {
         syslog(LOG_MAIL | LOG_ERR,"Out of memory (%s)",strerror(errno));
         return(-1);
      }
   }

//SCRIPT_NAME

   if(getenv("SCRIPT_NAME") != NULL)
   {
      user_env.script_name=strdup(getenv("SCRIPT_NAME"));

      if(user_env.script_name == NULL)
      {
         syslog(LOG_MAIL | LOG_ERR,"out of memory");
         return(-1);
      }
   }
   else
   {
      syslog(LOG_MAIL | LOG_ERR,"no SCRIPT_NAME in cgi env");
      return(-1);
   }

//REQUEST_METHOD

   if(getenv("REQUEST_METHOD") != NULL)
   {
      if(strcasecmp(getenv("REQUEST_METHOD"),"GET") == 0)
         user_env.r_method=cgi_get;
      else if(strcasecmp(getenv("REQUEST_METHOD"),"POST") == 0)
         user_env.r_method=cgi_post;
      else
      {
         syslog(LOG_MAIL | LOG_ERR,"cgi env REQUEST_METHOD "
			"is not POST or GET");
         user_env.r_method=cgi_not_specified;
         return(-1);
      }      
   }
   else
   {
      syslog(LOG_MAIL | LOG_ERR,"no REQUEST_METHOD in cgi env");
      user_env.r_method=cgi_not_specified;
      return(-1);
   }

//if REQUEST_METHOD == post, get CONTENT_TYPE

   if(user_env.r_method==cgi_post)
   {
      if(getenv("CONTENT_TYPE") != NULL)
      {
         if(strncasecmp(getenv("CONTENT_TYPE"),
		"application/x-www-form-urlencoded",
		sizeof("application/x-www-form-urlencoded")-1) == 0)
            user_env.r_method=cgi_post;
         else if(strncasecmp(getenv("CONTENT_TYPE"),
		"multipart/form-data",
		sizeof("multipart/form-data")-1) == 0)
            user_env.r_method=cgi_post_multipart;
         else
         {
            syslog(LOG_MAIL | LOG_ERR,"invalid CONTENT_TYPE specification for "
			"request method post");
            user_env.r_method=cgi_not_specified;
            return(-1);
         }
      }
      else
      {
         syslog(LOG_MAIL | LOG_ERR,"no CONTENT_TYPE env variable for "
			"request method post");
         return(-1);
      }
   }

//if REQUEST_METHOD == post, get CONTENT_LENGTH

   if(user_env.r_method==cgi_post && getenv("CONTENT_LENGTH") != NULL)
      user_env.content_length=strtoul(getenv("CONTENT_LENGTH"),NULL,10);

//if REQUEST_METHOD == get, get QUERY_STRING

   if(user_env.r_method==cgi_get && getenv("QUERY_STRING") != NULL)
      if( (user_env.query_string=strdup(getenv("QUERY_STRING"))) == NULL)
      {
         syslog(LOG_MAIL | LOG_ERR,"Out of memory (%s)",strerror(errno));
         return(-1);
      }

//if REQUEST_METHOD == post/multipart

   if(user_env.r_method==cgi_post_multipart)
   {
      char* tmp_ptr=NULL;

      if( (tmp_ptr=getenv("CONTENT_TYPE")) == NULL)
      {
         syslog(LOG_MAIL | LOG_ERR,"no CONTENT_TYPE for cgi request post type "
			"multipart/form-data");
         return(-1);
      }

      if( (tmp_ptr=strchr(tmp_ptr,';')) == NULL)
      {
         syslog(LOG_MAIL | LOG_ERR,"no parameter (boundary) for request "
                        "method post type multipart/form-data");
         return(-1);
      }

      if( (tmp_ptr=strpcbrk(tmp_ptr+1," \t")) == NULL)
      {
         syslog(LOG_MAIL | LOG_ERR,"no parameter (boundary) for request "
			"method post type multipart/form-data");
         return(-1);
      }

      if(strncasecmp(tmp_ptr,"boundary",sizeof("boundary")-1) != 0)
      {
         syslog(LOG_MAIL | LOG_ERR,"no parameter (boundary) for request "
			"method post type multipart/form-data");
         return(-1);
      }

      if( (tmp_ptr=strchr(tmp_ptr,'=')) == NULL)
      {
         syslog(LOG_MAIL | LOG_ERR,"no boundary data for request method post "
			"type multipart/form-data");
         return(-1);
      }

      if( (user_env.boundary=strdup(tmp_ptr+1)) == NULL)
      {
         syslog(LOG_MAIL | LOG_ERR,"Out of memory (%s)",strerror(errno));
         return(-1);
      }
   }

   return(0);   
}

////////////////////////////////////////////////////////////////////////////

int destroy_user_env(void)
{
   if(user_env.server_name != NULL)
   {
      free(user_env.server_name);
      user_env.server_name=NULL;
   }

   if(user_env.server_addr != NULL)
   {
      free(user_env.server_addr);
      user_env.server_addr=NULL;
   }

   if(user_env.remote_addr != NULL)
   {
      free(user_env.remote_addr);
      user_env.remote_addr=NULL;
   }

   if(user_env.doc_root != NULL)
   {
      free(user_env.doc_root);
      user_env.doc_root=NULL;
   }

   if(user_env.script_name != NULL)
   {
      free(user_env.script_name); 
      user_env.script_name=NULL;
   }

   if(user_env.query_string != NULL)
   {
      free(user_env.query_string);
      user_env.query_string=NULL;
   }

   if(user_env.boundary != NULL)
   {
      free(user_env.boundary);
      user_env.boundary=NULL;
   }

   return(0);
}

////////////////////////////////////////////////////////////////////////////





