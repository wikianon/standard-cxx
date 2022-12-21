/*
	session.cpp
*/


#define _GNU_SOURCE

#include <sys/types.h>
#include <shadow.h>
#include <unistd.h>
#include <syslog.h>
#include <pwd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>		//time
#include <stdio.h>		//asprintf
#include <sys/stat.h>		//stat
#include <syslog.h>
#include <errno.h>
#include <dirent.h>
#include <time.h>
#include <signal.h>
#include <sys/wait.h>

#include "main.h"
#include "session.h"
#include "environment.h"
#include "config.h"
#include "mail_process.h"

session_t user_ses;

char* create_ses_filename(void);
int get_session_info_from_file(FILE*);
int create_session_file(void);

int destroy_passwd(char*); 

///////////////////////////////////////////////////////////////////////
//This will check to see if the user is has already authenticated.
//   user_ses.sessionid is required
//   returns 0 on success

int check_session(void)
{
   char* ses_filename=NULL;
   FILE* ses_file=NULL;

   if(user_ses.sessionid == NULL || user_env.remote_addr == NULL)
      return(-1);

   if( (ses_filename=create_ses_filename()) == NULL)
      return(-1);

   setuid(euid);

   if(file_exists(ses_filename) != 1)
   {
      setuid(ruid);

      syslog(LOG_MAIL | LOG_INFO,"%s tried to open non-existent session %s",
		user_env.remote_addr,user_ses.sessionid);
      free(ses_filename);
      return(2);
   }

   ses_file=fopen(ses_filename,"r");
   setuid(ruid);

   free(ses_filename);
   ses_filename=NULL;


   if(ses_file == NULL)
   {
      syslog(LOG_MAIL | LOG_ERR,"Could open ses file (%s)",strerror(errno));
      return(-1);
   }

   if(get_session_info_from_file(ses_file) != 0)
   {
      fclose(ses_file);
      return(-1);
   }

   fclose(ses_file);

   if (strcmp(user_ses.remoteip,user_env.remote_addr) != 0)
   {
      syslog(LOG_MAIL | LOG_INFO,
		"%s tried to open session he didn't own.",
		user_env.remote_addr);
      return(-1);
   }

   return(0);
}

///////////////////////////////////////////////////////////////////////
//This will read the session info from a session file
//   user_ses.sessionid is required
//   user_ses.name is filled in
//   user_ses.remoteip is filled in and verified
//
//   returns 0 on success, -1 on failure

int get_session_info_from_file(FILE* ses_file)
{
   char* file_buff=NULL;
   size_t file_buff_size=0;
   ssize_t b_read=0;

   char* parameter_start=NULL;

   enum read_status { rs_pending, rs_done };

   read_status status=rs_pending;

   if(ses_file == NULL || user_ses.sessionid == NULL)
      return(-1);

   do
   {
      b_read=getline(&file_buff,&file_buff_size,ses_file);

      if(b_read <= 0 || file_buff == NULL)
      {
         if(file_buff != NULL)
            free(file_buff);
         status=rs_done;
      }
      else 
      {
         parameter_start=strchr(file_buff,'\n');

         if(parameter_start != NULL)
            *parameter_start='\0';

         parameter_start=strchr(file_buff,'=');

         if(parameter_start != NULL)
         {
            parameter_start++;

            if(strlen(parameter_start) <= 0)
               parameter_start=NULL;
         }
      }

      if(status == rs_pending && parameter_start != NULL)
      {
         if(strncmp(file_buff,"loginname",sizeof("loginname")-1) == 0 &&
			user_ses.loginname == NULL)
         {
            if( (user_ses.loginname=strdup(parameter_start)) == NULL)
            {
               syslog(LOG_MAIL | LOG_ERR,"Out of memory (%s)",strerror(errno));
               destroy_user_ses();
               return(-1);
            }
         }
         else if(strncmp(file_buff,"name",sizeof("name")-1) == 0 &&
			user_ses.name == NULL)
         {
            if( (user_ses.name=strdup(parameter_start)) == NULL)
            {
               syslog(LOG_MAIL | LOG_ERR,"Out of memory (%s)",strerror(errno));
               destroy_user_ses();
               return(-1);
            }
         }
         else if(strncmp(file_buff,"remoteip",sizeof("remoteip")-1) == 0 &&
			user_ses.remoteip == NULL)
         {
            if( (user_ses.remoteip=strdup(parameter_start)) == NULL)
            {
               syslog(LOG_MAIL | LOG_ERR,"Out of memory (%s)",strerror(errno));
               destroy_user_ses();
               return(-1);
            }
         }
      }
   }
   while(status == rs_pending);


   if(user_ses.loginname == NULL || user_ses.remoteip == NULL)
   {
      syslog(LOG_MAIL | LOG_ERR,"Corrupt session file %s.",user_ses.sessionid);
      destroy_user_ses();
      return(-1);
   }

   return(0);
}

///////////////////////////////////////////////////////////////////////
//This will init a login session, does name/passwd check. If valid,
//   creates session file 
//
//   user_ses.loginname is required
//
//   returns 0 on success
//   returns 1 on bad username/password
//   returns -1 on fatal error

int create_session(char* password)
{
   int fdes[2];
   FILE* out_pipe=NULL;
   pid_t pid=0;

   int status=-1;

   if(user_ses.loginname == NULL)
      return(-1);

   if(pipe(fdes) != 0)
   {
      syslog(LOG_MAIL | LOG_ERR,"Pipe failed (%s)",strerror(errno));
      return(-1);
   }

   pid=fork();

   if(pid == 0)			//child
   {
      destroy_passwd(password);

      close(fdes[1]);
      dup2(fdes[0],0);
      close(fdes[0]);
      
      execl(SBINDIR "/login_validate",SBINDIR "/login_validate",NULL);
      syslog(LOG_MAIL | LOG_ERR,"exec login_validate failed (%s)",
				strerror(errno));
      exit(-1);
   }
   else if(pid > 0)		//parent
   {
      close(fdes[0]);
      out_pipe=fdopen(fdes[1],"w");

      if(out_pipe == NULL)
      {
         kill(pid,SIGKILL);
         syslog(LOG_MAIL | LOG_ERR,"fdopen(pipe) fail (%s)",strerror(errno));
         return(-1);
      }

      fprintf(out_pipe,"%s\n",user_ses.loginname);
      fprintf(out_pipe,"%s\n",password);
      destroy_passwd(password);
      fclose(out_pipe);

      waitpid(pid,&status,WUNTRACED);
   }
   else				//failed
   {
      destroy_passwd(password);
      syslog(LOG_MAIL | LOG_ERR,"Fork failed during auth (%s)",strerror(errno));
      return(-1);
   }

   if(status != 0)
   {
      syslog(LOG_MAIL | LOG_INFO,"User login %s from %s failed",
		user_ses.loginname,user_env.remote_addr);
      return(1);
   }

   if( create_session_file() != 0)
   {
      syslog(LOG_MAIL | LOG_ERR,"Could not create session file");
      return(-1);
   }

   return(0);
}

///////////////////////////////////////////////////////////////////////
//This creates a session file.
//   user_ses.loginname && user_env.remote_addr is required.
//   user_ses.name && user_ses.sessionid && user_ses.remoteip is filled in
//
//   returns 0 on success
//   returns -1 on fatal error

int create_session_file(void)
{
   mode_t old_mask=0;
   char* ses_filename;
   FILE* ses_file_handle;
   int err=0;

   struct passwd* data_pwd;

   if(user_ses.loginname == NULL || user_env.remote_addr == NULL)
      return(-1);

//Fill in user_ses.remoteip

   if( (user_ses.remoteip=strdup(user_env.remote_addr)) == NULL)
   {
      syslog(LOG_MAIL | LOG_ERR,"Out of memory (%s)",strerror(errno));
      return(-1);
   }

//Fill in user_ses.name

   data_pwd=getpwnam(user_ses.loginname);

   if(data_pwd == NULL)
   {
      syslog(LOG_MAIL | LOG_INFO,"User %s exists in /etc/shadow "
			"but not /etc/passwd",user_ses.loginname);
      return(-1);
   }

   if(data_pwd->pw_uid == 0)
   {
      syslog(LOG_MAIL | LOG_INFO,"%s attempted to check root mail",
			user_ses.remoteip);
      return(-1);
   }

   if(data_pwd->pw_gecos != NULL && *(data_pwd->pw_gecos) != '\0')
      if( (user_ses.name=strdup(data_pwd->pw_gecos)) == NULL)
      {
         syslog(LOG_MAIL | LOG_ERR,"Out of memory (%s)",strerror(errno));
         return(-1);
      }

//Fill in user_ses.sessionid

   srand(time(NULL));
   asprintf(&(user_ses.sessionid),"%d",rand());

   if(user_ses.sessionid == NULL)
   {
      syslog(LOG_MAIL | LOG_ERR,"Out of memory (%s)",strerror(errno));
      return(-1);
   }

   if( (ses_filename=create_ses_filename()) == NULL)
      return(-1);

   setuid(euid);
   err=file_exists(ses_filename);

   if(err == 1)
   {
      setuid(ruid);
      syslog(LOG_MAIL | LOG_ERR,"Duplicate sessionid");
      return(-1);
   }
   if(err != 0)
   {
      setuid(ruid);
      return(-1);
   }

   old_mask=umask(0177);

   ses_file_handle=fopen(ses_filename,"w");     
   setuid(ruid);

   umask(old_mask);
   free(ses_filename);

   if(ses_file_handle == NULL)
   {
      syslog(LOG_MAIL | LOG_ERR,
		"Could not create session file (%s)",strerror(errno));
      return(-1);
   } 

   fprintf(ses_file_handle,"loginname=%s\n",user_ses.loginname);
   if(user_ses.name != NULL)
      fprintf(ses_file_handle,"name=%s\n",user_ses.name);
   fprintf(ses_file_handle,"remoteip=%s\n",user_ses.remoteip);

   fclose(ses_file_handle);

   return(0);
}

///////////////////////////////////////////////////////////////////////
//This will delete the users session file from the local state directory 
//   user_ses.sessionid required

int logout_session(void)
{
   int err=0;
   char* ses_filename;

   if(user_ses.sessionid == NULL)
      return(-1);

   ses_filename=create_ses_filename();

   if(ses_filename == NULL)
      return(-1);

   setuid(euid);
   err=file_exists(ses_filename);

   if(err == 0)
   {
      setuid(ruid);
      syslog(LOG_MAIL | LOG_ERR,"Cannot delete file that does not exist");
      free(ses_filename);
      return(-1);
   }
   if(err == -1)
   {
      setuid(ruid);
      return(-1);
   }

   err=unlink(ses_filename);
   setuid(ruid);

   if(err != 0)
   {
      syslog(LOG_MAIL | LOG_ERR,"Could not delete session file %s (%s)",
			ses_filename,strerror(errno));
      free(ses_filename);
      return(-1);
   }

   free(ses_filename);

   return(0); 
}

///////////////////////////////////////////////////////////////////////
//This function will clean up old out-dated sessions from the local
//   state directory

int cleanup_sessions(void)
{
   DIR* dirptr=NULL;
   struct dirent* direntptr=NULL;
   struct stat file_stat;
   int err=0;
   char* old_cwd=NULL;

   if( (old_cwd=getcwd(NULL,0)) == NULL)
   {
      syslog(LOG_MAIL | LOG_ERR,"error during getcwd");
      return(-1);
   }

   setuid(euid);

   if( chdir(LOCKDIR) != 0)
   {
      setuid(ruid);
      syslog(LOG_MAIL | LOG_ERR,"could not chdir(LOCKDIR) (%s)",
				strerror(errno));
      return(-1);
   }

   dirptr=opendir(LOCKDIR);

   if(dirptr == NULL)
   {
      syslog(LOG_MAIL | LOG_ERR,
	"could not open LOCKDIR for reading (%s)",strerror(errno));

      return(-1);
   }

   while( (direntptr=readdir(dirptr)) != NULL )
   {
      if( strncmp(direntptr->d_name,SESPREFIX,sizeof(SESPREFIX)-1) == 0) 
      {
         if( stat(direntptr->d_name,&file_stat) == 0 ) 
         {
            if( difftime(time(NULL),file_stat.st_atime) > TIMEOUTVALUE )
            {
              err=unlink(direntptr->d_name);

              if(err != 0)
                 syslog(LOG_MAIL | LOG_ERR,"Could not delete file %s (%s)",
			direntptr->d_name,strerror(errno));
              else
                 syslog(LOG_MAIL | LOG_INFO,
			"Session file %s deleted. Timed out.",
			direntptr->d_name);
            }
         }
         else
            syslog(LOG_MAIL | LOG_ERR,"stat failed on %s (%s)",
			direntptr->d_name,
			strerror(errno));
      }
   }

   closedir(dirptr);

   setuid(ruid);

   return(0);
}

///////////////////////////////////////////////////////////////////////
//This will overwrite the password stored in memory. This way if we
//   get and error and core dump, the plaintext user password is not
//   stored on the system.

int destroy_passwd(char* passwd)
{
   if(passwd == NULL)
      return(-1);

   while(*passwd != '\0')
   {
      *passwd='\0';
      passwd++;
   }

   return(0);
}

///////////////////////////////////////////////////////////////////////

int init_user_ses(void)
{
   user_ses.sessionid=NULL;
   user_ses.loginname=NULL;
   user_ses.name=NULL;
   user_ses.remoteip=NULL;

   return(0);
}

///////////////////////////////////////////////////////////////////////
//This will free up memory used by the current session

int destroy_user_ses(void)
{
   if(user_ses.sessionid != NULL)
   {
      free(user_ses.sessionid);
      user_ses.sessionid=NULL;
   }
   if(user_ses.loginname != NULL)
   {
      free(user_ses.loginname);
      user_ses.loginname=NULL;
   }
   if(user_ses.name != NULL)
   {
      free(user_ses.name);
      user_ses.name=NULL;
   }
   if(user_ses.remoteip != NULL)
   {
      free(user_ses.remoteip);
      user_ses.remoteip=NULL;
   }

   return(0);
}

///////////////////////////////////////////////////////////////////////
//This function create a dynanically allocated string that represents
//   the session filename bases on LOCALSTATEDIR/ses_sessionid
//   returns pointer to filename if successful, NULL if memory error

char* create_ses_filename(void)
{
   char* ses_filename;
   size_t size_of_filename;

   if(user_ses.sessionid == NULL)
      return(NULL);

   size_of_filename=sizeof(LOCKDIR "/" SESPREFIX)+
		strlen(user_ses.sessionid)+1;      

   ses_filename=(char*)malloc(size_of_filename);

   if(ses_filename == NULL)
   {
      syslog(LOG_MAIL | LOG_ERR,"Out of memory (%s)",strerror(errno));
      return(NULL);
   }

   strcpy(ses_filename,LOCKDIR "/" SESPREFIX);
   strcat(ses_filename,user_ses.sessionid);

   return(ses_filename);
}

///////////////////////////////////////////////////////////////////////

int cleanup_tmp_files(void)
{
   DIR* tmp_dir=NULL;
   DIR* ses_dir=NULL;
   struct dirent* dir_entry=NULL;
   char* tmp_name=NULL;
   char* ses_name=NULL;
   char* tmp_ptr=NULL;

   int status=0;

   setuid(euid);
   tmp_dir=opendir(TMPDIR);
   setuid(ruid);

   if(tmp_dir == NULL)
   {
      syslog(LOG_MAIL | LOG_ERR,"could not open dir " TMPDIR " (%s)",
				strerror(errno));
      return(-1);
   }


   setuid(euid);
   ses_dir=opendir(LOCKDIR);
   setuid(ruid);

   if(ses_dir == NULL)
   {
      closedir(tmp_dir);
      syslog(LOG_MAIL | LOG_ERR,"could not open dir " LOCKDIR " (%s)",
				strerror(errno));
      return(-1);
   }


   do
   {
      status=0;

      do
      {
         dir_entry=readdir(tmp_dir);

         if(dir_entry == NULL)
            status=-1;
         else if(strncmp(dir_entry->d_name,TMPPREFIX,
			sizeof(TMPPREFIX)-1) == 0)
            status=1;
       
      }
      while(status == 0);

      if(status==1)
      {
         tmp_name=(char*) malloc(sizeof(TMPDIR "/") + 
			strlen(dir_entry->d_name));

         ses_name=strdup( (dir_entry->d_name) + sizeof(TMPPREFIX) -1);

         if(tmp_name == NULL || ses_name == NULL)
         {
            if(tmp_name != NULL)
               free(tmp_name);
            if(ses_name != NULL)
               free(ses_name);

            closedir(tmp_dir);
            closedir(ses_dir);

            syslog(LOG_MAIL | LOG_ERR,"Out of memory");
            return(-1);
         }

         strcpy(tmp_name,TMPDIR "/");
         strcat(tmp_name,dir_entry->d_name);

         tmp_ptr=strrchr(ses_name,'.');

         if(tmp_ptr != NULL)
            *tmp_ptr='\0';

         tmp_ptr=ses_name+sizeof(TMPPREFIX)-1;
     
         status=0;
         rewinddir(ses_dir);

         do
         {
            dir_entry=readdir(ses_dir);

            if(dir_entry == NULL)
               status=2;

            else if(dir_entry->d_name != NULL && 
		strncmp(dir_entry->d_name,SESPREFIX,sizeof(SESPREFIX)-1) == 0)
            {
               if(strcmp( (dir_entry->d_name)+sizeof(SESPREFIX)-1,
				ses_name) == 0)
                  status=1;
            }
         }
         while(status==0);

         if(status != 1)
         {
            syslog(LOG_MAIL | LOG_INFO,"deleting stale tmp file %s",
			tmp_name);

            setuid(euid);

            if(unlink(tmp_name) != 0)
            {
               syslog(LOG_MAIL | LOG_ERR,"Could not delete tmp file (%s)",
				strerror(errno));
               setuid(ruid);
               status=-1;
            }

            setuid(ruid);
         }
         free(tmp_name);
         tmp_name=NULL;
         free(ses_name);
         ses_name=NULL;
      }
   }
   while(status >= 0);



   closedir(tmp_dir);
   closedir(ses_dir);

   return(0);
}

///////////////////////////////////////////////////////////////////////


