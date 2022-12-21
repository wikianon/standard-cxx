/*
	import_addr_page.cpp
*/

#include <stdio.h>

#include "headers.h"
#include "session.h"
#include "environment.h"

////////////////////////////////////////////////////////////////////

int import_addr_page(void)
{
   content_html();
   html_header("Import address book");

   printf("<FORM METHOD=\"POST\" ENCTYPE=\"multipart/form-data\" "
	  "NAME=\"import\">\n"
	  "   <INPUT TYPE=\"hidden\" NAME=\"action\" "
	  "VALUE=\"do_import_addr\">\n"
	  "   <INPUT TYPE=\"hidden\" NAME=\"sessionid\" "
	  "VALUE=\"%s\">\n\n",
		user_ses.sessionid);
   printf("<TABLE BORDER=\"0\" ALIGN=\"CENTER\" WIDTH=\"90%%\" "
	  "CELLPADDING=\"1\" CELLSPACING=\"1\">\n"
	  "   <TR>\n"
	  "      <TD BGCOLOR=\"#002266\" ALIGN=\"LEFT\">\n"
	  "         <FONT COLOR=\"#FFFFFF\" FACE=\"Arial, Helvetica\" "
	  "SIZE=\"3\">\n"
	  "         <B>Import Address Book</B></FONT>\n"
	  "      </TD>\n"
	  "   </TR>\n");
   printf("   <TR BGCOLOR=\"#DCDCDC\">\n"
	  "      <TD>\n"
	  "         <TABLE BORDER=\"0\">\n"
	  "            <TR>\n"
	  "               <TD ALIGN=\"LEFT\">\n"
	  "                  <A HREF=\"%s?action=view_inbox&sessionid=%s\">\n"
	  "                  Inbox</A> | \n",
		user_env.script_name,user_ses.sessionid);
   printf("                  <A "
	  "HREF=\"%s?action=view_addr_book&sessionid=%s\">\n"
	  "                  Address Book</A> | \n",
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
	  "         Select the file you wish to import.\n"
	  "      </TD>\n"
	  "   </TR>\n"
	  "   <TR BGCOLOR=\"#EEEEEE\">\n"
	  "      <TD>\n"
	  "         <BR>Recognized file types are:\n"
	  "         <DD>*.csv (Outlook Express)<BR>\n"
	  "         <DD>*.ldif (Netscape)<BR><BR>\n"
	  "         <INPUT TYPE=\"file\" NAME=\"attachment\" SIZE=\"50\">\n"
	  "      </TD>\n"
	  "   </TR>\n"
	  "   <TR>\n"
	  "      <TD>\n"
	  "         <INPUT TYPE=\"submit\" VALUE=\"Import\">\n"
	  "      </TD>\n"
	  "   </TR>\n"
	  "</TABLE>\n\n"
	  "</FORM>\n");




  html_footer();

   return(0);
}

////////////////////////////////////////////////////////////////////

