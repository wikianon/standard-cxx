/*
	do_import_addr.cpp
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>

#include "addr.h"
#include "headers.h"
#include "err_page.h"
#include "parse_form.h"
#include "edit_addr_book_page.h"
#include "conv_csv.h"
#include "conv_ldif.h"
#include "preferences.h"
#include "environment.h"
#include "session.h"

int import_addr_book_err(char*, addr_t*);
int import_addresses(addr_t*);
int import_addr_success_page(addr_t*);

//////////////////////////////////////////////////////////////////

int do_import_addr_action(void)
{
   addr_t* addr=NULL;
   char* tmp_ptr=NULL;
   char* filename=NULL;
   FILE* import_file=NULL;

   cgi_t* cgi_data=NULL;

   while(get_cgi_data(&cgi_data) == 0)
   {
      if(strcmp(cgi_data->name,"attachment") == 0)
      {
         filename=cgi_data->filename;
         import_file=cgi_data->tmp_file;
      }
   }

   if(import_file == NULL || 
      cgi_data->filename == NULL || 
      *(cgi_data->filename) == '\0')
   {
      view_addr_book_action();
      return(-1);
   }

   tmp_ptr=strrchr(filename,'.');

   if(tmp_ptr == NULL)
   {
      import_addr_book_err(filename, addr);
      return(-1);
   }

   if(strcasecmp(tmp_ptr,".csv") == 0)
   {
      if(convert_from_csv(import_file, &addr) != 0)
      {
         import_addr_book_err(NULL,addr);
         return(-1);
      }

   }
   else if(strcasecmp(tmp_ptr,".ldif") == 0)
   {
      if(convert_from_ldif(import_file, &addr) != 0)
      {
         import_addr_book_err(NULL,addr);
         return(-1);
      }
   }
   else
   {
      import_addr_book_err(filename,addr); 
      return(-1);
   }


   if(addr == NULL)
   {
      import_addr_book_err(NULL,addr);
      return(-1);
   }

   import_addresses(addr);

   return(0);
}


//////////////////////////////////////////////////////////////////

int import_addr_book_err(char* filename, addr_t* addr)
{
   char* tmp_ptr=NULL;

   content_html();
   html_header("ERROR: bad import file");

   printf("<TABLE BORDER=\"0\" WIDTH=\"80%%\" ALIGN=\"CENTER\">\n"
	  "   <TR BGCOLOR=\"#002266\">\n"
	  "      <TD>\n"
	  "         <FONT COLOR=\"#FFFFFF\" FACE=\"Arial, Helvetica\" "
	  "SIZE=\"3\">\n"
	  "         <B>Error during address import</B></FONT>\n"
	  "      </TD>\n"
	  "   </TR>\n"
	  "   <TR BGCOLOR=\"#EEEEEE\">\n"
	  "      <TD>\n"
	  "         <BR>\n");

   if(filename != NULL)
   {
      tmp_ptr=strrchr(filename,'.');

      if(tmp_ptr == NULL)
      {
         printf("Could not determine file type due to extension.<BR>\n");
      }
      else if(strcasecmp(tmp_ptr,".csv") != 0 && 
		strcasecmp(tmp_ptr,".ldif") != 0)
      {
         printf("Unsupported file type *%s<BR>\n",tmp_ptr);
      }
   }

   if(addr == NULL)
   {
      printf("No addresses to import.<BR>\n");
   }

   printf("\n"
          "         <BR>\n"
	  "      </TD>\n"
	  "   </TR>\n"
	  "</TABLE>\n");   


   html_footer();

   return(0);
}

//////////////////////////////////////////////////////////////////

int import_addresses(addr_t* addr)
{
   FILE* pref_file=NULL;
   int status=0;
   int x=0;

   if(addr == NULL)
   {
      err_string_page("No addresses to import");
      return(-1);
   }

   pref_file=open_user_pref_file("r+",&status);

   if( pref_file==NULL && status==0 )
      pref_file=open_user_pref_file("w+",NULL);

   if(pref_file == NULL)
   {
      err_page();
      return(-1);
   }

   for(x=0; addr[x].name || addr[x].email; x++)
   {
      if(valid_email(addr[x].email) == 0)
         addr[x].status=0x01;
      else
      {
         if( add_pref_email(pref_file,addr[x].name,addr[x].email) == 0)
            addr[x].status=0;
         else
            addr[x].status=0x02;
      }
   }

   import_addr_success_page(addr);
  
   return(0);
}

//////////////////////////////////////////////////////////////////

int import_addr_success_page(addr_t* addr)
{
   int x=0;
   int err=0;

   for(x=0; addr[x].name || addr[x].email; x++)
      if(addr[x].status)
         err=-1;

   content_html();

   if(err == 0)
      html_header("Import Address Book - Success");
   else
      html_header("Import Address Book - Errors");

   printf("<TABLE BORDER=\"0\" WIDTH=\"90%%\" ALIGN=\"CENTER\">\n"
	  "   <TR BGCOLOR=\"#002266\">\n"
	  "      <TD>\n"
	  "         <FONT COLOR=\"#FFFFFF\" SIZE=\"3\">\n"
	  "         <B>Import Address Book - Status</B></FONT>\n"
	  "      </TD>\n"
	  "   </TR>\n"
	  "   <TR BGCOLOR=\"#DCDCDC\">\n"
	  "      <TD>\n"
	  "         <TABLE BORDER=\"0\">\n"
	  "            <TR>\n"
	  "               <TD>\n");

   printf("                  <A HREF=\"%s?action=view_inbox&sessionid=%s\">\n"
	  "                  Inbox</A> | \n",
			user_env.script_name,user_ses.sessionid);
   printf("                  <A "
	  "HREF=\"%s?action=view_addr_book&sessionid=%s\">\n"
	  "                  Address Book</A> | \n",
			user_env.script_name,user_ses.sessionid);
   printf("                  <A HREF=\"%s?action=logout&sessionid=%s\">\n"
	  "                  Logout\n",
			user_env.script_name,user_ses.sessionid);

   printf("               </TD>\n"
	  "            </TR>\n"
	  "         </TABLE>\n"
	  "      </TD>\n"
	  "   </TR>\n"
	  "   <TR>\n"
	  "      <TD>\n"
	  "         <BR>\n"
	  "      </TD>\n"
	  "   </TR>\n"
	  "   <TR BGCOLOR=\"#DDDDDD\">\n"
	  "      <TD>\n"
	  "         <B>Addresses Imported Successfully</B>\n"
	  "      </TD>\n"
	  "   </TR>\n"
	  "   <TR BGCOLOR=\"#EEEEEE\">\n"
	  "      <TD>\n");

   for(x=0; addr[x].name || addr[x].email; x++)
   {
      if(addr[x].status == 0)
      {
         if(addr[x].name != NULL)
            printf("        \"%s\" ",addr[x].name);

         if(addr[x].email != NULL)
            printf("&lt;<B>%s</B>&gt;",addr[x].email);

         printf("<BR>\n");
      }
   }

   printf("      </TD>\n"
	  "   </TR>\n");


   if(err != 0)
   {
      printf("   <TR>\n"
	     "      <TD>\n"
	     "         <BR>\n"
	     "      </TD>\n"
	     "   </TR>\n"
	     "   <TR BGCOLOR=\"#DDDDDD\">\n"
	     "      <TD>\n"
	     "         <B>Error: Addresses NOT imported</B>\n"
	     "      </TD>\n"
	     "   </TR>\n"
	     "   <TR BGCOLOR=\"#EEEEEE\">\n"
	     "      <TD>\n");

      for(x=0; addr[x].name || addr[x].email; x++)
      {
         if(addr[x].status != 0)
         {
            if(addr[x].name != NULL)
               printf("\"%s\" ",addr[x].name);
            if(addr[x].email != NULL)
               printf("&lt;%s&gt;",addr[x].email);
            printf("<BR>\n");
         }
      }

      printf("      </TD>\n"
	     "   </TR>\n");
   }

   printf("</TABLE>\n");
   html_footer();


   return(0);
}

//////////////////////////////////////////////////////////////////


