/*
	parse_mail_headers.cpp
*/

#define _GNU_SOURCE

#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <syslog.h>
#include <errno.h>

#include "inbox_page.h"
#include "session.h"
#include "parse_mail_headers.h"
#include "main.h"
#include "mail_process.h"
#include "view_message.h"
#include "encoding_conv.h"
#include "custom_string.h"

char* get_head_data(FILE*, char**, size_t*);
time_t compute_time(char*);
char* get_ct_string(char*);
char* get_parameter_value(char*,char*);

int parse_rfc2047(char**);
char parse_rfc2047_token(char*, char**, char**);
int unencode_rfc2047_quoted_printable(char*, char*, char*);
int unencode_rfc2047_base64(char*, char*, char*);
int quoted_unencode(char*);
int conv_from_ascii_hex(char);

void free_mh_data(struct mail_header*);

int inbox_size;

//////////////////////////////////////////////////////////////////////
//This will pull all mail headers from a users mailfile. Used during inbox
//   display

struct mail_header* get_mail_headers(void)
{
   FILE* user_mail_file=NULL;  
   int m_num=1;
   int err=0;

   struct mail_header* first_mh_data;
   struct mail_header* next_header_ptr;

   const char* stupid_imap_message="DON'T DELETE THIS MESSAGE";

   inbox_size=0;

   setgid(egid);
   user_mail_file=open_user_mailfile("r");
   setgid(rgid);

   if(user_mail_file == NULL)
   {
      if(errno != 0)
         errno=-1;
      else
         errno=0;
      return( (struct mail_header*) NULL);
   }

   m_num=1;
				//Create initial mail header structure

   first_mh_data=fill_mail_h_struct(user_mail_file,m_num++);


   if(first_mh_data != NULL)
      err=point_to_next_message(user_mail_file,
				&(first_mh_data->size_of_message));

   if(first_mh_data != NULL && first_mh_data->subject != NULL)
      if(strncmp(first_mh_data->subject,stupid_imap_message,
		strlen(stupid_imap_message)) == 0)
      {
         syslog(LOG_MAIL | LOG_INFO,"IMAP message found for %s",
				user_ses.loginname);
         free_mh_data(first_mh_data);
         first_mh_data=NULL;
         first_mh_data=fill_mail_h_struct(user_mail_file,m_num++);

         if(first_mh_data != NULL)
            err=point_to_next_message(user_mail_file,
				&(first_mh_data->size_of_message));
      }

   if(first_mh_data == NULL)
   {
      fclose(user_mail_file);
      errno=0;
      return((struct mail_header*) NULL);
   }

   next_header_ptr=first_mh_data;

				//Parse mail file headers
   while(next_header_ptr != NULL && err == 0)
   {
      next_header_ptr->next_header=
		fill_mail_h_struct(user_mail_file,m_num++);
      next_header_ptr=next_header_ptr->next_header;

      if(next_header_ptr != NULL)
         err=point_to_next_message(user_mail_file,
				&(next_header_ptr->size_of_message));
   } 

   fclose(user_mail_file);
   return(first_mh_data);
}

//////////////////////////////////////////////////////////////////////
//This will initialize a mail_header structure

void init_mail_header(struct mail_header* mh_data)
{
   mh_data->m_num=0;
   mh_data->next_header=NULL;
   mh_data->size_of_message=0;

   mh_data->mail_status=m_new;
   mh_data->date_received=0;

   mh_data->from=NULL;
   mh_data->to=NULL;
   mh_data->cc=NULL;
   mh_data->reply_to=NULL;
   mh_data->subject=NULL;

   mh_data->cont_type=ct_none;
   mh_data->cont_type_string=NULL;
   mh_data->cont_boundary=NULL;
   mh_data->cont_type_enc=cte_none;
   mh_data->cont_disposition=ctd_none;
   mh_data->filename=NULL;
}

//////////////////////////////////////////////////////////////////////
// This function will read the mail header the current file pointer points
//    to, allocate a mail_header structure, and fill it

struct mail_header* fill_mail_h_struct(FILE* user_mail_file,int m_num)
{
   char* file_buff=NULL;
   size_t file_buff_size=0;
   ssize_t b_read=0;
   char* char_buff=NULL;
   struct mail_header* mh_data=NULL;

   enum h_done { hd_pending, hd_done, hd_error } header_done=hd_pending;

   if(user_mail_file == NULL)
   {
      syslog(LOG_MAIL | LOG_ERR,"fill_mail_h_struct was passed NULL file");
      return( (struct mail_header*) NULL );	//sanity check
   }

   if( feof(user_mail_file) != 0 )
   {
      syslog(LOG_MAIL | LOG_INFO,"End of file found during mail_h_fill.");
      return( (struct mail_header*) NULL );	//sanity check
   }

   mh_data=(struct mail_header*) malloc(sizeof(struct mail_header));

   if(mh_data == NULL)
   {
      syslog(LOG_MAIL | LOG_ERR,"Out of memory (%s)",strerror(errno));
      return( mh_data );
   }

   init_mail_header(mh_data);

   mh_data->m_num=m_num;

   do
   {
      b_read=getline(&file_buff,&file_buff_size,user_mail_file);

      mh_data->size_of_message+=b_read;

      if(b_read <= 0 || file_buff == NULL)
         header_done=hd_error;

      else if(*file_buff == '\n')
         header_done=hd_done;

      else if(strncasecmp(file_buff,"From:",sizeof("From:") -1 ) == 0 &&
		mh_data->from == NULL)
      {
         char_buff=
		get_head_data(user_mail_file, &file_buff, &file_buff_size);
         
         mh_data->from=parse_addr(&char_buff);
      }

      else if(strncasecmp(file_buff,"To:",sizeof("To:") - 1 ) == 0 &&
		mh_data->to == NULL)
      {
         char_buff=
		get_head_data(user_mail_file, &file_buff, &file_buff_size);
         mh_data->to=parse_addr(&char_buff);
      }
      else if(strncasecmp(file_buff,"Cc:", sizeof("cc:")-1) == 0 &&
		mh_data->cc == NULL)
      {
         char_buff=
		get_head_data(user_mail_file, &file_buff, &file_buff_size);
         mh_data->cc=parse_addr(&char_buff);
      }

      else if(strncasecmp(file_buff,"Reply-To:", sizeof("Reply-To:")-1 ) == 0 &&
		mh_data->reply_to == NULL)
      {
         char_buff=
		get_head_data(user_mail_file, &file_buff, &file_buff_size);
         mh_data->reply_to=parse_addr(&char_buff);
      }

      else if(strncasecmp(file_buff,"Subject:",sizeof("Subject:")-1 ) == 0 &&
		mh_data->subject == NULL)
         mh_data->subject=
		get_head_data(user_mail_file, &file_buff, &file_buff_size);

 
      else if(strncasecmp(file_buff,"Status:", sizeof("Status:")-1 ) == 0 &&
		strlen(file_buff) >=8 &&
		strchr(file_buff+8,'R') != NULL)
         mh_data->mail_status=m_read;


      else if(strncasecmp(file_buff,"Date:", sizeof("Date:")-1) == 0 )
      {
         char_buff=
		get_head_data(user_mail_file, &file_buff, &file_buff_size);
         mh_data->date_received=compute_time(char_buff);

         if(char_buff != NULL)
         {
            free(char_buff);
            char_buff=NULL;
         }
      }
      
      else if( strncasecmp(file_buff, "Content-Type:",
                       sizeof("Content-Type:")-1 ) == 0 && char_buff == NULL)
      {

         char_buff=
		get_head_data(user_mail_file, &file_buff, &file_buff_size);


         if(mh_data->cont_type_string == NULL)
            mh_data->cont_type_string = get_ct_string(char_buff);

         mh_data->cont_type=
		determine_content_type(mh_data->cont_type_string);

         if(mh_data->cont_type == ct_multipart_mixed ||
			mh_data->cont_type == ct_multipart_alternative ||
			mh_data->cont_type == ct_multipart_parallel ||
			mh_data->cont_type == ct_multipart_digest ||
			mh_data->cont_type == ct_multipart_other )
            if(mh_data->cont_boundary == NULL)
               mh_data->cont_boundary=
			get_parameter_value(char_buff,"boundary");

         if(mh_data->filename == NULL)
            mh_data->filename=get_parameter_value(char_buff,"name");

         free(char_buff);
         char_buff=NULL;
      }

      else if( strncasecmp(file_buff, "Content-Transfer-Encoding:",
			sizeof("Content-Transfer-Encoding:")-1) == 0 &&
			char_buff == NULL)
      {
         char_buff=
		get_head_data(user_mail_file, &file_buff, &file_buff_size);
         mh_data->cont_type_enc=determine_content_type_enc(char_buff);
         free(char_buff);
         char_buff=NULL;
      }
      else if (strncasecmp(file_buff, "Content-Disposition:",
			sizeof("Content-Disposition:") -1 ) == 0)
      {
         char_buff=
		get_head_data(user_mail_file, &file_buff, &file_buff_size);

         if(char_buff != NULL)
         {
            if(strncasecmp(char_buff,"inline",sizeof("inline")-1) == 0)
               mh_data->cont_disposition=ctd_inline;
            else if(strncasecmp(char_buff,"attachment",sizeof("attachment")-1)
						== 0)
               mh_data->cont_disposition=ctd_attachment;
         }

         if(char_buff != NULL)
         {
            if(mh_data->filename != NULL)
               free(mh_data->filename);
            mh_data->filename=get_parameter_value(char_buff,"filename");
         }

         free(char_buff);
         char_buff=NULL;
      }
   }
   while(header_done == hd_pending);

   if(file_buff != NULL)
      free(file_buff);

   if(header_done == hd_error)
   {
      free_mail_headers(mh_data);
      return(NULL);
   }

   return(mh_data);
}

//////////////////////////////////////////////////////////////////////
//This will return a newly allocated string that follows a ':' in the 
//      passed string that contains a read mail header line. It will then
//      check the next line to see if it is a continuation (long header line)
//   returns a string that starts with the first non - LWSP char following
//      the first ':' in the string. Will concatinate any line continuations
//      and eliminate any LWSP at the start of the next line.

char* get_head_data(FILE* user_mail_file,
		    char** file_buff,
                    size_t* file_buff_size)
{
   size_t new_size=0;
   size_t b_read=0;
   size_t skip_set=0;
   char* start=NULL;
   char* new_buff=NULL;
   char* temp_buff=NULL;
   int next_char='\0';


   enum status_type { st_pending, st_done, st_error };

   status_type status=st_pending;

   if(user_mail_file == NULL || *file_buff == NULL || file_buff_size == NULL)
      return(NULL);

//Find ':'

   start=strchr(*file_buff,':');

   if(start == NULL)
      return(NULL);

   start++;

//Get rid of initial LWSP

   start+=strspn(start," \t");

   if(*start == '\n' || *start == '\0')
      return(NULL);

   new_buff=strdup(start);

   if(new_buff == NULL)
      return(NULL);

//get rid of nl character

   start=strchr(new_buff,'\n');

   if(start != NULL)
      *start=' ';


// check if next line(s) have LWSP characters at start. 
//    If so, read them in also.

   do
   {
      next_char=fgetc(user_mail_file);

      if(! (next_char == '\t' || next_char == ' ') )
      {
         ungetc(next_char,user_mail_file);
         status=st_done;
      }
      else
      {
         b_read=getline(file_buff,file_buff_size,user_mail_file);

         if(b_read <= 0 || file_buff == NULL)
            status=st_error;
         else
         {

//Get rid of remaining LWSP before copy

            skip_set=strspn(*file_buff," \t");

            start=*file_buff+skip_set;

            new_size=strlen(new_buff) + strlen(start) + 1;

//resize buffer

            temp_buff=(char*)realloc(new_buff,new_size);

            if(temp_buff == NULL)
            {
               if(new_buff != NULL)
               {
                  free(new_buff);
                  new_buff=NULL;
               }
               status=st_error;
            }

//copy remaining string
               
            else
            {
               new_buff=temp_buff;

               strcat(new_buff,start);
               start=strchr(new_buff,'\n');

               if(start != NULL)
                  *start=' ';
            }
         }
      }
   }
   while(status == st_pending);

   parse_rfc2047(&new_buff);

   return(new_buff); 
}

//////////////////////////////////////////////////////////////////////
// This will check a header line to see if it is in the ASCII encoded
//    string format specified by RFC2047
//
// Format of string is:
//	=?charset?encoding?encoded_text?=
//
//    encoding is Q for quoted printable, B for base64 (case insensitive)
//    no linear whitespace characters are allowed in the token
//    limited to 76 characters per token
//    multiple tokens can appear in a header if folding occurs
//    

int parse_rfc2047(char** buff)
{
   char* find=NULL;
   char* start=NULL;
   char* end=NULL;
   char encoding=0;

   if(buff == NULL || *buff == NULL)
      return(-1);

   find=*buff;

   while(*find != '\0')
   {
      encoding=0;
      start=NULL;
      end=NULL;

      if(*find == '=' && *(find+1) == '?')
      {
         encoding=parse_rfc2047_token(find,&start,&end);

         if(encoding == 'Q' || encoding == 'q')
            unencode_rfc2047_quoted_printable(find,start,end);

         else if(encoding == 'B' || encoding == 'b')
            unencode_rfc2047_base64(find,start,end);
      }

      find++;
   }

   return(0);
}

//////////////////////////////////////////////////////////////////////

char parse_rfc2047_token(char* buff, char** text, char** end)
{
   char* next=NULL;

   char* ptr_to_charset=NULL;
   char* ptr_to_encoding=NULL;
   char* ptr_to_text=NULL;
   char* ptr_to_end=NULL;

   if(buff == NULL || text == NULL || end == NULL)
     return(0);

   next=buff;

   while(ptr_to_end == NULL && 
         *next != '\0' && 
         *next != ' ' && 
         *next != '\t' && 
         *next != '\n')
   {
      if(*next == '?')
      {
         if(ptr_to_charset == NULL)
            ptr_to_charset=next;
         else if(ptr_to_encoding == NULL)
            ptr_to_encoding=next;
         else if(ptr_to_text == NULL)
            ptr_to_text=next;
         else if(ptr_to_end == NULL)
            ptr_to_end=next;

      }

      next++;
   }

   /* check to make sure we found all 4 '?' */
   if(   ptr_to_charset == NULL || 
         ptr_to_encoding == NULL || 
         ptr_to_text == NULL || 
         ptr_to_end == NULL)
      return(0);

   /* check to make sure the first '?' is prepended with a '=' */
   if(ptr_to_charset > buff)
      if( *(ptr_to_charset-1) != '=')
         return(0);

   /* check to make sure the last '?' is followed with a '=' */
   if( *(ptr_to_end+1) != '=')
      return(0);

   /* make sure charset has at least one character */
   if( ptr_to_charset+1 >= ptr_to_encoding)
      return(0);

   /* make sure encoding is valid character */
   if( *(ptr_to_encoding+1) != 'Q' && *(ptr_to_encoding+1) != 'q' &&
       *(ptr_to_encoding+1) != 'B' && *(ptr_to_encoding+1) != 'b')
      return(0);

   //passed all checks, looks valid
   *text=ptr_to_text+1;
   *end=ptr_to_end-1;

   return( *(ptr_to_encoding+1) );
}


//////////////////////////////////////////////////////////////////////
// This will unencode a quoted printable string. The encoded text starts
//    at encoding and ends at end. The start pointer is where the
//    unencoded message and the remaining string will be copied to (over).

int unencode_rfc2047_quoted_printable(char* start, char* encoding, char* end)
{
   char* next_enc=NULL;
   char* next_save=NULL;
   int ch=0;

   if(start == NULL || encoding == NULL)
      return(-1);

   next_save=start;
   next_enc=encoding;

   while(next_enc <= end && *next_enc != '\0')
   {
      if( *next_enc == '=' )	//if encoded char, decode it
      {
         ch=quoted_unencode(next_enc+1);

         if(ch >= 0)		//if encoding okay
         {
            *(next_save++)=ch;
            next_enc+=3;
         }
         else			//bad encoding, copy it to be tolerant
            *(next_save++)=*(next_enc++);
      }
      else			//not encoded, just copy it
         *(next_save++)=*(next_enc++);
   }

   if(*next_enc == '?')		//should be, unless '\0'
      next_enc++;

   if(*next_enc == '=')		//should be, unless '\0'
      next_enc++;

   while(*next_enc != '\0')	//copy remaining string
      *(next_save++)=*(next_enc++);

   *(next_save++)=*(next_enc++);//copy terminating NULL 

   return(0);
}

//////////////////////////////////////////////////////////////////////
// This will convert the two ascii hex characters pointed to by text into
//    binary form. Will return -1 if not valid ascii hex characters

int quoted_unencode(char* text)
{
   int a=0;
   int b=0;
   int c=0;

   if(text == NULL)
      return(-1);

   if(*text == '\0' || *(text+1) == '\0')
      return(-1);

   a=conv_from_ascii_hex(*text);
   b=conv_from_ascii_hex(*(text+1));

   if(a < 0 || b < 0)
      return(-1);

   c=(a & 0x0f) << 4 | (b & 0x0f);
  
   c&=0xff;

   return(c); 
}

//////////////////////////////////////////////////////////////////////
// This will convert the ascii hex character c into its pure binary form.
//    Will return -1 on error

int conv_from_ascii_hex(char c)
{
   if(c >= '0' && c <= '9')
      return(c-'0');

   if(c >= 'A' && c <= 'F')
      return(c-'A'+10);

   if(c >= 'a' && c <= 'f')
      return(c-'a'+10);

   return(-1);
}

//////////////////////////////////////////////////////////////////////

int unencode_rfc2047_base64(char* start, char* encoding, char* end)
{
   int in[4];
   int out[3];   

   int x=0;
   int err=0;

   char* ptr_next=NULL;
   char* ptr_save=NULL;

   if(start == NULL || encoding == NULL || end == NULL)
      return(-1);

   if(start >= encoding || encoding >= end)
      return(-1);

   for(ptr_save=start; ptr_save <= end; ptr_save++)
   {
      if(*ptr_save == '\0')
         return(-1);
   }

   ptr_next=encoding;
   ptr_save=start;

   while(ptr_next+3 <= end)
   {

      for(x=0; x<4; x++)
         in[x]=*(ptr_next+x);

      for(x=0; x<3; x++)
         out[x]=0;

      err=conv_from_base64(in,out);

      if(err == 0)
      {
         for(x=0; x<3; x++)
            if(out[x] >= 0)
               *(ptr_save++)=out[x];
         ptr_next+=4;
      }
      else
         ptr_next=end;
   }

   if(*(end+1) != '\0' && *(end+2) != '\0')
      ptr_next=end+3;
   else
      return(-1);

   while(*ptr_next != '\0')
      *(ptr_save++)=*(ptr_next++);	//copy remaining string

   
   *(ptr_save++)=*(ptr_next++);		//copy terminating null

   return(0);
}

//////////////////////////////////////////////////////////////////////
//This will free all mail_header structures used during inbox display
//
//   pass pointer to FIRST mail_header structure and it will free 
//   all subsequent mail_header structures

void free_mail_headers(struct mail_header* mh_data)
{
   mail_header* current_header=NULL;
   mail_header* next_header=NULL;

   next_header=mh_data; 

   while(next_header != NULL)
   {
      current_header=next_header;

      next_header=current_header->next_header;

      free_mh_data(current_header);
  }
}

//////////////////////////////////////////////////////////////////////
//This will free the data contained within a mail_header structure

void free_mh_data(struct mail_header* mh_data)
{
   if(mh_data != NULL)
   {
      if(mh_data->from != NULL)
         destroy_addr_arr(&(mh_data->from));

      if(mh_data->to != NULL)
         destroy_addr_arr(&(mh_data->to));

      if(mh_data->cc != NULL)
         destroy_addr_arr(&(mh_data->cc));

      if(mh_data->reply_to != NULL)
         destroy_addr_arr(&(mh_data->reply_to));

      if(mh_data->subject != NULL)
         free(mh_data->subject);

      if(mh_data->cont_type_string != NULL)
         free(mh_data->cont_type_string);

      if(mh_data->cont_boundary != NULL)
         free(mh_data->cont_boundary);

      if(mh_data->filename != NULL)
         free(mh_data->filename);

      free(mh_data);
   }
}

//////////////////////////////////////////////////////////////////////
//This will convert a date time string into a time_t value
//
//   pass string that points to first value of date time (no LWSP)

time_t compute_time(char* time_string)
{
   struct tm time_struct;
   time_t conv_time=0;

   char* months[]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep",
		"Oct","Nov","Dec"};

   char* charptr=NULL;
   int x;
   int err;

   char t_month[4];

   for(x=0;x<4;x++)
      t_month[x]=0;
 
   if(time_string == NULL)
      return(0);

   charptr=time_string;

//rfc822 states an optional day of week as first field. If first field is not
//   day of month, must be day of week. 
//   Skip Day of week (not needed for calculation)

   if(*charptr < '0' || *charptr > '9')
   {
      charptr=strchr(time_string,' ');

      if(charptr != NULL)
         charptr++;
   }

   if(charptr == NULL)
      return(0);


   err=sscanf(charptr,"%d %3c %d %d:%d:%d",
	&(time_struct.tm_mday),
	t_month,
	&(time_struct.tm_year),
	&(time_struct.tm_hour),
	&(time_struct.tm_min),
	&(time_struct.tm_sec) );

   if(err != 6)
      return(0);

//Fix for year

   time_struct.tm_year-=1900;

//Compute month

   x=0;
   time_struct.tm_mon=-1;

   while( time_struct.tm_mon == -1 && x < 12)
   {
      if( strcmp(t_month,months[x]) == 0)
         time_struct.tm_mon=x;
      x++;
   }

   if(time_struct.tm_mon == -1)
      return(0);

   time_struct.tm_isdst=-1;

   conv_time=mktime(&time_struct);
 
   return(conv_time);  
}


//////////////////////////////////////////////////////////////////////
//This will sort an array of mail_header structures used to order messages
//   during the inbox page

int sort_mh(struct mail_header* mh_data,struct mail_header*** sorted_headers)
{
   int num_messages=0;
   int x=0;
   int y=0;
   struct mail_header* mh_ptr=NULL;
   char* in_x=NULL;
   char* in_y=NULL;

   if(mh_data == NULL)
      return(0);			//no messages to sort

   mh_ptr=mh_data;

   while(mh_ptr != NULL)
   {
      num_messages++;
      mh_ptr=mh_ptr->next_header;
   }

   *sorted_headers=(struct mail_header**)
		malloc( sizeof(struct mail_header*) * num_messages );

   if(*sorted_headers == NULL)
   {
      syslog(LOG_MAIL | LOG_ERR,"Out of memory");
      return(-1);
   }

   mh_ptr=mh_data;

   for(x=0;x<num_messages;x++)
   {
      (*sorted_headers)[x]=mh_ptr;
      mh_ptr=mh_ptr->next_header;
   }

   if(num_messages <= 1)
      return(num_messages);


   switch (mail_sort) {
      case ms_subj_a:
         for(x=0; x < num_messages-1; x++)
            for(y=x+1; y < num_messages; y++)
               if(strcasecmp( (*sorted_headers)[x]->subject, 
			(*sorted_headers)[y]->subject) < 0)
               {
                  mh_ptr=(*sorted_headers)[x];
                  (*sorted_headers)[x]=(*sorted_headers)[y];
                  (*sorted_headers)[y]=mh_ptr;
               }
         break;
      case ms_subj_d:
         for(x=0; x < num_messages-1; x++)
            for(y=x+1; y < num_messages; y++)
               if(strcasecmp( (*sorted_headers)[x]->subject, 
			(*sorted_headers)[y]->subject) > 0)
               {
                  mh_ptr=(*sorted_headers)[x];
                  (*sorted_headers)[x]=(*sorted_headers)[y];
                  (*sorted_headers)[y]=mh_ptr;
               }
         break;
      case ms_sender_a:
         for(x=0; x < num_messages-1; x++)
            for(y=x+1; y < num_messages; y++)
            {
               if( (*sorted_headers)[x]->from[0].name != NULL)
                  in_x=(*sorted_headers)[x]->from[0].name;
               else if( (*sorted_headers)[x]->from[0].email != NULL)
                  in_x=(*sorted_headers)[x]->from[0].email;
               else
                  in_x=NULL;

               if( (*sorted_headers)[y]->from[0].name != NULL)
                  in_y=(*sorted_headers)[y]->from[0].name;
               else if( (*sorted_headers)[y]->from[0].email != NULL)
                  in_y=(*sorted_headers)[y]->from[0].email;
               else
                  in_y=NULL;


               if( in_x != NULL && in_y != NULL)
               {
                  if(strcasecmp(in_x,in_y) < 0)
                  {
                     mh_ptr=(*sorted_headers)[x];
                     (*sorted_headers)[x]=(*sorted_headers)[y];
                     (*sorted_headers)[y]=mh_ptr;
                  }
               }
               else if( in_x == NULL)
               {
                  mh_ptr=(*sorted_headers)[x];
                  (*sorted_headers)[x]=(*sorted_headers)[y];
                  (*sorted_headers)[y]=mh_ptr;
               }
            }
         break;
      case ms_sender_d:
         for(x=0; x < num_messages-1; x++)
            for(y=x+1; y < num_messages; y++)
            {
               if( (*sorted_headers)[x]->from[0].name != NULL)
                  in_x=(*sorted_headers)[x]->from[0].name;
               else if( (*sorted_headers)[x]->from[0].email != NULL)
                  in_x=(*sorted_headers)[x]->from[0].email;
               else
                  in_x=NULL;

               if( (*sorted_headers)[y]->from[0].name != NULL)
                  in_y=(*sorted_headers)[y]->from[0].name;
               else if( (*sorted_headers)[y]->from[0].email != NULL)
                  in_y=(*sorted_headers)[y]->from[0].email;
               else
                  in_y=NULL;


               if( in_x != NULL && in_y != NULL)
               {
                  if(strcasecmp(in_x,in_y) > 0)
                  {
                     mh_ptr=(*sorted_headers)[x];
                     (*sorted_headers)[x]=(*sorted_headers)[y];
                     (*sorted_headers)[y]=mh_ptr;
                  }
               }
               else if( in_y == NULL)
               {
                  mh_ptr=(*sorted_headers)[x];
                  (*sorted_headers)[x]=(*sorted_headers)[y];
                  (*sorted_headers)[y]=mh_ptr;
               }
            }
 

        break;
      case ms_size_a:
         for(x=0; x < num_messages-1; x++)
            for(y=x+1; y < num_messages; y++)
               if( (*sorted_headers)[x]->size_of_message >
			(*sorted_headers)[y]->size_of_message)
               {
                  mh_ptr=(*sorted_headers)[x];
                  (*sorted_headers)[x]=(*sorted_headers)[y];
                  (*sorted_headers)[y]=mh_ptr;
               }
         break;
      case ms_size_d:
         for(x=0; x < num_messages-1; x++)
            for(y=x+1; y < num_messages; y++)
               if( (*sorted_headers)[x]->size_of_message <
			(*sorted_headers)[y]->size_of_message)
               {
                  mh_ptr=(*sorted_headers)[x];
                  (*sorted_headers)[x]=(*sorted_headers)[y];
                  (*sorted_headers)[y]=mh_ptr;
               }
         break;
      case ms_date_a:
         for(x=0; x < num_messages-1; x++)
            for(y=x+1; y < num_messages; y++)
               if( (*sorted_headers)[x]->date_received >
			(*sorted_headers)[y]->date_received)
               {
                  mh_ptr=(*sorted_headers)[x];
                  (*sorted_headers)[x]=(*sorted_headers)[y];
                  (*sorted_headers)[y]=mh_ptr;
               }
         break;
      case ms_date_d:
      default:
         for(x=0; x < num_messages-1; x++)
            for(y=x+1; y < num_messages; y++)
               if( (*sorted_headers)[x]->date_received <
			(*sorted_headers)[y]->date_received)
               {
                  mh_ptr=(*sorted_headers)[x];
                  (*sorted_headers)[x]=(*sorted_headers)[y];
                  (*sorted_headers)[y]=mh_ptr;
               }
         break;
   }


   return(num_messages);
}

//////////////////////////////////////////////////////////////////////
//This will determine content type
//
//   first char of string passed should be first char of content type

content_type determine_content_type(char* char_buff)
{
   char x=0;

   struct content_type_labels {
      char* label;
      content_type ct_type;
   };

   const content_type_labels ct_labels[]={
      { "text/plain", ct_text_plain },
      { "text/html", ct_text_html },
      { "text/", ct_text_other },

      { "multipart/mixed", ct_multipart_mixed },
      { "multipart/alternative", ct_multipart_alternative },
      { "multipart/parallel", ct_multipart_parallel },
      { "multipart/digest", ct_multipart_digest },
      { "multipart/", ct_multipart_other },

      { "message/rfc822", ct_message_rfc822 },
      { "message/partial", ct_message_partial },
      { "message/external-body", ct_message_externalbody },
      { "message/", ct_message_other },

      { "application/pdf", ct_application_pdf },
      { "application/zip", ct_application_zip },
      { "application/x-zip-compressed", ct_application_zip },
      { "application/msword", ct_application_msword },
      { "application/vnd.ms-excel", ct_application_msexcel },
      { "application/msexcel", ct_application_msexcel },
      { "application/", ct_application_other },

      { "image/jpeg", ct_image_jpeg },
      { "image/gif", ct_image_gif },
      { "image/png", ct_image_png },
      { "image/", ct_image_other },

      { "audio/", ct_audio_other },

      { "video/", ct_video_other },

      { NULL, ct_other }
   };

   if(char_buff == NULL)
      return(ct_none);			//sanity check

   for(x=0; ct_labels[x].label; x++)
   {
      if( ct_labels[x].label == NULL )
         return(ct_other);
      if( strncasecmp(ct_labels[x].label, 
		char_buff,strlen(ct_labels[x].label)-1) == 0)
         return(ct_labels[x].ct_type);
   }

   return(ct_other);
}

//////////////////////////////////////////////////////////////////////
//This will determine the content type encoding
//
//   string passed should point to first char of encoding type

content_type_encoding determine_content_type_enc(char* char_buff)
{
   char x=0;

   struct content_type_enc_labels {
      char* label;
      content_type_encoding ct_encoding;
   };

   content_type_enc_labels cte_labels[]= {
      { "7bit", cte_7bit }, 
      { "quoted-printable", cte_quoted_printable },
      { "base64", cte_base64 },
      { "8bit", cte_8bit },
      { "binary", cte_binary },
      { NULL, cte_none }
   };

   if(char_buff == NULL)
      return(cte_none);


   for(x=0; cte_labels[x].label; x++)
   {
      if( strncasecmp(cte_labels[x].label, 
		char_buff,strlen(cte_labels[x].label)) == 0)
         return(cte_labels[x].ct_encoding);
   }


   return(cte_none);
}

//////////////////////////////////////////////////////////////////////
//This function will return a newly allocated string that is the 
//      content type text
//   pass pointer to Content-Type value
//      get_head_data should automatically point to string since it
//      gets rid of LWSP


char* get_ct_string(char* buff)
{
   char* new_buff=NULL;
   size_t new_size=0;

   if(buff == NULL)
   {
      syslog(LOG_MAIL | LOG_ERR,"get_ct_string passed null pointer.");
      return(NULL);
   }

//get size of content type string value

   new_size=strcspn(buff," \t;\n");

//allocate new string

   new_buff=(char*)malloc(new_size + 1);

   if(new_buff == NULL)
   {
      syslog(LOG_MAIL | LOG_ERR,"Out of memory.");
      return(NULL);
   }

//copy string

   strncpy(new_buff,buff,new_size);

//terminate string

   *(new_buff + new_size)='\0';

   return(new_buff);

}

//////////////////////////////////////////////////////////////////////
//Returns the parameter value
//   pass buffer and parameter name of value you want to find

char* get_parameter_value(char* buff,char* parameter_name)
{
   char* char_ptr=NULL;
   char* new_buff=NULL;
   size_t new_buff_size=0;

   status_type status=st_pending;

   if(buff == NULL || parameter_name == NULL)
      return(NULL);

   char_ptr=buff;

   do
   {
      char_ptr=strchr(char_ptr,';');	//all parameternames follow ';'

      if(char_ptr == NULL)
         return(NULL);

      char_ptr++;
					//skip any initial LWSP
      char_ptr=strpcbrk(char_ptr," \t");

      if(strncasecmp(char_ptr,parameter_name,strlen(parameter_name)) == 0)
      {
         status=st_found;
         char_ptr+=strlen(parameter_name);
      }
   }
   while(status==st_pending);

   if(status != st_found || char_ptr == NULL)
      return(NULL);


   char_ptr=strchr(char_ptr,'=');	//find '='

   if(char_ptr == NULL)
      return(NULL);

   char_ptr++;

   char_ptr=strpcbrk(char_ptr," \t\"");	//skip any LWSP after '='

   if(char_ptr == NULL)
      return(NULL);

					//compute remaing string size
   new_buff_size=strcspn(char_ptr,";\"\n");

					//allocate new string
   new_buff=(char*) malloc(new_buff_size + 1);

   if(new_buff == NULL)
      return(NULL);

//copy and return string

   strncpy(new_buff,char_ptr,new_buff_size);
   *(new_buff+new_buff_size)='\0';

   return(new_buff);
}

//////////////////////////////////////////////////////////////////////








