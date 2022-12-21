/*
	addr.h
*/

#ifndef _ADDR_H
#define _ADDR_H

typedef struct address_struct {
   char* name;
   char* email;
   int status;
} addr_t;


addr_t* parse_addr(char**);
char* fill_addr(addr_t*, char*);
int inc_addr(addr_t**);
int destroy_addr_arr(addr_t**);
int destroy_addr(addr_t*);

int valid_email(char*);

#endif

