/*
	err_page.cpp
*/

#include <stdio.h>
#include <syslog.h>

#include "main.h"
#include "headers.h"

///////////////////////////////////////////////////////////////////////

int err_page(void)
{
   content_html();
   html_header("ERROR");

   printf("<TABLE BORDER=\"0\" ALIGN=\"CENTER\" WIDTH=\"40%%\" "
          "CELLPADDING=\"1\" CELLSPACING=\"1\">\n"
          "   <TR>\n"
          "      <TD BGCOLOR=\"#002266\" ALIGN=\"LEFT\">\n"
          "         <FONT COLOR=\"#FFFFFF\" FACE=\"Arial, Helvetica\" "
          "SIZE=\"3\">\n"
          "         <B>Internal Error<B></FONT>\n"
          "      </TD>\n"
          "   </TR>\n"
          "   <TR>\n"
          "      <TD ALIGN=\"LEFT\" VALIGN=\"MIDDLE\" BGCOLOR=\"#EEEEEE\">\n"
          "         <FONT FACE=\"Arial, Helvetica\" SIZE=\"2\">\n"
          "         <BR>An internal error has occurred.\n"
          "         <BR>Please contact your system administrator.\n"
          "         <BR><BR>\n"
          "         </FONT>\n"
          "      </TD>\n"
          "   </TR>\n"
          "</TABLE>\n");

   html_footer();

   return(0);
}

///////////////////////////////////////////////////////////////////////

int err_string_page(char* err_msg)
{
   content_html();

   html_header("ERROR");

   printf("<TABLE BORDER=\"0\" ALIGN=\"CENTER\" WIDTH=\"40%%\" "
          "CELLPADDING=\"1\" CELLSPACING=\"1\">\n"
          "   <TR>\n"
          "      <TD BGCOLOR=\"#002266\" ALIGN=\"LEFT\">\n"
          "         <FONT COLOR=\"#FFFFFF\" FACE=\"Arial, Helvetica\" "
          "SIZE=\"3\">\n"
          "         <B>Error<B>\n"
          "         </FONT>\n"
          "      </TD>\n"
          "   </TR>\n"
          "   <TR>\n"
          "      <TD ALIGN=\"LEFT\" VALIGN=\"MIDDLE\" BGCOLOR=\"#EEEEEE\">\n"
          "         <BR>");

   if(err_msg != NULL)
      printf("%s\n",err_msg);

   printf("         <BR><BR>\n"
          "      </TD>\n"
          "   </TR>\n"
          "</TABLE>");

   html_footer();

   return(0);
}

///////////////////////////////////////////////////////////////////////

int err_bad_user_page(void)
{
   content_html();
   html_header("ERROR - Bad Username/password");

   printf("<TABLE BORDER=\"0\" ALIGN=\"CENTER\" WIDTH=\"40%%\" "
          "CELLPADDING=\"1\" CELLSPACING=\"1\">\n"
          "   <TR>\n"
          "      <TD BGCOLOR=\"#002266\" ALIGN=\"LEFT\">\n"
          "         <FONT COLOR=\"#FFFFFF\" FACE=\"Arial, Helvetica\" "
          "SIZE=\"3\">\n"
          "         <B>ERROR: Bad username / password</B>\n"
          "         </FONT>\n"
          "      </TD>\n"
          "   </TR>\n"
          "   <TR>\n"
          "      <TD ALIGN=\"LEFT\" VALIGN=\"MIDDLE\" BGCOLOR=\"#EEEEEE\">\n"
          "         <FONT FACE=\"Arial, Helvetica\" SIZE=\"2\">\n"
          "         <BR><BR>Username or password is incorrect\n"
          "         <BR>Check you're username/password and login again.\n"
          "         <BR><BR>");
   printf("<A HREF=\"%s\">Back to login page.</A>\n",
				user_env.script_name);
   printf("         <BR><BR>\n"
          "         </FONT>\n"
          "      </TD>\n"
          "   </TR>\n"
          "</TABLE>");

   html_footer();

   return(0);
}

///////////////////////////////////////////////////////////////////////

int err_timeout_page(void)
{
   content_html();
   html_header("ERROR - Session timeout");

   printf("<TABLE BORDER=\"0\" ALIGN=\"CENTER\" WIDTH=\"40%%\" "
          "CELLPADDING=\"1\" CELLSPACING=\"1\">\n"
          "   <TR>\n"
          "      <TD BGCOLOR=\"#002266\" ALIGN=\"LEFT\">\n"
          "         <FONT COLOR=\"#FFFFFF\" FACE=\"Arial, Helvetica\" "
          "SIZE=\"3\">\n"
          "         <B>ERROR: Session Timeout</B>\n"
          "         </FONT>\n"
          "      </TD>\n"
          "   </TR>\n"
          "   <TR>\n"
          "      <TD ALIGN=\"LEFT\" VALIGN=\"MIDDLE\" BGCOLOR=\"#EEEEEE\">\n"
          "         <FONT FACE=\"Arial, Helvetica\" SIZE=\"2\">\n"
          "         <BR><BR>You're current session has timed out.\n"
          "         <BR>Please login again.\n"
          "         <BR><BR>\n");
   printf("         <A HREF=\"%s\">Back to login page.</A>\n",
			user_env.script_name);
   printf("         <BR><BR>\n"
          "         </FONT>\n"
          "      </TD>\n"
          "   </TR>\n"
          "</TABLE>");

   html_footer();

   return(0);
}

///////////////////////////////////////////////////////////////////////
