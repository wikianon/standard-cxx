/*
	parse_form.h
*/

#ifndef _PARSE_FORM_H
#define _PARSE_FORM_H


typedef struct cgi_value_pair
{
   char* name;
   char* value;

   char* filename;
   FILE* tmp_file;
   char* tmp_filename;
   char* cont_type;
} cgi_t;

int get_cgi_data(cgi_t**);
int cgi_rewind(void);
int init_cgi_env(void);
int destroy_cgi_env(void);
char* create_tmp_filename(void);

#endif
