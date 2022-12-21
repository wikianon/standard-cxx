/*
	preferences.h
*/

#ifndef _PREFERENCES_H
#define _PREFERENCES_H




#define _GNU_SOURCE

#include "inbox_page.h"
#include "session.h"
#include "addr.h"

typedef struct preferences {
   addr_t* addr;		//this is an array
   addr_t reply_to;		//this is an actual pointer
   m_sort sort_by;
   char* signature;
   time_t cookie_expire;
   char* cookie_crypt;
} pref_t;   

extern pref_t user_pref;

int init_user_pref(void);
int get_user_pref(void);
int destroy_user_pref(void);

FILE* open_user_pref_file(char*, int*);

int add_pref_email(FILE*, char*, char*);
int delete_pref_email(FILE*, char*, char*);
int modify_pref_email(FILE*, char*, char*, char*, char*);

int save_pref_entry(char*, char*);

#endif
