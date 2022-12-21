/*
	login_page.cpp
*/


#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>

#include "main.h"
#include "headers.h"

////////////////////////////////////////////////////////////////////

int login_page(void)
{
   content_html();
   html_header("Login");

   printf("<BR><BR>\n"
          "<TABLE BORDER=\"0\" ALIGN=\"CENTER\" WIDTH=\"40%%\" "
          "CELLPADDING=\"1\" CELLSPACING=\"1\">\n"
          "   <TR>\n"
          "      <TD BGCOLOR=\"#002266\" ALIGN=\"LEFT\">\n"
          "         <FONT COLOR=\"#FFFFFF\" FACE=\"Arial, Helvetica\" "
          "SIZE=\"3\">\n"
          "         <B>LOGIN</B></FONT>\n"
          "      </TD>\n"
          "   </TR>\n"
          "   <TR>\n"
          "      <TD ALIGN=\"CENTER\" VALIGN=\"MIDDLE\" "
          "BGCOLOR=\"#EEEEEE\">\n");
   printf("         <FORM METHOD=\"POST\" "
          "ENCTYPE=\"application/x-www-form-urlencoded\" "
          "ACTION=\"%s\">\n",user_env.script_name);
   printf("            <INPUT TYPE=\"hidden\" NAME=\"action\" "
          "VALUE=\"login\">\n"
          "            <P>UserID: \n"
          "            <FONT FACE=\"monospace\">\n"
          "            <INPUT TYPE=\"text\" NAME=\"userid\" SIZE=10>\n"
          "            </FONT>\n"
          "            <BR>Password:  \n"
          "            <FONT FACE=\"monospace\">\n"
          "            <INPUT TYPE=\"password\" "
          "NAME=\"password\" SIZE=10>\n"
          "            </FONT>\n"
          "            <BR><INPUT TYPE=\"submit\" NAME=\"Login\" "
          "VALUE=\"Login\">\n"
          "            <INPUT TYPE=\"reset\" VALUE=\"Clear\">\n"
          "         </FORM>\n"
          "      </TD>\n"
          "   </TR>\n"
          "</TABLE>\n");

   html_footer();

   return(0);
}

////////////////////////////////////////////////////////////////////



