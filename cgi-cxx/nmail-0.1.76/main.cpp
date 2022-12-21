/*
	main.cpp
*/


#include <stdio.h>
#include <stdlib.h>	//getenv,free 
#include <string.h>
#include <unistd.h>	//getuid
#include <syslog.h>
#include <sys/stat.h>

#include "main.h"
#include "headers.h"
#include "environment.h"
#include "parse_form.h"
#include "login_authenticate.h"
#include "session.h"

#include "login_page.h"
#include "inbox_page.h"
#include "logout_page.h"
#include "err_page.h"
#include "action.h"
#include "view_message.h"
#include "view_attachment.h"
#include "delete_message.h"
#include "edit_addr_book_page.h"
#include "edit_addr_book.h"
#include "view_addresses.h"
#include "compose_message_page.h"
#include "send_message.h"
#include "config.h"
#include "preferences.h"
#include "import_addr_page.h"
#include "do_import_addr.h"
#include "export_addr.h"
#include "edit_pref_page.h"

int init_global_variables(void);
int parse_action(enum web_action);

int command_line(void);
int nmail(void);

uid_t ruid;
uid_t euid;

gid_t rgid;
gid_t egid;

////////////////////////////////////////////////////////////////////////////
// Startup - disable any special privledges, then see if called from
//    command line or from http server and service request appropriately

int main(void)
{
   int err=0;

   ruid=getuid();
   euid=geteuid();
   rgid=getgid();
   egid=getegid();

   setuid(ruid);
   setgid(rgid);

   umask(0777);


   if(getenv("SCRIPT_NAME") == NULL)
      err=command_line();
   else
      err=nmail();

   return(err);
}

////////////////////////////////////////////////////////////////////////////
// This gets run when nmail is called from the command line. Things to 
//    add here later include checking of permissions on directories and
//    reporting the status as such and how to fix. For now, just report 
//    the version.

int command_line(void)
{
   printf("NMAIL  version " VERSION "\n\n");

   return(0);
}

////////////////////////////////////////////////////////////////////////////
// This gets run when the program is invoked from a http server

int nmail(void)
{
   int err=0;

   /* Don't allow SUID root */
   if(ruid == 0 || euid == 0)
   {
      syslog(LOG_MAIL | LOG_ERR,"nmail will not allow you to run as root");
      err_page();
      return(EXIT_FAILURE);
   }

   init_global_variables();

   /* get environment variables */
   if(init_user_env() != 0)
   {
      syslog(LOG_MAIL | LOG_ERR,"problems with environment");
      err_page();
      return(EXIT_FAILURE);
   }

   /* read in and separate cgi stuff */
   if(init_cgi_env() != 0)
   {
      syslog(LOG_MAIL | LOG_ERR,"problems with cgi stuff");
      err_page();
      return(EXIT_FAILURE);
   }

   /* get action and sessionid */
   find_action_and_session();

   if(mail_action == none)
   {
      login_page();
      destroy_user_env();
      destroy_cgi_env();
      return(EXIT_SUCCESS);
   }

   err=login_authenticate();
         
   if(err != 0)
   {
      if(err == 1)
         err_bad_user_page();
      else if(err == 2)
         err_timeout_page();
      else
         err_page();

      destroy_user_env();
      destroy_cgi_env();
      destroy_user_ses();

      return(EXIT_FAILURE);
   }

   get_user_pref();

   parse_action(mail_action);

   destroy_user_env();
   destroy_cgi_env();
   destroy_user_ses();
   destroy_user_pref();

   return(EXIT_SUCCESS);
}

////////////////////////////////////////////////////////////////////////////

int init_global_variables(void)
{
   init_user_ses();
   init_user_pref();

   return(0);
}

////////////////////////////////////////////////////////////////////////////
// This calls the appropriate function to service the users request
//    (ie: viewing inbox, viewing message, saving preferences, ...)

int parse_action(enum web_action mail_action)
{
   switch (mail_action)
   {
      case login:
      case view_inbox:
         inbox_action();
         break;
      case logout:
         logout_action();
         break;
      case view_message:
         view_message_action();
         break;
      case view_attachment:
         view_attachment_action();
         break;
      case delete_message:
      case delete_messages:
         delete_msg();
         break;
      case view_addr_book:
         view_addr_book_action();
         break;
      case edit_addr_book:
         edit_addr_book_action();
         break;
      case import_addr_book:
         import_addr_page();
         break;
      case do_import_addr:
         do_import_addr_action();
         break;
      case export_addr_book:
         export_addr_action();
         break;
      case edit_pref:
         edit_pref_page();
         break;
      case save_pref:
         save_pref_action();
         break;
      case compose_message:
      case reply_message:
      case reply_all_message:
      case forward_message:
         compose_message_action();
         break;
      case view_addresses:
         view_addresses_action();
         break;
      case send_message:
         send_message_action();
         break;
      default:
         syslog(LOG_MAIL | LOG_ERR,"Action not found");
         err_page();
   }

   return(0);
}

////////////////////////////////////////////////////////////////////////////

