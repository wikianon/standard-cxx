/*
	mail_process.cpp
*/

#define _GNU_SOURCE

#define BLOCK_MOVE_SIZE		1024

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <syslog.h>
#include <sys/stat.h>

#include "mail_process.h"
#include "session.h"
#include "main.h"
#include "config.h"
#include "custom_string.h"

int buffer_allocate(size_t, size_t, size_t*, void**);
int free_buffers(void**);

/////////////////////////////////////////////////////////////////////////
// This will allocate a buffer. 
//    It tries to be intelligent and select the most optimum size. 
//    If there is a minimum requirement, this will be met. 
// Returns 0 on success

struct buff_optimize_defination {
   size_t approx_size;
   size_t buff_size;
}

const buff_optimize[]=
{
   { 1024*100     , 1024*1       },
   { 1024*1024*1  , 1024*10      },
   { 1024*1024*10 , 1024*100     },
   { 1024*1024*100, 1024*1024*1  },
   { 0            , 1024*1024*10 }
};



int buffer_allocate(ssize_t min_size,
                    size_t remaining,
                    size_t* size, 
                    void** buff)
{
   int x=0;

   if(buff == NULL || size == NULL)
      return(-1);

   buff[0]=NULL;
   buff[1]=NULL;
   *size=0;

   if(remaining == 0 || size == NULL || buff == NULL)
      return(-1);

   if(min_size > 0)
      for(x=0; buff_optimize[x].approx_size != 0 &&
            buff_optimize[x].buff_size < (size_t)min_size; 
          x++);

   for(; buff_optimize[x].approx_size != 0 &&
         remaining >= buff_optimize[x].approx_size;
       x++);

   *size=buff_optimize[x].buff_size;

   buff[0]=malloc(*size);

   if(buff[0] == NULL)
   {
      syslog(LOG_MAIL | LOG_ERR,"Error trying to allocate %d bytes (%s)",
				*size,strerror(errno));
      *size=0;
      return(-1);
   }

   buff[1]=malloc(*size);

   if(buff[1] == NULL)
   {
      syslog(LOG_MAIL | LOG_ERR,"Error trying to allocate %d bytes (%s)",
				*size,strerror(errno));
      free(buff[0]);
      buff[0]=NULL;
      *size=0;
      return(-1);
   }

   return(0);   
}

/////////////////////////////////////////////////////////////////////////
//This function will point to the message number as offset from the
//      start of user_file
//   returns 0 on success

int point_to_message(FILE* user_file, long int message_num)
{
   long int current_num=0;
   int err=0;

   char* file_buff=NULL;
   size_t file_buff_size=0;
   ssize_t b_read=0;

   long int current_file_pointer=0;

   enum status_ptm { ptm_nl, ptm_pending, ptm_error };

   status_ptm pt_to_msg_status=ptm_nl;

   if(user_file == NULL || message_num <= 0)
      return(-1);		//sanity check

   rewind(user_file);		//point to start of file

   do
   {
      b_read=0;

      if(pt_to_msg_status == ptm_nl)
         current_file_pointer=ftell(user_file);

      b_read=getline(&file_buff,&file_buff_size,user_file);

      if(b_read <= 0 || file_buff == NULL)
         pt_to_msg_status=ptm_error;

      else if(*file_buff == '\n')
         pt_to_msg_status=ptm_nl;

      else if(pt_to_msg_status == ptm_nl)
      {
         if(strncmp(file_buff,"From ",5) == 0)
            current_num++;		//this is a new message
            
         pt_to_msg_status=ptm_pending;
      }

   }
   while( (pt_to_msg_status == ptm_pending || pt_to_msg_status == ptm_nl)
		 && current_num < message_num);


   if(file_buff != NULL)
      free(file_buff);

   if(current_file_pointer != -1 
	&& current_num == message_num)
      err=fseek(user_file,current_file_pointer,SEEK_SET);
				//set file pointer to start of message
   else
      err=-1;

   return(err);
}

/////////////////////////////////////////////////////////////////////////
//This function will point to the next message that starts with
//   "\nFrom "
//    file pointer will point to the F in From if found
//    returns 0 on success
//    returns 1 on error
//    if size_read != NULL, will += size read

int point_to_next_message(FILE* user_mail_file, ssize_t* size_read)
{
   char* file_buff=NULL;
   size_t file_buff_size=0;
   ssize_t b_read=0;
   ssize_t remaining_size_of_message=0;
   long int file_pointer=0;

   enum head_found_enum {
      hf_pending,
      hf_nl,
      hf_error_eof,
      hf_found
   };

   enum head_found_enum head_found=hf_pending;

   if(user_mail_file == NULL || feof(user_mail_file))
      return(-1);			//sanity check

   do
   {
      b_read=getline(&file_buff,&file_buff_size,user_mail_file);

      if(b_read <= 0 || file_buff == NULL)
         head_found=hf_error_eof;
      else
      {
         if(*file_buff == '\n')
         {
            head_found=hf_nl;
            file_pointer=ftell(user_mail_file);
         }
         else if(head_found == hf_nl)
         {
            if( strncmp(file_buff,"From ",sizeof("From ")-1) == 0)
               head_found=hf_found;
            else
               head_found=hf_pending;
         }
      }

      if(head_found != hf_error_eof && head_found != hf_found)
         remaining_size_of_message+=b_read;

   }
   while(head_found == hf_pending || head_found == hf_nl);

   if(file_buff != NULL)
      free(file_buff);

   if(size_read != NULL)
      *size_read+=remaining_size_of_message;

   if(head_found == hf_found)
   {
      fseek(user_mail_file,file_pointer,SEEK_SET);
      return(0);
   }
 
   return(-1);

}

/////////////////////////////////////////////////////////////////////////
//Updates message pointed to by current file pointer to indicate status
//   read. Leaves file pointer where it was initially.

int update_status_read(FILE* user_file, long int message_num)
{
   int err=0;

   char* file_buff=NULL;
   size_t file_buff_size=0;
   ssize_t b_read=0;

   long int file_pointer_1=0;
   long int file_pointer_2=0;
   long int file_pointer_save=0;

   enum pointer_status { 
      pending, 
      found_status, 
      new_line,
      error,
   } ptr_status;

   ptr_status=pending;

   if(user_file == NULL)
      return(-1);

   file_pointer_save=ftell(user_file);

   do
   {
      file_pointer_1=ftell(user_file);

      b_read=getline(&file_buff,&file_buff_size,user_file);

      if(b_read <= 0 || file_buff == NULL)
         ptr_status=error;

      else
      {
         if(*file_buff == '\n')
            ptr_status=new_line;
         else if(strncmp("Status: ",file_buff,sizeof("Status:")) == 0)
            ptr_status=found_status;
      }
   }
   while(ptr_status == pending);

   if(ptr_status == found_status && strlen(file_buff) > 8)
   {
      if(strchr(file_buff+8,'R') == 0)
      {
         free(file_buff);
         return(0);
      }
   }

   if(file_buff != NULL)
      free(file_buff);

   if(err != 0)
      ptr_status=error;
 

   if(ptr_status == error)
      return(-1);

   if(ptr_status == found_status)
      file_pointer_2=ftell(user_file);
   else
      file_pointer_2=file_pointer_1;


   if( insert_line_in_file(user_file,
			   "Status: RO\n",
			   file_pointer_2,	//start reading at
			   file_pointer_1)	//start writing at
	!= 0)
   {
      syslog(LOG_MAIL | LOG_ERR,"error updating mail status for %s",
				user_ses.loginname);
      return(-1);
   }

   fclose(user_file);

   return(0);
}

/////////////////////////////////////////////////////////////////////////
//This will open a users mailfile.
//   returns NULL on failure
//   on error, sets errno to 0 if file not exist, -1 on failure

FILE* open_user_mailfile(char* open_status)
{
   char* mail_filename=NULL;
   FILE* user_mail_file=NULL;

   mail_filename=create_user_filename();

   if(mail_filename == NULL)
   {
      errno=-1;
      return( (FILE*) NULL);
   }

   if(file_exists(mail_filename) == 0)
   {
      errno=0;
      return( (FILE*) NULL);
   }

   user_mail_file=fopen(mail_filename,open_status);

   if(user_mail_file == NULL)
      syslog(LOG_MAIL | LOG_ERR,"could not open %s mailfile (%s)",
			user_ses.loginname,strerror(errno));
       
   free(mail_filename);

   errno=-1;

   return(user_mail_file);
}

/////////////////////////////////////////////////////////////////////////

char* create_user_filename(void)
{
   char* filename=NULL;
   size_t filename_size=0;

   if(user_ses.loginname == NULL)
      return(NULL);		//sanity check

   filename_size=sizeof(MAILSPOOLDIR "/")+strlen(user_ses.loginname);

   if( (filename=(char*)malloc(filename_size)) == NULL)
   {
      syslog(LOG_MAIL | LOG_ERR,"out of memory (%s)",strerror(errno));
      return(NULL);
   }

   strcpy(filename,MAILSPOOLDIR "/");
   strcat(filename,user_ses.loginname);

   return(filename);
}

/////////////////////////////////////////////////////////////////////////
//This function will insert a line in a file, OR it can delete lines
//   message_line is a string to insert into the file, or can be NULL
//      if no line to write
//   file_pointer write is where to start writing the message at
//   file_pointer read is where to start reading lines from to append
//      after message_line has been written
//   if no line is to be deleted, file_pointer_read should equal 
//      file_pointer_write
//   file_pointer_write should not be greater than file_pointer_read
//      otherwise we would be writing into our data to read and would
//      be in a paradox, inserting characters that we will reinsert in an
//      endless loop

int insert_line_in_file(FILE* user_file,
			char* message_line,
			long int file_ptr_read,
			long int file_ptr_write)
{
   void* buff[3];
   size_t buff_size[2];
   size_t size=0;

   long int file_ptr_end=0;
   int fdes=-1;

   ssize_t min_size=0;
   size_t remaining_size=0;

   buff[0]=NULL;
   buff[1]=NULL;
   buff[2]=NULL;
   buff_size[0]=0;
   buff_size[1]=0;

					//we cannot write into what we're
					//   reading
   if(user_file == NULL)
   {
      syslog(LOG_MAIL | LOG_ERR,"insert_line passed NULL pointer");
      return(-1);
   }

   if(file_ptr_write > file_ptr_read)
   {
      syslog(LOG_MAIL | LOG_ERR,"insert line in file error, bad pointers");
      return(-1);
   }


   min_size=file_ptr_write;

   if(message_line != NULL)
      min_size+=strlen(message_line);

   if(min_size > file_ptr_read)
      min_size-=file_ptr_read;
   else
      min_size=0;

   if( fseek(user_file,0,SEEK_END) != 0)
   {
      syslog(LOG_MAIL | LOG_ERR,"fseek failed (%s)",strerror(errno));
      return(-1);
   }

   if( (file_ptr_end=ftell(user_file)) == -1)
   {
      syslog(LOG_MAIL | LOG_ERR,"ftell failed (%s)",strerror(errno));
      return(-1);
   }

   if(file_ptr_end < file_ptr_read)
   {
      syslog(LOG_MAIL | LOG_ERR,"invalid file pointers");
      return(-1);
   }

   remaining_size=file_ptr_end-file_ptr_read;


// min_size and remaining_size have been computed. If necessary, allocate
//    buffers


// if no remaining size, no need to allocate buffers and read data. Just
//    write new data and exit

   if(remaining_size == 0)
   {
      if( (fseek(user_file,file_ptr_write,SEEK_SET)) != 0)
      {
         syslog(LOG_MAIL | LOG_ERR,"fseek failed (%s)",strerror(errno));
         return(-1);
      }

      if(message_line != NULL)
         if( (fprintf(user_file,"%s",message_line)) < 0)
         {
            syslog(LOG_MAIL | LOG_ERR,"file write error (%s)",strerror(errno));
            return(-1);
         }

      if( (file_ptr_end=ftell(user_file)) == -1)
      {
         syslog(LOG_MAIL | LOG_ERR,"ftell failed (%s)",strerror(errno));
         return(-1);
      }

      if( (fdes=fileno(user_file)) == -1)
      {
         syslog(LOG_MAIL | LOG_ERR,"fileno failed (%s)",strerror(errno));
         return(-1);
      }

      if( ftruncate(fdes,file_ptr_end) != 0)
      {
         syslog(LOG_MAIL | LOG_ERR,"ftruncate failed (%s)",strerror(errno));
         return(-1);
      }
      
      return(0);
   }


//allocate mem

   if( (buffer_allocate(min_size, remaining_size, &size, buff)) != 0)
   {
      syslog(LOG_MAIL | LOG_ERR,"buffer allocation failed");
      return(-1);
   }




// read first buff

   if(fseek(user_file,file_ptr_read,SEEK_SET) != 0)
   {
      syslog(LOG_MAIL | LOG_ERR,"fseek failed (%s)",strerror(errno));
      free_buffers(buff);
      return(-1);
   }

   buff_size[0]=fread(buff[0], (size_t) 1, size , user_file);

   if(buff_size[0] < size && feof(user_file) == 0)
   {
      syslog(LOG_MAIL | LOG_ERR,"fread error (%s)",strerror(errno));
      free_buffers(buff);
      return(-1);
   }

   if( (file_ptr_read=ftell(user_file)) == -1)
   {
      syslog(LOG_MAIL | LOG_ERR,"ftell failed (%s)",strerror(errno));
      free_buffers(buff);
      return(-1);
   }

// point to write

   if( fseek(user_file,file_ptr_write,SEEK_SET) != 0)
   {
      syslog(LOG_MAIL | LOG_ERR,"fseek failed (%s)",strerror(errno));
      free_buffers(buff);
      return(-1);
   }


// write message if applicable

   if(message_line != NULL)
   {
      if( fprintf(user_file,"%s",message_line) < 0)
      {
         syslog(LOG_MAIL | LOG_ERR,"insert line in file failed (%s)",
				strerror(errno));
         free_buffers(buff);
         return(-1);
      }

      if( (file_ptr_write=ftell(user_file)) == -1)
      {
         syslog(LOG_MAIL | LOG_ERR,"ftell failed (%s)",strerror(errno));
         free_buffers(buff);
         return(-1);
      }
   }




// start moving data blocks

   while(buff_size[0] > 0)
   {

// read next block if not finished

      if(buff_size[0] == size)
      {
         if( fseek(user_file,file_ptr_read,SEEK_SET) != 0)
         {
            syslog(LOG_MAIL | LOG_ERR,"fseek failed (%s). "
		"Possible file corruption",strerror(errno));
            free_buffers(buff);
            return(-1);
         }

         buff_size[1]=fread(buff[1], (size_t) 1, size, user_file);

         if(buff_size[1] < size && feof(user_file) == 0)
         {
            syslog(LOG_MAIL | LOG_ERR,"fread failed (%s). "
		"Possible file corruption",strerror(errno));
            free_buffers(buff);
            return(-1);
         }
         else if(buff_size[1] == size)
         {
            if( (file_ptr_read=ftell(user_file)) == -1)
            {
               syslog(LOG_MAIL | LOG_ERR,"ftell failed (%s). "
			"Possible file corruption.",strerror(errno));
               free_buffers(buff);
               return(-1);
            }
         }
      }
      else
         buff_size[1]=0;

// write old buff

      if( fseek(user_file,file_ptr_write,SEEK_SET) == -1)
      {
         syslog(LOG_MAIL | LOG_ERR,"fseek failed (%s)",strerror(errno));
         free_buffers(buff);
         return(-1);
      }

      if( fwrite(buff[0], (size_t) 1, buff_size[0], user_file) 
		< buff_size[0])
      {
         syslog(LOG_MAIL | LOG_ERR,"fwrite failed (%s). "
			"Possible file corruption.",strerror(errno));
         free_buffers(buff);
         return(-1);
      }
      
      if( (file_ptr_write=ftell(user_file)) == -1)
      {
         syslog(LOG_MAIL | LOG_ERR,"ftell failed (%s). "
			"Possible file corruption.",strerror(errno));
         free_buffers(buff);
         return(-1);
      }

// exchange data blocks

      buff[2]=buff[0];
      buff[0]=buff[1];
      buff[1]=buff[2];

      buff_size[0]=buff_size[1];
   }



// done, cleanup and exit

   free_buffers(buff);
   fflush(user_file);

   if( (fdes=fileno(user_file)) == -1)
   {
      syslog(LOG_MAIL | LOG_ERR,"fileno failed (%s)."
		"Possible file corruption.",strerror(errno));
      return(-1);
   }

   if( (ftruncate(fdes,file_ptr_write)) != 0)
   {
      syslog(LOG_MAIL | LOG_ERR,"ftruncate failed (%s). "
		"Possible file corruption.",strerror(errno));
      return(-1);
   }

   return(0);
}

/////////////////////////////////////////////////////////////////////////

int free_buffers(void** buff)
{
   if(buff == NULL)
      return(-1);

   if(buff[0] != NULL)
   {
      free(buff[0]);
      buff[0]=NULL;
   }

   if(buff[1] != NULL)
   {
      free(buff[1]);
      buff[1]=NULL;
   }

   return(0);
}

/////////////////////////////////////////////////////////////////////////
// This function check to see if a file exists
//    returns 1 if file exists
//    returns 0 if file does not exist
//    returns -1 on error


int file_exists(char* filename)
{
   int err=0;
   struct stat file_status;

   if(filename == NULL)
      return(-1);

   err=stat(filename,&file_status);

   if(err == 0)
      return(1);

   if(errno == ENOENT)
      return(0);

   syslog(LOG_MAIL | LOG_ERR,"stat failed for %s (%s)",
			filename,strerror(errno));

   return(-1);
}

/////////////////////////////////////////////////////////////////////////
//This function will eliminate any linear whitespace (space, tab, quotes)
//   at the beginning or end of the string. If there are no valid
//   characters in the string, it will free the string and return
//   null, otherwise it will return the new string.
//   It is safe to pass a NULL string.

char* eliminate_lwsp(char* buff)
{
   char* start_ptr=NULL;
   char* tmp_ptr=NULL;

   if(buff == NULL)
      return(NULL);

   tmp_ptr=strrpcbrk(buff," \t\"'\x0d\x0a");

   if(tmp_ptr != NULL)
      *(tmp_ptr+1)='\0';

   tmp_ptr=strpcbrk(buff," \t\"'");

   if(tmp_ptr == NULL)
   {
      free(buff);
      return(NULL);
   }

   start_ptr=buff;

   while(*tmp_ptr != '\0')
   {
      *start_ptr=*tmp_ptr;
      start_ptr++;
      tmp_ptr++;
   }

   *start_ptr='\0';

   return(buff);
}

//////////////////////////////////////////////////////////////////


