#ifndef _RMSPC_C
#define _RMSPC_C
//esta função remove todos os espaços em branco de uma frase
#include <StrSet/include/strset.h>

























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