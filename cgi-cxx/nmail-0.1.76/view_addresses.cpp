/*
	view_addresses.cpp
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <errno.h>

#include "headers.h"
#include "session.h"
#include "main.h"
#include "parse_form.h"
#include "err_page.h"
#include "preferences.h"
#include "addr.h"

typedef enum addr_field { af_none, af_to, af_cc, af_bcc } addr_f;

int view_addresses_get_cgi(addr_f*, char**);
int display_address_page(addr_f, char*);
int parse_addresses(char**);
int print_address(addr_t*, int);
int display_name_html(addr_t* addr);
int validate_addresses(addr_t*, addr_t*);

/////////////////////////////////////////////////////////////////////

int view_addresses_action(void)
{
   addr_f field=af_none;
   char* preexisting=NULL;

   if(view_addresses_get_cgi(&field, &preexisting) != 0)
   {
      err_page();
      return(-1);
   }

   content_html();
   html_header("Add Addresses");

   display_address_page(field,preexisting);

   html_footer();

   return(0);
}

/////////////////////////////////////////////////////////////////////

int view_addresses_get_cgi(addr_f* field, char** preexisting)
{
   cgi_t* cgi_data=NULL;
   *field=af_none;
   *preexisting=NULL;

   if(preexisting == NULL)
      return(-1);

   while(get_cgi_data(&cgi_data) == 0)
   {
      if(strcmp(cgi_data->name,"field") == 0 && cgi_data->value != NULL)
      {
         if(strcmp(cgi_data->value,"to") == 0)
            *field=af_to;
         else if(strcmp(cgi_data->value,"cc") == 0)
            *field=af_cc;
         else if(strcmp(cgi_data->value,"bcc") == 0)
            *field=af_bcc;

         free(cgi_data->name);
         cgi_data->name=NULL;
         free(cgi_data->value);
         cgi_data->value=NULL;
      }
      else if(strcmp(cgi_data->name,"preexisting") == 0 && 
	 *preexisting == NULL &&
         cgi_data->value != NULL)
      {
         *preexisting=cgi_data->value;
         cgi_data->value=NULL;
         free(cgi_data->name);
         cgi_data->name=NULL;
      }
   }

   if(*field == af_none)
      return(-1);

   return(0);
}

/////////////////////////////////////////////////////////////////////

int display_address_page(addr_f field, 
			char* preexisting)
{
   printf("<FORM METHOD=\"POST\" ACTION=\"javascript:update();\" "
	  "ENCTYPE=\"application/x-www-form-urlencoded\" "
	  "NAME=\"addressbook\">\n\n"
	  "<TABLE BORDER=\"0\" ALIGN=\"CENTER\" WIDTH=\"90%%\" "
	  "CELLPADDING=\"1\" CELLSPACING=\"1\">\n"
	  "   <TR BGCOLOR=\"#002266\">\n"
	  "      <TD COLSPAN=\"2\" ALIGN=\"LEFT\">\n"
	  "         <FONT COLOR=\"#FFFFFF\" FACE=\"Arial, Helvetica\" "
	  "SIZE=\"3\">\n"
	  "         <B>");

   if(field == af_to)
      printf("To:");
   else if(field == af_cc)
      printf("cc:");
   else if(field == af_bcc)
      printf("bcc:");

   printf(" Address Book</B></FONT>\n"
	  "      </TD>\n"
	  "   </TR>\n");

   parse_addresses(&preexisting);

   printf("   <TR BGCOLOR=\"DDDDDD\">\n"
	  "      <TD ALIGN=\"CENTER\" COLSPAN=\"2\">\n"
          "         <INPUT TYPE=\"submit\" NAME=\"mailto.x\" "
	  "VALUE=\"OK\"> &nbsp; &nbsp;\n"
	  "         <INPUT TYPE=\"button\" VALUE=\"Cancel\" "
	  "onClick=\"window.close();\">\n"
	  "      </TD>\n"
	  "   </TR>\n"
	  "</TABLE>\n"
	  "</FORM>\n\n"
	  "<SCRIPT LANGUAGE=\"javascript\">\n"
	  "<!--\n"
	  "   function update()\n"
	  "   {\n"
	  "      var e2=\"\";\n"
	  "      for (var i = 0; i < document.addressbook.elements.length;  "
	  "i++)\n"
	  "      {\n"
	  "         var e = document.addressbook.elements[i];\n"
	  "         if (e.name == \"to\" && e.checked)\n"
	  "         {\n"
	  "            if (e2)\n"
	  "               e2+=\", \";\n"
	  "            e2+=e.value;\n"
	  "         }\n"
	  "      }\n"
	  "      window.opener.document.composeform.");

   if(field == af_to)
      printf("to");
   else if(field == af_cc)
      printf("cc");
   else if(field == af_bcc)
      printf("bcc");

   printf(".value = e2;\n"
	  "      window.close();\n"
	  "   }\n"
	  "//-->\n"
	  "</SCRIPT>\n");

   return(0);
}

/////////////////////////////////////////////////////////////////////

int parse_addresses(char** preexisting)
{
   int even_odd=0;
   int x=0;
   int y=0;
   int flag=0;

   addr_t* addr=NULL;

   addr=parse_addr(preexisting);
   validate_addresses(addr,user_pref.addr);

   flag=0;

   if(addr != NULL)
      for(y=0; addr[y].name || addr[y].email; y++)
      {
         if( ((addr[y].status)&0x04) == 0)
            flag|=0x01;
      }

   if(user_pref.addr != NULL)
      for(x=0; user_pref.addr[x].name || user_pref.addr[x].email; x++)
         print_address(&(user_pref.addr[x]),even_odd++);

   if(flag&0x01)
   {
      printf("   <TR BGCOLOR=\"#002266\">\n"
	     "      <TD COLSPAN=\"2\" ALIGN=\"LEFT\">\n"
	     "         <FONT COLOR=\"#FFFFFF\" FACE=\"Arial, Helvetica\" "
	     "SIZE=\"3\">\n"
	     "         <B>Other Addresses</B></FONT>\n"
	     "      </TD>\n"
	     "   </TR>\n");

      for(y=0; addr[y].name || addr[y].email; y++)
      {
         if(((addr[y].status)&0x04) == 0)
            print_address(&(addr[y]),even_odd++);
      }
   }

   return(0);
}

/////////////////////////////////////////////////////////////////////

int validate_addresses(addr_t* addr, addr_t* prefs)
{
   int x_size=0;
   int y_size=0;
   int x=0;
   int y=0;

   if(prefs != NULL)
      for(x_size=0; prefs[x_size].name || prefs[x_size].email; x_size++)
         prefs[x_size].status=0x01;

   if(addr != NULL)
      for(y_size=0; addr[y_size].name || addr[y_size].email; y_size++)
         addr[y_size].status=0x02;

   if(addr == NULL || prefs == NULL)
   {
      if(addr != NULL)
         for(y=0; addr[y].name || addr[y].email; y++)
            if(addr[y].email == NULL)
               addr[y].status|=0x80;

      return(0);
   }

   for(y=0; y<y_size; y++)
   {
      for(x=0; x<x_size; x++)
      {
         if(addr[y].email == NULL)
         {
            if(addr[y].name != NULL && prefs[x].name != NULL)
            {
               if(strcasecmp(addr[y].name,prefs[x].name) == 0)
               {
                  addr[y].status|=0x04;
                  prefs[x].status|=0x02;
                  break;
               }
            }
         }
         else
         {
            if(prefs[x].email != NULL)
            {
               if(strcasecmp(addr[y].email,prefs[x].email) == 0)
               {
                  addr[y].status|=0x04;
                  prefs[x].status|=0x02;
                  break;
               }
            }
         }
      }

      /* If email null and didn't find name, status == error */
      if(addr[y].email == NULL && ((addr[y].status)&0x04) == 0)
      {
         addr[y].status|=0x08;
      }
   }

   return(0);
}

/////////////////////////////////////////////////////////////////////

int print_address(addr_t* addr, int even_odd)
{
   if(addr == NULL)
      return(-1);

   printf("   <TR BGCOLOR=\"#%s\">\n",even_odd%2 == 0 ?
			"DDDDDD" : "EEEEEE");

   if( (addr->status & 0x08) != 0)
   {
      printf("      <TD WIDTH=\"20\">\n"
             "         &nbsp;\n"
             "      </TD>\n"
             "      <TD WIDTH=\"100%%\">\n"
             "         <FONT COLOR=\"#FF0000\"><B>\n"
             "            INVALID: ");
      if(addr->name != NULL)
         printf("&#34;%s&#34;",addr->name);
      if(addr->email != NULL)
         printf("&lt;%s&gt;",addr->email);

      printf("\n        </B></FONT>\n"
             "      </TD>\n");
   }
   else
   {
      printf("      <TD WIDTH=\"20\">\n");
      printf("         <INPUT TYPE=\"checkbox\" NAME=\"to\" VALUE=\"");
      display_name_html(addr);
      printf("\"");

      if(addr->status&0x02)
         printf("CHECKED");

      printf(">\n"
             "      </TD>\n"
             "      <TD WIDTH=\"100%%\">\n");
      if(addr->name != NULL)
         printf("         %s\n",addr->name);
      else if(addr->email != NULL)
         printf("         %s\n",addr->email);
      printf("      </TD>\n");
   }

   printf("   </TR>\n");

   return(0);
}

/////////////////////////////////////////////////////////////////////

int display_name_html(addr_t* addr)
{
   if(addr == NULL)
      return(-1);

   if(addr->status&0x01 != 0)
   {
      if(addr->name != NULL)
      {
         if(strchr(addr->name,',') != NULL)
            printf("&#34;%s&#34;",addr->name);
         else
            printf("%s",addr->name);
      }
      else if(addr->email != NULL)
         printf("%s",addr->email);
   }
   else
   {
      if(addr->name != NULL)
      {
         if(strchr(addr->name,',') != 0)
            printf("&#34;%s&#34;",addr->name);
         else
            printf("%s",addr->name);

         if(addr->email != NULL)
            printf(" &lt;%s&gt;",addr->email);
      }
      else if(addr->email != NULL)
         printf("%s",addr->email);
   }

   return(0);
}

/////////////////////////////////////////////////////////////////////


