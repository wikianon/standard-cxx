/*
	edit_pref_page.cpp
*/

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <string.h>
#include <errno.h>

#include "headers.h"
#include "preferences.h"
#include "environment.h"
#include "mail_process.h"
#include "err_page.h"
#include "parse_form.h"

int get_cgi_save_pref(char**);

////////////////////////////////////////////////////////////

int edit_pref_page(void)
{
   content_html();
   html_header("Preferences");

   printf("<TABLE BORDER=\"0\" WIDTH=\"90%%\" ALIGN=\"CENTER\" "
	  "CELLPADDING=\"1\" CELLSPACING=\"1\">\n"
	  "   <TR BGCOLOR=\"#002266\">\n"
	  "      <TD>\n"
	  "         <FONT COLOR=\"#FFFFFF\" FACE=\"Arial, Helvetica\" "
	  "SIZE=\"3\">\n"
	  "         <B>Edit Preferences</B></FONT>\n"
	  "      </TD>\n"
	  "   </TR>\n");
   printf("   <TR BGCOLOR=\"DCDCDC\">\n"
	  "      <TD>\n"
	  "         <TABLE BORDER=\"0\">\n"
	  "            <TR>\n"
	  "               <TD>\n");

   printf("                  <A HREF=\"%s?action=view_inbox&sessionid=%s\">\n"
	  "                  Inbox</A> | \n",
			user_env.script_name,user_ses.sessionid);
   printf("                  <A HREF=\"%s?action=logout&sessionid=%s\">\n"
	  "                  Logout</A>\n",
			user_env.script_name,user_ses.sessionid);

   printf("               </TD>\n"
	  "            </TR>\n"
	  "         </TABLE>\n"
	  "      </TD>\n"
	  "   </TR>\n");

   printf("   <TR>\n"
	  "      <TD>\n"
	  "         <BR>\n"
	  "      </TD>\n"
	  "   </TR>\n"
	  "   <TR BGCOLOR=\"#DDDDDD\">\n"
	  "      <TD>\n"
	  "         Reply-To:\n"
	  "      </TD>\n"
	  "   </TR>\n");
   printf("   <TR BGCOLOR=\"#EEEEEE\">\n"
	  "      <TD><BR>\n"
	  "         ");

   if(user_pref.reply_to.name != NULL)
      printf("\"%s\" ",user_pref.reply_to.name);
   if(user_pref.reply_to.email != NULL)
      printf("&lt;%s&gt",user_pref.reply_to.email);
   printf("<BR>\n");

   printf("         <FORM METHOD=\"POST "
	  "ENCTYPE=\"application/x-www-form-urlencoded\" "
	  "NAME=\"reply_to_addr\" ACTION=\"%s\">\n",user_env.script_name);
   printf("         <INPUT TYPE=\"hidden\" NAME=\"action\" "
	  "VALUE=\"save_pref\">\n"
	  "         <INPUT TYPE=\"hidden\" NAME=\"sessionid\" "
	  "VALUE=\"%s\">\n",
			user_ses.sessionid);
	

   printf("         <INPUT TYPE=\"text\" NAME=\"name\" SIZE=\"30\" VALUE=\"");

   if(user_pref.reply_to.name != NULL)
      printf("%s",user_pref.reply_to.name);
   else if(user_ses.name != NULL)
      printf("%s",user_ses.name);
   else if(user_ses.loginname != NULL)
      printf("%s",user_ses.loginname);

   printf("\">\n");



   printf("      <INPUT TYPE=\"text\" NAME=\"email\" SIZE=\"30\" VALUE=\"");

   if(user_pref.reply_to.email != NULL)
      printf("%s",user_pref.reply_to.email);

   printf("\">\n");

   printf("<INPUT TYPE=\"SUBMIT\" VALUE=\"Update\">\n");

   printf("         </FORM>\n"
	  "      </TD>\n"
	  "   </TR>\n"
	  "</TABLE>\n");

   html_footer();

   return(0);
}

////////////////////////////////////////////////////////////

int save_pref_action(void)
{
   char* address=NULL;
   int err=0;

   err=get_cgi_save_pref(&address); 
  
   if(err == 1)
   {
      err_string_page("Invalid email address.");
      return(-1);
   } 
   else if(err != 0 || address == NULL)
   {
      err_page();
      return(-1);
   }

   err=save_pref_entry("reply_to",address);

   free(address);
   address=NULL;

   if(err != 0)
   {
      err_string_page("Error updating preferences.");
      return(-1);
   }

   destroy_user_pref();
   get_user_pref();

   edit_pref_page();

   return(0);
}

////////////////////////////////////////////////////////////

int get_cgi_save_pref(char** address)
{
   char* name=NULL;
   char* email=NULL;
   cgi_t* cgi_data=NULL;

   *address=NULL;

   while(get_cgi_data(&cgi_data) == 0)
   {
      if(strcmp(cgi_data->name,"name") == 0 )
      {
         if(name == NULL) 
         {
            name=cgi_data->value;
            cgi_data->value=NULL;
            free(cgi_data->name);
            cgi_data->name=NULL;

         }
      }
      else if(strcmp(cgi_data->name,"email") == 0)
      {
         if(email == NULL)
         {
            email=cgi_data->value;
            cgi_data->value=NULL;
            free(cgi_data->name);
            cgi_data->name=NULL;
         }
      }
   }

   if(name == NULL || email == NULL)
   {
      if(name != NULL)
      {
         free(name);
         name=NULL;
      }

      if(email != NULL)
      {
         free(email);
         email=NULL;
      }

      return(-1);
   }

   name=eliminate_lwsp(name);
   email=eliminate_lwsp(email);

   if(email == NULL || valid_email(email) == 0)
   {
      if(name != NULL)
         free(name);

      return(1);
   }

   if(name != NULL)
      asprintf(address,"\"%s\" <%s>",name,email);
   else
      asprintf(address,"\"\" <%s>",email);

   if(name != NULL)
   {
      free(name);
      name=NULL;
   }
   if(email != NULL)
   {
      free(email);
      email=NULL;
   }

   if(address == NULL)
      return(-1);

   return(0);
}

////////////////////////////////////////////////////////////


