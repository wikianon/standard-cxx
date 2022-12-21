#ifndef _RMSPC_C
#define _RMSPC_C
/* remove todos os espaços das strings */
#include "../../include/strset.h"
 
char rmspc(char* string)
{
 int j = 0;
  for(int i = 0; string[i]; i++) 
   {
    if(string[i] != ' ') 
    {
    string[j] = string[i];
    j++;
   }
  }
 string[j] = '\0';
}

#endif