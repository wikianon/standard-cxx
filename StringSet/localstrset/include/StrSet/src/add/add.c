#ifndef _ADD_C
#define _ADD_C
/* aloca uma string a uma variavel */
#include "../../include/strset.h"

void add(const char* s)
{
 char** strings;
  int numstrings;
   if(strings) 
    {
     strings = (char**) realloc(strings, (numstrings + 1) * sizeof(char*));
     } 
     else 
     {
     strings = (char**) malloc(sizeof(char*));
    }
   strings[numstrings] = strdup(s);
  numstrings++;
} 

#endif


// Create and append to dinamyc length string - Alez 
char *str_add(char *dst, const char *src) 
{  
  if (dst == NULL) 
  { 
    dst = (char *) malloc(strlen(src) + 1);
    strcpy(dst, src); 
  }
  else
  { 
    dst = (char *) realloc(dst, strlen(dst) + strlen(src) + 1);
    strcat(dst, src); 
  };
  
  return dst;
}