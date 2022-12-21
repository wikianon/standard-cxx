#ifndef _STRJST_C
#define _STRJST_C
//Esta função remove todos os espacos em escesso de uma frase
#include <StrSet/include/strset.h>













char *strjst(char* string)
{
 char blank[1000];
 int c = 0, d = 0;
 
   while (string[c] != '\0') {
      if (string[c] == ' ') 
      {
         int temp = c + 1;
         if (string[temp] != '\0') {
            while (string[temp] == ' ' && string[temp] != '\0') 
            {
             c=temp; 
             temp++;
            }
         }
      }
      
      blank[d] = string[c];
      c++;
      d++;
     }
   blank[d] = '\0';
  strcpy( string, blank );
}
 
#endif