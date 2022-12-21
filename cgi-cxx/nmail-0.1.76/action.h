/*
	action.h
*/

#ifndef _ACTION_H
#define _ACTION_H


typedef enum web_action {
   none,
   login,
   logout,

   view_inbox,
   view_message,
   view_attachment,

   delete_message,
   delete_messages,
 
   view_addr_book,
   edit_addr_book,
   view_addresses,
   import_addr_book,
   do_import_addr,
   export_addr_book,

   edit_pref,
   save_pref,

   send_message,

   compose_message,
   reply_message,
   reply_all_message,
   forward_message

} web_action_t;

extern web_action_t mail_action;

int find_action_and_session(void);

#endif   
