/*
	logout_page.cpp
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

#include "headers.h"
#include "main.h"

int logout_page(void);

////////////////////////////////////////////////////////////////////

int logout_action(void)
{
   logout_session();
   logout_page();

   return(0);
}

////////////////////////////////////////////////////////////////////

int logout_page(void)
{
   content_html();
   html_header("Logout");

   printf("<TABLE BORDER=\"0\" ALIGN=\"CENTER\" WIDTH=\"40%%\" "
          "CELLPADDING=\"1\" CELLSPACING=\"1\">\n"
          "   <TR>\n"
          "      <TD BGCOLOR=\"#002266\" ALIGN=\"LEFT\">\n"
          "         <FONT COLOR=\"#FFFFFF\" FACE=\"Arial, Helvetica\" "
          "SIZE=\"3\">\n"
          "         <B>Logout Successful</B>\n"
          "         </FONT>\n"
          "      </TD>\n"
          "   </TR>\n"
          "   <TR>\n"
          "      <TD ALIGN=\"LEFT\" VALIGN=\"MIDDLE\" BGCOLOR=\"#EEEEEE\">\n"
          "         <BR><BR>\n");

   printf("         <A HREF=\"%s\">Back to Login Page.</A>\n",
				user_env.script_name);

   printf("         <BR><BR>\n"
          "      </TD>\n"
          "   </TR>\n"
          "</TABLE>");

   html_footer();

   return(0);
}
