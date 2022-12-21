/*
	edit_addr_book_page.cpp
*/

#include <stdio.h>

#include "preferences.h"
#include "main.h"
#include "headers.h"

int addr_book_upper_table(void);
int addr_book_names(void);
int addr_book_footer(void);

///////////////////////////////////////////////////////////

int view_addr_book_action(void)
{
   content_html();
   html_header("Address Book");

   addr_book_upper_table();
   addr_book_names();
   addr_book_footer();

   html_footer();
   return(0);
}

///////////////////////////////////////////////////////////

int addr_book_upper_table(void)
{

   printf("<FORM METHOD=\"POST\" "
          "ENCTYPE=\"application/x-www-form-urlencoded\" "
          "NAME=\"newaddress\">\n"
          "   <INPUT TYPE=\"hidden\" NAME=\"action\" "
          "VALUE=\"edit_addr_book\">\n"
          "   <INPUT TYPE=\"hidden\" NAME=\"sessionid\" "
		"VALUE=\"%s\">\n",user_ses.sessionid);
   printf("   <INPUT TYPE=\"hidden\" NAME=\"subaction\" "
          "VALUE=\"add\">\n"
          "   <INPUT TYPE=\"hidden\" NAME=\"modifyname\" "
          "VALUE=\"\">\n"
          "   <INPUT TYPE=\"hidden\" NAME=\"modifyemail\" "
          "VALUE=\"\">\n\n"
          "<TABLE ALIGN=\"CENTER\" WIDTH=\"90%%\" CELLPADDING=\"1\" "
          "CELLSPACING=\"1\" BORDER=\"0\">\n"
          "   <TR>\n"
          "      <TD COLSPAN=\"3\" BGCOLOR=\"#002266\" ALIGN=\"LEFT\">\n"
          "         <TABLE CELLPADDING=\"0\" "
          "CELLSPACING=\"0\" WIDTH=\"100%%\">\n"
          "            <TR>\n"
	  "               <TD>\n"
          "                  <FONT COLOR=\"#FFFFFF\" "
	  "FACE=\"Arial,Helvetica\" SIZE=\"3\">\n"
	  "                  <B>Edit Address Book</B></FONT>\n"
          "               </TD>\n"
          "            </TR>\n"
	  "         </TABLE>\n"
          "      </TD>\n"
          "   </TR>\n\n"
          "   <TR>\n"
          "      <TD COLSPAN=\"3\" BGCOLOR=\"#DCDCDC\" ALIGN=\"LEFT\">\n"
          "         <TABLE BORDER=\"0\">\n"
          "            <TR>\n"
          "               <TD>\n");
   printf("                  <A HREF=\"%s?action=view_inbox&sessionid=%s\">\n"
          "                  Inbox</A> | \n",
		user_env.script_name,user_ses.sessionid);
   printf("                  <A "
          "HREF=\"%s?action=import_addr_book&sessionid=%s\">\n"
          "                  Import Address Book</A> | \n",
		user_env.script_name,user_ses.sessionid);
   printf("                  <A "
	  "HREF=\"%s/addresses.csv?action=export_addr_book&sessionid=%s\">\n"
	  "                  Export Address Book</A> | \n",
		user_env.script_name,user_ses.sessionid);

   printf("                  <A "
	  "HREF=\"%s?action=logout&sessionid=%s\">\n"
	  "                  Logout\n",
		user_env.script_name,user_ses.sessionid);
   printf("               </TD>\n"
          "            </TR>\n"
          "         </TABLE>\n"
          "      </TD>\n"
          "   </TR>\n\n"
          "   <TR>\n"
          "      <TD>\n"
          "         <BR>\n"
          "      </TD>\n"
          "   </TR>\n\n"
          "   <TR BGCOLOR=\"#D3DCE3\">\n"
          "      <TD ALIGN=\"CENTER\">\n"
          "         <B>Name</B>\n"
          "      </TD>\n"
          "      <TD ALIGN=\"CENTER\">\n"
          "         <B>E-mail Address (click to edit)</B>\n"
          "      </TD>\n"
          "      <TD ALIGN=\"CENTER\">\n"
          "         <B>Action</B>\n"
          "      </TD>\n"
          "   </TR>\n\n"
          "   <TR BGCOLOR=\"#EEEEEE\">\n"
          "      <TD WIDTH=\"40%%\">\n"
          "         <INPUT TYPE=\"TEXT\" NAME=\"realname\" SIZE=\"30\">\n"
          "      </TD>"
          "      <TD WIDTH=\"40%%\">\n"
          "         <INPUT TYPE=\"TEXT\" NAME=\"realemail\" SIZE=\"30\">\n"
          "      </TD>\n"
          "      <TD ALIGN=\"CENTER\">\n"
          "         <INPUT TYPE=\"SUBMIT\" NAME=\"add_modify\" "
          "VALUE=\"  Add  \">\n"
          "         <INPUT TYPE=\"BUTTON\" NAME=\"clear\" VALUE=\"Clear\" "
          "ONCLICK=\"javascript:clear_boxes()\">\n"
          "      </TD>\n"
          "   </TR>\n");

   return(0);
}

///////////////////////////////////////////////////////////

int addr_book_names(void)
{
   int x=0;

   if(user_pref.addr == NULL)
      return(-1);

   for(x=0; user_pref.addr[x].email || user_pref.addr[x].name; x++)
   {
      printf("   <TR BGCOLOR=\"#%s\">\n",x%2 == 0 ? "DDDDDD" : "EEEEEE");
      printf("      <TD>\n"
             "         %s\n",user_pref.addr[x].name != NULL ?
				user_pref.addr[x].name :
				"&nbsp;");
      printf("      </TD>\n"
             "      <TD>\n"
             "         <A HREF=\"javascript:modify_name('%s','%s')\">\n"
             "            %s</A>\n",
			user_pref.addr[x].name != NULL ?
				user_pref.addr[x].name :
				"",
			user_pref.addr[x].email,
			user_pref.addr[x].email);
      printf("      </TD>\n"
             "      <TD ALIGN=\"CENTER\">\n"
             "         <INPUT TYPE=\"BUTTON\" NAME=\"delete\" "
             "VALUE=\"Delete\" "
             "ONCLICK=\"javascript:delete_addr('%s','%s')\">\n",
				user_pref.addr[x].name != NULL ?
					user_pref.addr[x].name :
					"",
				user_pref.addr[x].email);
      printf("      </TD>\n"
             "   </TR>\n");

   }

   return(0);
}

///////////////////////////////////////////////////////////

int addr_book_footer(void)
{
   printf("\n</TABLE>\n"
          "</FORM>\n\n"
          "<SCRIPT LANGUAGE=\"JavaScript\">\n"
          "<!--\n"
          "   function modify_name(name,email)\n"
          "   {\n"
          "      document.newaddress.realname.value = name;\n"
          "      document.newaddress.realemail.value = email;\n"
          "      document.newaddress.modifyname.value = name;\n"
          "      document.newaddress.modifyemail.value = email;\n"
          "      document.newaddress.add_modify.value = \"Modify\";\n"
          "      document.newaddress.subaction.value = \"modify\";\n"
          "   }\n"
          "   function clear_boxes()\n"
          "   {\n"
          "      document.newaddress.realname.value = \"\";\n"
          "      document.newaddress.realemail.value = \"\";\n"
          "      document.newaddress.modifyname.value = \"\";\n"
          "      document.newaddress.modifyemail.value = \"\";\n"
          "      document.newaddress.add_modify.value = \"  Add  \";\n"
          "      document.newaddress.subaction.value = \"add\";\n"
          "   }\n"
          "   function delete_addr(name,email)\n"
          "   {\n"
          "      document.newaddress.realname.value = \"\";\n"
          "      document.newaddress.realemail.value = \"\";\n"
          "      document.newaddress.modifyname.value = name;\n"
          "      document.newaddress.modifyemail.value = email;\n"
          "      document.newaddress.add_modify.value = \"delete\";\n"
          "      document.newaddress.subaction.value = \"delete\";\n"
          "      document.newaddress.submit();\n"
          "   }\n"
          "//-->\n"
          "</SCRIPT>\n\n");

   return(0);
}

///////////////////////////////////////////////////////////

