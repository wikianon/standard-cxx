/*
	headers.cpp
*/

#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>

#include "main.h"
#include "config.h"

///////////////////////////////////////////////////////////////////////////

int content_html(void)
{
   printf("Content-type: text/html\n"
   	  "Cache-Control: no-store\n\n");
   return(0);
}

////////////////////////////////////////////////////////////////////////////

int html_header(char* title_msg)
{
   printf("<HTML>\n"
          "<HEAD>\n"
          "   <TITLE>MAIL");

   if(title_msg != NULL)
      printf(": %s",title_msg);

   printf("</TITLE>\n"
          "   <LINK REL=STYLESHEET TYPE=\"text/css\" "
          "HREF=\"" NMHTTP "/default.css\">\n"
	  "   <META HTTP-EQUIV=\"expires\" CONTENT=\"0\">\n"
          "</HEAD>\n\n"
          "<BODY BACKGROUND=\"" NMHTTP "/background.gif\">\n\n");

   return(0);
}

////////////////////////////////////////////////////////////////////////////

int html_footer(void)
{
   printf("\n</BODY>\n</HTML>\n");
   return(0);
}

////////////////////////////////////////////////////////////////////////////

