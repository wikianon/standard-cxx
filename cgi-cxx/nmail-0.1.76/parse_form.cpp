/*
	parse_form.cpp
*/

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>	//getenv,free 
#include <string.h>	//strdup,
#include <errno.h>
#include <syslog.h>
#include <sys/stat.h>
#include <dirent.h>

#include "parse_form.h"
#include "main.h"
#include "environment.h"
#include "custom_string.h"
#include "config.h"


int acquire_post_data(cgi_t*);
int acquire_post_multipart_data(cgi_t*);
int acquire_get_data(cgi_t*);
int fix_cgi_string(char*);
char conv_hex(char);
int is_cgi_boundary(char*);
int get_parameter(char*, char*, char**);
int get_form_file_save(FILE*, char*);
int get_form_text(char**, char*);
int get_form_file(cgi_t*);
int init_cgi_struct(cgi_t*);

cgi_t* cgi_ptr=NULL;
int cgi_ptr_size=0;
int cgi_next_read=0;

////////////////////////////////////////////////////////////////////////////
// This will sort out the cgi name/value pairs. They are saved in an array
//    so we can rewind and parsed again.

int init_cgi_env(void)
{
   int err=0;
   void* tmp_ptr=NULL;

   if(cgi_ptr != NULL)
      return(-1);		//cgi_env already initialized

   /* request method should have been found in environment */
   if(user_env.r_method != cgi_get &&
      user_env.r_method != cgi_post &&
      user_env.r_method != cgi_post_multipart )
   {
      syslog(LOG_MAIL | LOG_ERR,"request method not specified");
      return(-1);
   }

   /* fill cgi name/value array */
   do
   {
      cgi_ptr_size++;
      tmp_ptr=realloc(cgi_ptr,sizeof(cgi_t)*cgi_ptr_size);

      if(tmp_ptr == NULL)
      {
         syslog(LOG_MAIL | LOG_ERR,"Out of memory (%s)",strerror(errno));
         return(-1);
      }

      cgi_ptr=(cgi_t*)tmp_ptr;
      tmp_ptr=NULL;

      init_cgi_struct( &(cgi_ptr[cgi_ptr_size-1]) );

      if(user_env.r_method == cgi_post)
         err=acquire_post_data( &(cgi_ptr[cgi_ptr_size-1]) );
      else if(user_env.r_method == cgi_post_multipart)
         err=acquire_post_multipart_data( &(cgi_ptr[cgi_ptr_size-1]) );
      else if(user_env.r_method == cgi_get)
         err=acquire_get_data( &(cgi_ptr[cgi_ptr_size-1]) );
   }
   while(err == 0);

   cgi_ptr_size--;

   if(err != 1)
      return(-1);

   return(0); 
}

////////////////////////////////////////////////////////////////////////////
// This will free the resources used by the cgi name/value pairs

int destroy_cgi_env(void)
{
   int x=0;

   if(cgi_ptr == NULL)
      return(-1);

   for(x=0; x<cgi_ptr_size; x++)
   {
      if(cgi_ptr[x].name != NULL)
      {
         free(cgi_ptr[x].name);
         cgi_ptr[x].name=NULL;
      }
      if(cgi_ptr[x].value != NULL)
      {
         free(cgi_ptr[x].value);
         cgi_ptr[x].value=NULL;
      }
      if(cgi_ptr[x].filename != NULL)
      {
         free(cgi_ptr[x].filename);
         cgi_ptr[x].filename=NULL;
      }
      if(cgi_ptr[x].tmp_file != NULL)
      {
         fclose(cgi_ptr[x].tmp_file);
         cgi_ptr[x].tmp_file=NULL;
      }
      if(cgi_ptr[x].tmp_filename != NULL)
      {
         setuid(euid);
         unlink(cgi_ptr[x].tmp_filename);
         setuid(ruid);

         free(cgi_ptr[x].tmp_filename);
         cgi_ptr[x].tmp_filename=NULL;
      }
      if(cgi_ptr[x].cont_type != NULL)
      {
         free(cgi_ptr[x].cont_type);
         cgi_ptr[x].cont_type=NULL;
      }
   }

   free(cgi_ptr);

   cgi_ptr=NULL;

   return(0);

}

////////////////////////////////////////////////////////////////////////////
// This function will point to the next name/value pairs in the cgi_ptr array
//    returns 0 on success, 1 end of cgi data, -1 on failure 

int get_cgi_data(cgi_t** cgi_data)
{
   int x=0;

   if(cgi_data == NULL)
      return(-1);

   if(cgi_next_read >= cgi_ptr_size)
      return(1);

   for( x=cgi_next_read; x < cgi_ptr_size; x++)
      if(cgi_ptr[x].name != NULL)
         break;

   if(x >= cgi_ptr_size)
   {
      cgi_next_read=cgi_ptr_size;
      *cgi_data=NULL;
      return(-1);
   }

   *cgi_data=&(cgi_ptr[x]);
   cgi_next_read=x+1;

   return(0);
}

////////////////////////////////////////////////////////////////////////////
// This points the pointer into the cgi array back to the start so
//    the next read is the first cgi name/value pair

int cgi_rewind(void)
{
   cgi_next_read=0;
   return(0);
}

////////////////////////////////////////////////////////////////////////////
//This function gets the post cgi data from stdin
//   returns 0 on success, 1 on no more data, -1 on failure

int acquire_post_data(cgi_t* cgi_data)
{
   size_t temp_size=0;
   ssize_t b_read=0;
   char* temp_ptr=NULL;
 
   if(user_env.content_length <= 0 )
      return(1);			//no data for us to read

   temp_size=0;

   b_read=getdelim(&(cgi_data->name),&temp_size,'=',stdin);

   if(b_read < 0 || cgi_data->name == NULL)
   {
      if( feof(stdin) )
         return(1);

      return(-1);
   }

   temp_ptr=strchr(cgi_data->name,'=');

   if(temp_ptr != NULL)
      *temp_ptr='\0';
   
   temp_size=0;

   b_read=getdelim(&(cgi_data->value),&temp_size,'&',stdin);

   if(b_read < 0 || cgi_data->value == NULL)
   {
      if( feof(stdin) )
         return(1);

      return(-1);			//we are at end of stream or out of mem
   }

   temp_ptr=strchr(cgi_data->value,'&');

   if(temp_ptr != NULL)
      *temp_ptr='\0';

   fix_cgi_string(cgi_data->name);
   fix_cgi_string(cgi_data->value);

   return(0);
}

////////////////////////////////////////////////////////////////////////////
//This function will get the cgi data from stdin
//   returns 0 on success &
//	malloc data_cgi->name
//	malloc data_cgi->value
//		if value is empty, will malloc(1) = '\0'
//	if filename exist
//		malloc data_cgi->filename
//		if filename != NULL, will malloc(1) = '\0' and value
//		file* will be a tmp file that is file read
//	if exist malloc data_cgi->cont_type
//	returns -1 on error

int acquire_post_multipart_data(cgi_t* cgi_data)
{
   char* file_buff=NULL;
   size_t file_buff_size=0;
   ssize_t b_read=0;

   char* tmp_ptr=NULL;
   char* tmp_ptr_end=NULL;

   if(feof(stdin) != 0)
      return(1);

   do
   {
      b_read=getline(&file_buff,&file_buff_size,stdin);

      if(b_read <= 0 || file_buff == NULL)
      {
         if(file_buff != NULL)
            free(file_buff);

         if(feof(stdin) != 0)
            return(1);

         return(-1);
      }
   }
   while(is_cgi_boundary(file_buff) == 0 || 
	 *file_buff == 0x0A || 
	 *file_buff == 0x0D);

//Content-Disposition header is required according to HTML v4.01

   if(strncasecmp(file_buff,"Content-Disposition",
		sizeof("Content-Disposition")-1) != 0)
   {
      syslog(LOG_MAIL | LOG_ERR,"cgi post type multipart/form-data has part "
		"that does not start with \"Content-Disposition\"");
      free(file_buff);
      return(-1);
   }

   if(get_parameter(file_buff,"name",&(cgi_data->name)) != 0)
   {
      syslog(LOG_MAIL | LOG_ERR,"cgi post type multipart/form-data has part "
		"that does not have name");
      free(file_buff);
      return(-1);
   }

   get_parameter(file_buff,"filename",&(cgi_data->filename));


   /* if part is not a file attachment, read it */
   if(cgi_data->filename == NULL || *(cgi_data->filename) == '\0')
   {
      do
      {
         b_read=getline(&file_buff,&file_buff_size,stdin);

         if(b_read < 0 || file_buff == NULL)
         {
            if(file_buff != NULL)
               free(file_buff);

            syslog(LOG_MAIL | LOG_ERR,"multipart/form submission ended "
			"abruptly");
            return(-1);
         }
      }
      while(*file_buff != 0x0A && *file_buff != 0x0D);


      if( get_form_text(&(cgi_data->value), user_env.boundary) != 0)
      {
         syslog(LOG_MAIL | LOG_ERR,"get_form_text failed");
         return(-1);
      }
   }

   /* this is a file attachment, save it as temp file */
   else     
   {
      do
      {
         b_read=getline(&file_buff,&file_buff_size,stdin);

         if(b_read < 0 || file_buff == NULL)
         {
            if(file_buff != NULL)
               free(file_buff);

            syslog(LOG_MAIL | LOG_ERR,"multipart/form submission ended "
			"abruptly");
            return(-1);
         }

         if(cgi_data->cont_type == NULL &&
                strncasecmp(file_buff,"Content-Type",
                      sizeof("Content-Type")-1) == 0)
         {
            tmp_ptr_end=strchr(file_buff,0x0A);
            if(tmp_ptr_end != NULL)
               *tmp_ptr_end='\0'; 

            tmp_ptr_end=strchr(file_buff,0x0D);
            if(tmp_ptr_end != NULL)
               *tmp_ptr_end='\0';


            tmp_ptr=file_buff+sizeof("Content-Type");
            tmp_ptr=strpcbrk(tmp_ptr,": \t");

            if(tmp_ptr == NULL)
            {
               syslog(LOG_MAIL | LOG_ERR,"cgi method post type " 
                                "multipart/form-data had NULL content type");
               free(file_buff);
               return(-1);
            }

            cgi_data->cont_type=strdup(tmp_ptr);

            if(cgi_data->cont_type == NULL)
            {
               syslog(LOG_MAIL | LOG_ERR,"Out of memory (%s)",strerror(errno));
               free(file_buff);
               return(-1);
            }
         }
      }
      while(*file_buff != 0x0A && *file_buff != 0x0D);


      if(cgi_data->cont_type == NULL)
      {
         cgi_data->cont_type=strdup("application/octet-stream");

         if(cgi_data->cont_type == NULL)
         {
            syslog(LOG_MAIL | LOG_ERR,"Out of memory (%s)",strerror(errno));
            return(-1);
         }
      }


      cgi_data->value=(char*)malloc(1);

      if(cgi_data->value == NULL)
      {
         syslog(LOG_MAIL | LOG_ERR,"Out of memory (%s)", strerror(errno));
         free(file_buff);
         return(-1);
      }

      *(cgi_data->value)='\0';

      free(file_buff);
      file_buff=NULL;

      if( *(cgi_data->filename) != '\0')
         if( get_form_file(cgi_data) != 0)
         {
            syslog(LOG_MAIL | LOG_ERR,"get_form_file failed");
            return(-1);
         }
   }

   return(0);
}

////////////////////////////////////////////////////////////////////////////
// This will read stdin up to boundary, malloc and save in *new_buff

int get_form_text(char** new_buff, char* boundary)
{
   char* file_buff=NULL;
   size_t file_buff_size=0;
   ssize_t b_read=0;
   char* tmp_ptr=NULL;
   size_t tmp_size=0;

   if(new_buff == NULL || boundary == NULL)
      return(-1);

   do
   {
      b_read=getline(&file_buff,&file_buff_size,stdin);

      if(b_read <= 0 || file_buff == NULL)
      {
         syslog(LOG_MAIL | LOG_ERR,"cgi post type multipart/form data ended "
	   	"abruptly (%s)",strerror(errno));
         if(file_buff != NULL)
               free(file_buff);
         return(-1);
      }
      
  
      if( (tmp_ptr=strchr(file_buff,0x0A)) != NULL)
         *tmp_ptr='\0';

      if( (tmp_ptr=strchr(file_buff,0x0D)) != NULL)
         *tmp_ptr='\0';

      if(*new_buff == NULL)
      {
         *new_buff=strdup(file_buff);

         if(*new_buff == NULL)
         {
            syslog(LOG_MAIL | LOG_ERR,"err allocating mem (%s)",
		strerror(errno));
            free(file_buff);
            return(-1);
         }
      } 
      else if(is_cgi_boundary(file_buff) != 0)
      {
         tmp_size=strlen(*new_buff)+sizeof("\n")+strlen(file_buff);
         tmp_ptr=(char*)realloc(*new_buff,tmp_size);

         if(tmp_ptr == NULL)
         {
            syslog(LOG_MAIL | LOG_ERR,"err allocating mem(%s)",
			strerror(errno));
            free(file_buff);
            return(-1);
         }
           
         *new_buff=tmp_ptr;
         strcat(*new_buff,"\n");
         strcat(*new_buff,file_buff);
      }
   }
   while(is_cgi_boundary(file_buff) != 0);

   if(file_buff != NULL)
      free(file_buff);

   return(0); 
}

////////////////////////////////////////////////////////////////////////////
// This will read stdin and save to a temp file. 
//    cgi_data->tmp_filename will be filename of temp file
//    cgi_data->file will be file handle of temp file

int get_form_file(cgi_t* cgi_data)
{
   mode_t old_mask=0;

   if(cgi_data == NULL)
      return(-1);

   cgi_data->tmp_filename=create_tmp_filename();

   if(cgi_data->tmp_filename == NULL)
   {
      syslog(LOG_MAIL | LOG_ERR,"invalid tmp_filename");
      return(-1);
   }

   old_mask=umask(0177);

   setuid(euid);
   cgi_data->tmp_file=fopen(cgi_data->tmp_filename,"w+");
   setuid(ruid);

   if(cgi_data->tmp_file == NULL)
   {
      syslog(LOG_MAIL | LOG_ERR,"could not create tmp file (%s)",
			strerror(errno));
      umask(old_mask);
      return(-1);
   }

   umask(old_mask);

   if( get_form_file_save(cgi_data->tmp_file, user_env.boundary) != 0)
   {
      syslog(LOG_MAIL | LOG_ERR,"could not save tmp file");
      return(-1);
   }

   return(0);
}

////////////////////////////////////////////////////////////////////////////

int get_form_file_save(FILE* tmp_file, char* boundary)
{
   int char_read=0;

   char* buff_compare=NULL;
   size_t hold_size=0;
   size_t hold=0;
   size_t x=0;

   if(tmp_file == NULL || boundary == NULL)
   {
      syslog(LOG_MAIL | LOG_ERR,"get_form_file_save passed NULL arg");
      return(-1);
   }

   hold_size=strlen(boundary)+sizeof("\x0d\x0a--");
   buff_compare=(char*) malloc(hold_size);

   if(buff_compare == NULL)
   {
      syslog(LOG_MAIL | LOG_ERR,"Out of memory (%s)",strerror(errno));
      return(-1);
   }

   strcpy(buff_compare,"\x0d\x0a--");
   strcat(buff_compare,boundary);

   hold=0;

   do
   {
      char_read=getchar();

      if(char_read != EOF)
      {
         if(hold < (hold_size-1) )
         {
            if(char_read == buff_compare[hold])
               hold++;
            else
            {
               if(hold > 0)
               {
                  for(x=0;x<hold;x++)
                     if( fputc(buff_compare[x],tmp_file) == EOF)
                     {
                        syslog(LOG_MAIL | LOG_ERR,"fputc tmp_file failed (%s)"
				,strerror(errno));
                        free(buff_compare);
                        buff_compare=NULL;
                        return(-1);
                     }

                  hold=0;

                  if(char_read == buff_compare[hold])
                     hold++;
                  else
                     if( fputc(char_read,tmp_file) == EOF)
                     {
                        syslog(LOG_MAIL | LOG_ERR,"fputc tmp_file failed (%s)"
				,strerror(errno));
                        free(buff_compare);
                        buff_compare=NULL;
                        return(-1);
                     }
               }
               else
                  putc(char_read,tmp_file);
            }
         }
         else
            char_read=EOF;
      }
   }
   while(char_read != EOF);

   if(hold < hold_size-1)
   {
      syslog(LOG_MAIL | LOG_ERR,"No end boundary during send_message.");
      return(-1);
   }

   char_read=getchar();

   if(char_read == '-')
   {
      char_read=getchar();

      if(char_read != '-');
         ungetc(char_read,stdin);
   }
   else 
      ungetc(char_read,stdin);
      

   free(buff_compare);

   return(0);
}

////////////////////////////////////////////////////////////////////////////

char* create_tmp_filename(void)
{
   char* filename=NULL;
   size_t filename_size=0;
   DIR* tmp_dir=NULL;
   struct dirent* tmp_dir_file=NULL;
   int tmp_num=0;
   int new_num=1;
   char* tmp_ptr=NULL;

   if(user_ses.sessionid == NULL)
      find_action_and_session();

   if(user_ses.sessionid == NULL)
   {
      syslog(LOG_MAIL | LOG_ERR,"Invalid sessionid");
      syslog(LOG_MAIL | LOG_ERR,"Browser did not send form data in proper "
			"sequence. Expected sessionid before file.");
      return(NULL);
   }

   setuid(euid);
   tmp_dir=opendir(TMPDIR);
   setuid(ruid);

   if(tmp_dir == NULL)
   {
      syslog(LOG_MAIL | LOG_ERR,"could not open tempdir for reading (%s)",
				strerror(errno));
      return(NULL);
   }

   filename_size=sizeof(TMPDIR "/" TMPPREFIX ".0000" )
		 +strlen(user_ses.sessionid);
   filename=(char*) malloc(filename_size);

   if(filename == NULL)
   {
      syslog(LOG_MAIL | LOG_ERR,"Out of memory (%s)",strerror(errno));
      return(NULL);
   }

   strcpy(filename,TMPPREFIX);
   strcat(filename,user_ses.sessionid);

   do
   {
      tmp_dir_file=readdir(tmp_dir);

      if(tmp_dir_file != NULL && tmp_dir_file->d_name != NULL)
      {
         if( strncmp(filename,tmp_dir_file->d_name,strlen(filename)) == 0)
         {
            tmp_ptr=strchr(tmp_dir_file->d_name,'.');

            if(tmp_ptr != NULL)
            {
               tmp_ptr++;
               tmp_num=strtol(tmp_ptr,NULL,10);

               if(tmp_num >= new_num)
                  new_num=tmp_num+1;
            }
         }
      }
   }
   while(tmp_dir_file != NULL);

   closedir(tmp_dir);

   snprintf(filename,filename_size,
		TMPDIR "/" TMPPREFIX "%s.%d",
		user_ses.sessionid,new_num);

   return(filename);
}

////////////////////////////////////////////////////////////////////////////
//This will find the parameter parm_name in buff and malloc a copy of
//   it and save it in *save_buff
//   returns 0 on success
//   returns -1 on failure

int get_parameter(char* buff, char* parm_name, char** save_buff)
{
   char* tmp_ptr=NULL;
   char* start_ptr=NULL;
   char* end_ptr=NULL;
   size_t size=0;

   if(buff == NULL || parm_name == NULL || save_buff == NULL)
      return(-1);

   *save_buff=NULL;

   tmp_ptr=buff;

   do
   {
      tmp_ptr=strchr(tmp_ptr,';');

      if(tmp_ptr == NULL)
         return(-1);

      tmp_ptr++;
      tmp_ptr=strpcbrk(tmp_ptr," \t");

      if(tmp_ptr == NULL)
         return(-1);

   }
   while(strncasecmp(tmp_ptr,parm_name,strlen(parm_name)) != 0);

   tmp_ptr+=strlen(parm_name);

   tmp_ptr=strpcbrk(tmp_ptr," \t");

   if(tmp_ptr == NULL)
      return(-1);

   if(*tmp_ptr != '=')
      return(-1);

   tmp_ptr++;

   start_ptr=strpcbrk(tmp_ptr," \t");

   if(start_ptr == NULL)
      return(-1);

   if(*start_ptr == '"')
   {
      start_ptr++;

      end_ptr=strchr(start_ptr,'"');

      if(end_ptr != NULL)
         size=end_ptr-start_ptr;
      else
         size=strlen(start_ptr);
   }
   else
   {
      end_ptr=strchr(start_ptr,';');

      if(end_ptr != NULL)
         size=end_ptr-start_ptr;
      else
         size=strlen(start_ptr);
   }

   *save_buff=(char*)malloc(size+1);

   if(*save_buff == NULL)
   {
      syslog(LOG_MAIL | LOG_ERR,"Out of memory (%s)",strerror(errno));
      return(-1);
   }

   strncpy(*save_buff,start_ptr,size);
   *((*save_buff)+size)='\0';
  
   return(0); 
}

////////////////////////////////////////////////////////////////////////////
//This will determine if the string pointed to by buff is boundary
//   returns 0 if boundary
//   returns -1 if not boundary

int is_cgi_boundary(char* buff)
{
   if(buff == NULL)
      return(-1);
   if(strlen(buff) < (strlen(user_env.boundary)+sizeof("--")-1) )
      return(-1);

   if(buff[0] != '-' || buff[1] != '-')
      return(-1);

   if(strncmp(buff+2,user_env.boundary,strlen(user_env.boundary)) != 0)
      return(-1);

   return(0);
}

////////////////////////////////////////////////////////////////////////////
//This function will determine if the next read from stdin is a boundary
//	returns 0 if it is
//		will read terminating '\n'
//	returns 1 is ending boundary
//	returns 2 if not
//		sets buff to bytes read from stdin
//		sets buff_size to num bytes read from stdin
//	returns -1 on error

int next_is_cgi_boundary(char** buff, size_t* size_read)
{
   *buff=NULL;
   *size_read=0;
   size_t buff_size=0;
   int char_read=0;

   if(user_env.boundary == NULL)
      return(-1);

   buff_size=strlen(user_env.boundary)+sizeof("----")-1;
   *buff=(char*)malloc(buff_size);
 
   if(*buff == NULL)
   {
      syslog(LOG_MAIL | LOG_ERR,"%s",strerror(errno));
      return(-1);
   }

   if((char_read=getchar()) == EOF)
   {
      free(*buff);
      *buff=NULL;
      *size_read=0;
      return(-1);
   } 

   (*buff)[(*size_read)++]=char_read;

   if(char_read != '-')
      return(2);

   if((char_read=getchar()) == EOF)
   {
      free(*buff);
      *buff=NULL;
      *size_read=0;
      return(-1);
   } 

   (*buff)[(*size_read)++]=char_read;

   if(char_read != '-')
      return(2);


   do
   {
      if((char_read=getchar()) == EOF)
      {
         free(*buff);
         *buff=NULL;
         *size_read=0;
         return(-1);
      }

      (*buff)[(*size_read)++]=char_read;
      if(char_read != user_env.boundary[(*size_read)-3])
         return(2);
   }
   while( (*size_read) < (buff_size - (sizeof("--")-1) ) );

   if((char_read=getchar()) == EOF)
   {
      free(*buff);
      *buff=NULL;
      *size_read=0;
      return(-1);
   }
   
   (*buff)[(*size_read)++]=char_read;

   if(char_read != '\n' && char_read != '-')
      return(2);


//we found our boundary

   if(char_read == '\n')
   {
      free(buff);
      buff=NULL;
      *size_read=0;
      return(0);
   }
      
//ending boundary ???

   if((char_read=getchar()) == EOF)
   {
      free(*buff);
      *buff=NULL;
      *size_read=0;
      return(-1);
   }
   
   (*buff)[(*size_read)++]=char_read;

   if(char_read != '-')
      return(2);

//we found ending boundary

   free(*buff);
   *buff=NULL;
   *size_read=0;
   return(1);

}

////////////////////////////////////////////////////////////////////////////
//This function will get the cgi data from environment variable QUERY_STRING
//   on success
//	returns 0
//	malloc data_cgi->name
//	malloc data_cgi->value
//		if value does not exist, will malloc(1) & set == '\0'
//	both data_cgi.name && data_cgi.value will be non-null on success
//  on error
//	returns -1

int acquire_get_data(cgi_t* cgi_data)
{

   static char* ptr_to_next=NULL;
   static char* ptr_to_start=NULL;
   char* ptr_to_equal=NULL;

   ssize_t temp_size=0;

   if(ptr_to_next == NULL && ptr_to_start != NULL)
      return(1);			//we have read everything

   if(ptr_to_next == NULL)
      ptr_to_next=user_env.query_string;

   if(ptr_to_next == NULL)
      return(1);

   ptr_to_start=ptr_to_next;

   ptr_to_equal=strchr(ptr_to_start,'=');

   if(ptr_to_equal == NULL)
      return(1);

   ptr_to_next=strchr(ptr_to_equal,'&');

   temp_size=ptr_to_equal-ptr_to_start;

   if(temp_size < 0)
      return(-1);

   cgi_data->name=(char*)malloc(temp_size+1);

   if(cgi_data->name == NULL)
   {
      syslog(LOG_MAIL | LOG_ERR,"Out of memory (%s)",strerror(errno));
      return(-1);
   }

   strncpy(cgi_data->name,ptr_to_start,temp_size);
   cgi_data->name[temp_size]='\0';

   ptr_to_start=ptr_to_equal+1;

   temp_size=ptr_to_next == NULL ? 
		strlen(ptr_to_start) : 
		ptr_to_next-ptr_to_start;

   if(temp_size < 0)
   {
      return(-1);
   }

   cgi_data->value=(char*)malloc(temp_size+1);

   if(cgi_data->value == NULL)
   {
      syslog(LOG_MAIL | LOG_ERR,"Out of memory (%s)",strerror(errno));
      return(-1);
   }

   strncpy(cgi_data->value,ptr_to_start,temp_size);
   cgi_data->value[temp_size]='\0';

   if(ptr_to_next != NULL)
      ptr_to_next++;

   fix_cgi_string(cgi_data->name);
   fix_cgi_string(cgi_data->value);

   return(0);
}

////////////////////////////////////////////////////////////////////////////

int fix_cgi_string(char* buff)
{
   char* temp_ptr=NULL;
   char* move_ptr=NULL;
   char conv_char=0;

   if(buff == NULL)
      return(-1);

   temp_ptr=buff;

   do
   {
      temp_ptr=strchr(buff,'+');

      if(temp_ptr != NULL)
         *temp_ptr=' ';
   }
   while(temp_ptr != NULL);

   temp_ptr=buff;

   do
   {
      temp_ptr=strchr(temp_ptr,'%');

      if(temp_ptr != NULL && strlen(temp_ptr) >= 3)
      {
         conv_char=conv_hex(*(temp_ptr+1)) << 4;
         conv_char|=conv_hex(*(temp_ptr+2));

         *temp_ptr=conv_char;

         move_ptr=temp_ptr+1;

         while(*(move_ptr+2) != '\0')
         {
            *move_ptr=*(move_ptr+2);
            move_ptr++;
         }
         *move_ptr='\0';
      }
   }
   while(temp_ptr != NULL);

   return(0);
}

////////////////////////////////////////////////////////////////////////////

char conv_hex(char c)
{
   if(c >= '0' && c <= '9')
      return(c - '0');
   if(c >= 'A' && c <= 'F')
      return(c - 'A' + 10);
   if(c >= 'a' && c <= 'f')
      return(c - 'a' + 10);

   return(0);
}

////////////////////////////////////////////////////////////////////////////

int init_cgi_struct(cgi_t* cgi_data)
{
   cgi_data->name=NULL;
   cgi_data->value=NULL;
   cgi_data->cont_type=NULL;
   cgi_data->filename=NULL;
   cgi_data->tmp_file=NULL;
   cgi_data->tmp_filename=NULL;

   return(0);
}

////////////////////////////////////////////////////////////////////////////

