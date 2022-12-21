/*
	edit_addr_book.cpp
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <syslog.h>

#include "parse_form.h"
#include "session.h"
#include "main.h"
#include "edit_addr_book_page.h"
#include "err_page.h"
#include "preferences.h"
#include "mail_process.h"

enum edit_addr_subaction { eas_add, eas_modify, eas_delete, eas_none };

struct edit_addr_struct {
   char* modify_name;
   char* modify_email;
   char* real_name;
   char* real_email;
   edit_addr_subaction eas_subaction;
};

int get_edit_addr_struct(edit_addr_struct*);
int parse_edit_addr(edit_addr_struct*);
int free_edit_addr_struct(edit_addr_struct*);

//////////////////////////////////////////////////////////////////

int edit_addr_book_action(void)
{
   int err=0;
   edit_addr_struct edit_addr;

   get_edit_addr_struct(&edit_addr);
   err=parse_edit_addr(&edit_addr);
   free_edit_addr_struct(&edit_addr);

   destroy_user_pref();
   get_user_pref();

   if(err == 0)
      view_addr_book_action();
   else if(err == 1)
      err_string_page("Invalid email address");
   else
   {
      syslog(LOG_MAIL | LOG_ERR,"error editing address book for %s",
			user_ses.loginname);
      err_page();
   }

   return(0);
}

//////////////////////////////////////////////////////////////////
// This parses the cgi values and finds what name/email to add
//    or modify

int get_edit_addr_struct(edit_addr_struct* edit_addr)
{
   cgi_t* cgi_data;

   edit_addr->modify_name=NULL;
   edit_addr->modify_email=NULL;
   edit_addr->real_name=NULL;
   edit_addr->real_email=NULL;
   edit_addr->eas_subaction=eas_none;


   while(get_cgi_data(&cgi_data) == 0)
   {
      if(strcmp(cgi_data->name,"subaction") == 0 && cgi_data->value != NULL)
      {
         if(strcmp(cgi_data->value,"add") == 0)
            edit_addr->eas_subaction=eas_add;
         else if(strcmp(cgi_data->value,"modify") == 0)
            edit_addr->eas_subaction=eas_modify;
         else if(strcmp(cgi_data->value,"delete") == 0)
            edit_addr->eas_subaction=eas_delete;
         else
            edit_addr->eas_subaction=eas_none;
         free(cgi_data->name);
         cgi_data->name=NULL;
         free(cgi_data->value);
         cgi_data->value=NULL;
      }
      else if(strcmp(cgi_data->name,"modifyname") == 0)
      {
         if(edit_addr->modify_name == NULL && cgi_data->value != NULL)
         {
            edit_addr->modify_name=cgi_data->value;
            cgi_data->value=NULL;
            free(cgi_data->name);
            cgi_data->name=NULL;
         }
      }
      else if(strcmp(cgi_data->name,"modifyemail") == 0)
      {
         if(edit_addr->modify_email == NULL && cgi_data->value != NULL)
         {
            edit_addr->modify_email=cgi_data->value;
            cgi_data->value=NULL;
            free(cgi_data->name);
            cgi_data->name=NULL;
         }
      }
      else if(strcmp(cgi_data->name,"realname") == 0)
      {
         if(edit_addr->real_name == NULL && cgi_data->value != NULL)
         {
            edit_addr->real_name=cgi_data->value;
            cgi_data->value=NULL;
            free(cgi_data->name);
            cgi_data->name=NULL;
         }
      }
      else if(strcmp(cgi_data->name,"realemail") == 0)
      {
         if(edit_addr->real_email == NULL && cgi_data->value != NULL)
         {
            edit_addr->real_email=cgi_data->value;
            cgi_data->value=NULL;
            free(cgi_data->name);
            cgi_data->name=NULL;
         }
      }
   }

   edit_addr->modify_name=eliminate_lwsp(edit_addr->modify_name);
   edit_addr->modify_email=eliminate_lwsp(edit_addr->modify_email);
   edit_addr->real_name=eliminate_lwsp(edit_addr->real_name);
   edit_addr->real_email=eliminate_lwsp(edit_addr->real_email);

   return(0);
}

//////////////////////////////////////////////////////////////////
// This free's the resources needed for the edit address structure

int free_edit_addr_struct(edit_addr_struct* edit_addr)
{
   if(edit_addr == NULL)
      return(-1);

   if(edit_addr->modify_name != NULL)
   {
      free(edit_addr->modify_name);
      edit_addr->modify_name=NULL;
   }

   if(edit_addr->modify_email != NULL)
   {
      free(edit_addr->modify_email);
      edit_addr->modify_email=NULL;
   }

   if(edit_addr->real_name != NULL)
   {
      free(edit_addr->real_name);
      edit_addr->real_name=NULL;
   }

   if(edit_addr->real_email != NULL)
   {
      free(edit_addr->real_email);
      edit_addr->real_email=NULL;
   }

   return(0);
}

//////////////////////////////////////////////////////////////////
// This will open the user's preference file and add/modify/delete
//    the appropriate address

int parse_edit_addr(edit_addr_struct* edit_addr)
{
   int err=0;
   FILE* user_pref_file=NULL;

   user_pref_file=open_user_pref_file("r+",&err);

   if(user_pref_file == NULL && err == 0)
      user_pref_file=open_user_pref_file("w+",NULL);

   if(user_pref_file == NULL)
   {
      syslog(LOG_MAIL | LOG_ERR,"Could not open pref file for writing");
      return(-1);
   }
 
   if(edit_addr->eas_subaction == eas_none)
   {
      fclose(user_pref_file);
      return(-1);
   }


   if(edit_addr->eas_subaction == eas_add)
   {
      if(edit_addr->real_email == NULL)
      {
         fclose(user_pref_file);
         return(-1);
      }

      if(valid_email(edit_addr->real_email) == 0)
         return(1);

      if(add_pref_email(user_pref_file,
		edit_addr->real_name,
		edit_addr->real_email) != 0)
      {
         syslog(LOG_MAIL | LOG_ERR,"add pref email failed");
         fclose(user_pref_file);
         return(-1);
      }
   }

   else if(edit_addr->eas_subaction == eas_delete)
   {
      
      if(edit_addr->modify_email == NULL)
      {
         syslog(LOG_MAIL | LOG_INFO,"delete email null");
         fclose(user_pref_file);
         return(-1);
      }

      if(delete_pref_email(user_pref_file,
		edit_addr->modify_name,
		edit_addr->modify_email) != 0)
      {
         syslog(LOG_MAIL | LOG_ERR,"delete pref email failed");
         fclose(user_pref_file);
         return(-1);
      }
   }

   else if(edit_addr->eas_subaction == eas_modify)
   {
      if(edit_addr->modify_email == NULL ||
		edit_addr->real_email == NULL)
      {
         fclose(user_pref_file);
         return(-1);
      }

      if(valid_email(edit_addr->real_email) == 0)
         return(1);

      if(modify_pref_email(user_pref_file,
		edit_addr->modify_name,
		edit_addr->modify_email,
		edit_addr->real_name,
		edit_addr->real_email) != 0)
      {
         syslog(LOG_MAIL | LOG_ERR,"modify pref email failed");
         free(user_pref_file);
         return(-1);
      }
   }

   fclose(user_pref_file);

   return(0);
}

//////////////////////////////////////////////////////////////////



