#ifndef _ADD_C
#define _ADD_C
/* 
 * aloca uma string a uma variavel 
 */
#include <StrSet/include/strset.h>





















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