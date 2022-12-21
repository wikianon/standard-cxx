/*
	view_attachment.cpp
*/

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>

#include "environment.h"
#include "parse_form.h"
#include "main.h"
#include "err_page.h"
#include "parse_mail_headers.h"
#include "mail_process.h"
#include "headers.h"
#include "view_attachment.h"
#include "view_message.h"
#include "encoding_conv.h"


////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

int get_attachment_cgi_data(int*, int*, content_disposition*);

status_type find_attachment(FILE*, 
				mail_header**, mail_header**,
				int, int*, 
				char*, char**);

status_type find_next_ct_multipart(FILE*, 
				mail_header**, mail_header**,
				int, int*, 
				char*, char**);
status_type find_next_ct_message_rfc822(FILE*, 
				mail_header**, mail_header**,
				int, int*,
				char*, char**);

int display_attachment(FILE*, mail_header*, char*, content_disposition);
int disp_att_cont_type(FILE*, char*, mail_header*);


int disp_att_ct_base64(FILE*, char*);
int disp_att_ct_none(FILE*, char*, int);

/////////////////////////////////////////////////////////////////////

int view_attachment_action(void)
{
   FILE* user_file=NULL;
   int m_num=0;      
   int part_num=0;
   int new_part_num=0;
   status_type status=st_pending;
   char* new_boundary=NULL;
   mail_header* mh_data=NULL;
   mail_header* new_mh_data=NULL;
   content_disposition cont_disp=ctd_none;
   int err=0;

//get cgi data

   if(get_attachment_cgi_data(&m_num,&part_num,&cont_disp) != 0)
      return(-1);

//open file

   setgid(egid);
   user_file=open_user_mailfile("r");
   setgid(rgid);

   if(user_file == NULL)
      return(-1);

//point to message

   err=point_to_message(user_file,m_num);

   if(err != 0)
      return(-1);

//find attachment

   mh_data=fill_mail_h_struct(user_file, ++new_part_num);

   status=find_attachment(user_file, &mh_data, &new_mh_data,
			part_num, &new_part_num, 
			mh_data->cont_boundary, &new_boundary);


   if(status != st_found || new_mh_data == NULL)
   {
      err_page();
      return(-1);
   }

   display_attachment(user_file,new_mh_data,new_boundary,cont_disp);
  
   free(new_boundary);

   return(0);
}

/////////////////////////////////////////////////////////////////////
//This function will set the current file pointer to the start of the 
//      headers for the attachment part_num
//   RETURNS:
//      st_error if attachment not found or error occurs
//      st_found if attachment found
//      st_pending if next border found
//      st_done if end border found

status_type find_attachment(FILE* user_file,
			mail_header** mh_data,
			mail_header** new_mh_data,
                        int part_num,
			int* new_part_num,
			char* boundary,
			char** new_boundary)
{
   status_type status=st_pending;

   if(*new_part_num >= part_num)
   {
      if(boundary != NULL)
         *new_boundary=strdup(boundary);

      *new_mh_data=*mh_data;
      *mh_data=NULL;

      return(st_found);
   }

   if((*mh_data)->cont_type == ct_multipart_mixed ||
	(*mh_data)->cont_type == ct_multipart_parallel ||
	(*mh_data)->cont_type == ct_multipart_digest ||
	(*mh_data)->cont_type == ct_multipart_alternative ||
	(*mh_data)->cont_type == ct_multipart_other) 
   {
      status=find_next_ct_multipart(user_file, mh_data, new_mh_data,
				part_num, new_part_num, 
      				boundary, new_boundary); 
      return(status);
   }



   else if((*mh_data)->cont_type == ct_message_rfc822)
   {
      status=find_next_ct_message_rfc822(user_file, mh_data, new_mh_data,
				part_num, new_part_num,
				boundary, new_boundary);

      return(status);
   }

   else
   {
      if(boundary != NULL)
         status=find_next_border(user_file,boundary);
      else
         status=st_error;

      if(status == st_found)
         status=st_pending;
   }

   return(status);
}


////////////////////////////////////////////////////////////////////
//   RETURNS:
//      st_error if attachment not found or error occurs
//      st_found if attachment found
//      st_pending if next border found
//      st_done if end border found


status_type find_next_ct_multipart(FILE* user_file, 
				mail_header** mh_data,
				mail_header** new_mh_data,
				int part_num, 
				int* new_part_num, 
				char* boundary,
				char** new_boundary) 
{
   mail_header* next_mh=NULL;

   status_type status=st_pending;

   if(*mh_data == NULL || (*mh_data)->cont_boundary == NULL)
      return(st_error);

   status=find_next_border(user_file,(*mh_data)->cont_boundary);

   if(status == st_error || status == st_done)
      return(status);

   status=st_pending;   
   
   do
   {
      next_mh=fill_mail_h_struct(user_file,++(*new_part_num));

      if(next_mh != NULL && next_mh->cont_type == ct_none)
      {
         if( (*mh_data)->cont_type == ct_multipart_digest )
            next_mh->cont_type=ct_message_rfc822;
         else
            next_mh->cont_type=ct_text_plain;
      }

      
      status=find_attachment(user_file,&next_mh,new_mh_data,
			part_num, new_part_num,
			(*mh_data)->cont_boundary,new_boundary);

      if(status != st_found && next_mh != NULL)
         free_mail_headers(next_mh);
   }
   while(status == st_pending);


   if(status == st_done && boundary != NULL)
   {
      status=find_next_border(user_file,boundary);

      if(status == st_found)
         status=st_pending;
   }

   return(status); 
}

////////////////////////////////////////////////////////////////////
//   RETURNS:
//      st_error if attachment not found or error occurs
//      st_found if attachment found
//      st_pending if next border found
//      st_done if end border found


status_type find_next_ct_message_rfc822(FILE* user_file,
			mail_header** mh_data, mail_header** new_mh_data,
			int part_num, int* new_part_num,
			char* boundary, char** new_boundary)
{
   mail_header* next_mh=NULL;
   status_type status=st_pending;


   next_mh=fill_mail_h_struct(user_file,++(*new_part_num));

   if(next_mh == NULL)
      status=st_error;
   else
      status=find_attachment(user_file, &next_mh, new_mh_data,
			part_num, new_part_num, boundary, new_boundary);

   if(status == st_pending && boundary != NULL)
   {
      status=find_next_border(user_file,boundary);

      if(status == st_found)
         status=st_pending;
   }

   if(next_mh != NULL)
      free_mail_headers(next_mh);

   return(status);
}

////////////////////////////////////////////////////////////////////
//This will get the cgi data needed to view an attachment.
//   returns 0 on success
//   returns -1 on failure

int get_attachment_cgi_data(int* m_num, int* part_num,
			content_disposition* cont_disp)
{
   cgi_t* cgi_data=NULL;

   *m_num=0;
   *part_num=0;
   *cont_disp=ctd_none;

   while(get_cgi_data(&cgi_data) == 0)
   {

      if(strcmp(cgi_data->name,"m_num") == 0)
         *m_num=strtol(cgi_data->value,NULL,10);

      if(strcmp(cgi_data->name,"part_num") == 0)
         *part_num=strtol(cgi_data->value,NULL,10);

      if(strcmp(cgi_data->name,"disp") == 0)
      {
         if(strcmp(cgi_data->value,"inline") == 0)
            *cont_disp=ctd_inline;
         else if(strcmp(cgi_data->value,"attachment") == 0)
            *cont_disp=ctd_attachment;
      }
   }
   

   return(0);
}

////////////////////////////////////////////////////////////////////
//This will sort out an attachment and call the appropriate functions
//   to display it


int display_attachment(FILE* user_file, mail_header* mh_data, 
			char* boundary, content_disposition cont_disp)
{
   int err=0;

   if(mh_data == NULL || user_file == NULL)
      return(-1);			//sanity check

   if(mh_data->cont_type_string != NULL)
      printf("Content-Type: %s\n",mh_data->cont_type_string);
   else
      printf("Content-Type: text/plain\n");

   printf("Content-Disposition: ");

   if(cont_disp == ctd_inline)
      printf("inline; ");
   else
      printf("attachment; ");

   printf("filename=\"%s\"\n\n", mh_data->filename == NULL ? 
			"unknown" : mh_data->filename);

   if(mh_data->cont_type_enc == cte_base64)
      err=disp_att_ct_base64(user_file, boundary);

   else if(mh_data->cont_type_enc == cte_quoted_printable)
      err=disp_att_ct_none(user_file, boundary, -1);

   else
      err=disp_att_ct_none(user_file, boundary, 0);

   return(0);
}

////////////////////////////////////////////////////////////////////

int disp_att_ct_base64(FILE* user_file, char* boundary)
{
   char* file_buff=NULL;
   size_t file_buff_size=0;
   ssize_t b_read=0;
   char* line_ptr=NULL;
   
   size_t stop_set=0;

   status_type status_overall=st_nl;
			//set to st_nl until start of base64 block found
			//   set to st_pending on pending 
			//   set to st_error on error
			//   set to st_done on finished with block

   status_type status_line=st_pending;
			//set to st_pending if current line not finished


   do
   {

      b_read=getline(&file_buff,&file_buff_size,user_file);

      if(b_read <= 0 || file_buff == NULL)
      {
         if(file_buff != NULL)
            free(file_buff);
         return(-1);
      }

					//if we are done
      if(status_overall == st_nl)
      {
         if(*file_buff != '\n')
            status_overall=st_pending;
      }
      else if(status_overall == st_pending && *file_buff == '\n')
         status_overall=st_done;

      status_overall=is_boundary(file_buff,boundary);

      status_line=st_pending;

      line_ptr=file_buff;

      if(status_overall == st_pending)
      {
         do
         {
            status_line=conv_and_out_base64_string(line_ptr);

            if( strlen(line_ptr) > 4) 
               line_ptr+=4;
            else
               status_line=st_nl;

            if(status_line == st_pending && strlen(line_ptr) < 4)
               status_line=st_nl;

            else if( status_line == st_pending)
            {
               stop_set=strcspn(line_ptr," \n");

               if(stop_set < 4)
                  status_line=st_nl;
            }
         }
         while(status_line == st_pending && status_overall == st_pending);

         if(status_line == st_done)
            status_overall=st_done;
         else if(status_line == st_error)
            status_overall=st_error;
      }
   }
   while(status_overall == st_nl || status_overall == st_pending);

   return(0);
}

////////////////////////////////////////////////////////////////////

int disp_att_ct_none(FILE* user_file, char* boundary, int quoted_printable)
{
   char* file_buff=NULL;
   size_t file_buff_size=0;
   ssize_t b_read=0;

   status_type status=st_nl;

   do
   {
      b_read=getline(&file_buff,&file_buff_size,user_file);

      if(b_read <= 0 || file_buff == NULL)
      {
         if(file_buff != NULL)
         {
            free(file_buff);
            file_buff=NULL;
         }
         status=st_error;
         return(-1);
      }

      status=is_boundary(file_buff,boundary);


      if(status == st_pending || status == st_nl)
      {
         if(quoted_printable)
            fix_quoted_to_text(file_buff);

         printf("%s",file_buff);
      }
   }
   while(status == st_pending || status == st_nl);
      

   if(file_buff != NULL)
      free(file_buff);

   return(0);
}

////////////////////////////////////////////////////////////////////


