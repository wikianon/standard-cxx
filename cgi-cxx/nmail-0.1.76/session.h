/*
	session.h
*/

#ifndef _SESSION_H
#define _SESSION_H


#include <unistd.h>
#include <sys/types.h>

#define TIMEOUTVALUE	(60*30)

typedef struct websession {
   char* sessionid;
   char* loginname;
   char* name;
   char* remoteip;
} session_t;


extern session_t user_ses;

int init_user_ses(void);
int destroy_user_ses(void);

int check_session(void);
int create_session(char*);
int logout_session(void);
int cleanup_sessions(void);
int cleanup_tmp_files(void);


#endif
