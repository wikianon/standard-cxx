/*
	environment.h
*/
#ifndef _ENVIRONMENT_H
#define _ENVIRONMENT_H


enum cgi_method_t {		//cgi method
   cgi_not_specified,
   cgi_get,
   cgi_post,
   cgi_post_multipart
};

typedef struct environ {	//env_data contains information
   char* server_name;		//   about the connection request
   char* server_addr;
   char* remote_addr;
   char* doc_root;
   char* script_name;
   enum cgi_method_t r_method;
   unsigned int content_length;	//if method is post
   char* query_string;		//if method is get
   char* boundary;		//if method is post_multipart
} env_t;

int init_user_env(void);
int destroy_user_env(void);

extern env_t user_env;

#endif
