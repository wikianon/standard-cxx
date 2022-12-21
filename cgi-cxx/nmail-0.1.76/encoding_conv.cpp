/*
	encoding_conv.cpp
*/

#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <syslog.h>

#include "view_message.h"
#include "custom_string.h"
#include "encoding_conv.h"

int convert_qt_hex(char*);
int is_hex(char);
int conv_base64_char_to_value(char);

////////////////////////////////////////////////////////////////////////

int text_to_html(char* buff)
{
   char* temp_ptr=NULL;
   char* new_buff_ptr=NULL;
   char tmp_char='\0';

   if(buff == NULL)
      return(-1);

   new_buff_ptr=buff;

   do
   {
      temp_ptr=strpbrk(new_buff_ptr,"<>");

      if(temp_ptr != NULL)
      {
         tmp_char=*temp_ptr;
         *temp_ptr='\0';
      }

      printf("%s",new_buff_ptr);

      if(temp_ptr != NULL)
      {
         new_buff_ptr=temp_ptr+1;

         if(tmp_char == '<')
            printf("&lt;");

         else if (tmp_char == '>')
            printf("&gt;");
      }
   }
   while(temp_ptr != NULL);

   return(0);
}

////////////////////////////////////////////////////////////////////////
//This will convert a qutoed printable text line to it's actual value

int fix_quoted_to_text(char* buff)
{
   char* tmp_ptr=NULL;
   char* move_ptr=NULL;
   char* next_ptr=NULL;
   int c=-1;
   int err=0;

   if(buff == NULL)
      return(-1);

   tmp_ptr=strrpcbrk(buff," \t\n");

   if(tmp_ptr != NULL)
   {
      if(*tmp_ptr == '=')
         *tmp_ptr='\0';
   }

   next_ptr=buff;

   do
   {
      tmp_ptr=strchr(buff,'=');

      if(tmp_ptr == NULL)
         next_ptr=NULL;
      else
      {
         next_ptr=tmp_ptr+1;

         if(*(tmp_ptr+1) != '\0' && *(tmp_ptr+2) != '\0')
            move_ptr=tmp_ptr+3;
         else
            move_ptr=NULL;

         c=convert_qt_hex(tmp_ptr+1);

         if(c < 0)
            err=-1;
         else
         {
            *tmp_ptr=c;
            tmp_ptr++;
         }

         if(move_ptr != NULL)
         {
            while(*move_ptr != '\0')
            {
               *tmp_ptr=*move_ptr;
               tmp_ptr++;
               move_ptr++;
            }
            *tmp_ptr='\0';
         }
      }
   }
   while(next_ptr != NULL);

   return(err);
}

////////////////////////////////////////////////////////////////////////
//This converts the two char's pointed to by char* to their converted
//      hex character.
//   if either char not 0-9 or A-F, returns -1

int convert_qt_hex(char* buff)
{
   int value=0;

   if(buff == NULL)
      return(-1);

   if(*buff == '\0')
      return(-1);

   if( is_hex(*buff) != 0 || is_hex(*(buff+1)) != 0 ) 
      return(-1);

   if( *buff >= '0' && *buff <= '9')
      value=(*buff - '0') << 4;
   else
      value=(*buff - 'A' + 10) << 4;

   if( *(buff+1) >= '0' && *(buff+1) <= '9')
      value|=*(buff+1)-'0';
   else
      value|=*(buff+1)-'A' + 10;

   return(value);
}


////////////////////////////////////////////////////////////////////////

int is_hex(char c)
{

   if( ! ( ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F')) ) )
      return(-1);

   return(0);
}

////////////////////////////////////////////////////////////////////////
//Converts and sends to stdout 3 bytes converted from 4 base64 characters
//   returns st_pending on success
//   returns st_error on error
//   returns st_done on end of base64 encoding

status_type conv_and_out_base64_string(char* buff)

{
   int in[4];
   int out[3];
   int err=0;

   int x=0;

   if(buff == NULL)
      return(st_error);

   if(strlen(buff) < 4)
      return(st_error);

   for(x=0;x<4;x++)
      in[x]=*(buff+x);

   err=conv_from_base64(in,out);	//convert in to out

   if(err != 0)
      return(st_error);

   for(x=0; x < 3; x++)
   {
      if(out[x] >= 0)			//if valid
         err=putchar(out[x]);
      else				//otherwise, we're done
         return(st_done);

      if(err == EOF)
      {
         return(st_error);		//remote closed connection, stop
      }
   }

   return(st_pending);
}

////////////////////////////////////////////////////////////////////////

int conv_from_base64(int* in, int* out)
{
   int x=0;

   if(in == NULL || out == NULL)
      return(-1);

   for(x=0; x<4; x++)			//mask off all but valid chars
      in[x]&=0x00ff;

   for(x=0; x<3; x++)			//init output
      out[x]=0;

   for(x=0; x<4; x++)			//convert to value
   {
      in[x]=conv_base64_char_to_value(in[x]);

      if(in[x] == -1)
         return(-1);
   }

   if(in[1] == 64 || in[0] == 64)
      return(-1);

   out[0]=( ((in[0] & 0x003f) << 2) | ( (in[1] & 0x0030) >> 4) );
   out[1]=( ((in[1] & 0x000f) << 4) | ( (in[2] & 0x003c) >> 2) );
   out[2]=( ((in[2] & 0x0003) << 6) | ( (in[3] & 0x003f)     ) );

   if(in[3]==64)
      out[2]=-1;
   if(in[2] == 64)
      out[1]=-1;

   return(0);
}

////////////////////////////////////////////////////////////////////////
//Converts base64 char to it's value
//   returns converted value on success
//   returns -1 on error
//   returns 0 on '=' (last char in encoding)

int conv_base64_char_to_value(char c)
{
   if(c >= 'A' && c <= 'Z')  
      return( c-'A');

   if(c >= 'a' && c <= 'z')
      return(c-'a' + 26);

   if(c >= '0' && c <= '9')
      return(c-'0'+ 52);

   if(c == '+')
      return(62);

   if(c == '/')
      return(63);

   if(c == '=')
      return(64);

   return(-1);
}

////////////////////////////////////////////////////////////////////////


