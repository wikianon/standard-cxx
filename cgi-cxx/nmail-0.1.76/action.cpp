/*
	action.cpp
*/


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <syslog.h>
#include <string.h>

#include "action.h"
#include "session.h"
#include "parse_form.h"

web_action_t mail_action=none;

web_action_t parse_action(char*);

////////////////////////////////////////////////////////////////////////////
// This finds the action and sessionid in the cgi name/value pairs

int find_action_and_session(void)
{
   cgi_t* cgi_data=NULL;

   while(get_cgi_data(&cgi_data) == 0)
   {
      if(strcmp(cgi_data->name,"action") == 0 )
      {
         mail_action=parse_action(cgi_data->value);
         free(cgi_data->name);
         cgi_data->name=NULL;
         free(cgi_data->value);
         cgi_data->value=NULL;
      }

      else if(strcmp(cgi_data->name,"sessionid") == 0 && 
              user_ses.sessionid == NULL &&
              cgi_data->value != NULL)
      {
         user_ses.sessionid=cgi_data->value;
         cgi_data->value=NULL;
         free(cgi_data->name);
         cgi_data->name=NULL;
      }
   }

   cgi_rewind();

   return(0);
}

////////////////////////////////////////////////////////////////////////////
// This sorts out the action and determines what page the user is requesting

web_action_t parse_action(char* action)
{
   char x=0;

   if(action == NULL)
      return(none);

   struct web_mail_action {
      char* label;
      web_action action;
   };

   web_mail_action mail_action[]= {
      { "login", login },
      { "logout", logout },

      { "view_inbox", view_inbox },
      { "view_message", view_message },
      { "view_attachment", view_attachment },

      { "delete_message", delete_message },
      { "delete_messages", delete_messages },

      { "view_addr_book", view_addr_book },
      { "edit_addr_book", edit_addr_book },
      { "view_addresses", view_addresses },
      { "import_addr_book", import_addr_book },
      { "do_import_addr", do_import_addr },
      { "export_addr_book", export_addr_book },

      { "edit_pref", edit_pref },
      { "save_pref", save_pref },

      { "compose_message", compose_message },
      { "reply_message", reply_message },
      { "reply_all_message", reply_all_message },
      { "forward_message", forward_message },

      { "send_message", send_message },

      { NULL, none }
   };

   x=0;

   while(mail_action[x].label != NULL)
   {
      if (strcasecmp(action, mail_action[x].label) == 0 )
         return(mail_action[x].action);

      x++;
   }

   return(none);
}

////////////////////////////////////////////////////////////////////////////

