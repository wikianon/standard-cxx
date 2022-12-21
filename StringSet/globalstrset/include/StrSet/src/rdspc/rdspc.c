#ifndef _RDSPC_C
#define _RDSPC_C
/* função recursiva para ajustar espaços na string 
#include "../../include/strset.h"
*/

#include <StrSet/include/strset.h>


char *rdspc(char* string) 
{
  int length;
   int c=0;
   int d=0;
  char *start;
 
   length = strlen(string);
    start = (char*)malloc(length+1);
    if (start == NULL)
   exit(EXIT_FAILURE);
 
   while (*(string+c) != '\0') 
   {
     if (*(string+c) == ' ') 
      {
       int temp = c + 1;
        if (*(string+temp) != '\0') 
         {
          while (*(string+temp) == ' ' && *(string+temp) != '\0') 
          {
          c=temp; 
          temp++;
          }
         }
        }
      *(start+d) = *(string+c);
     c++;
    d++;
   }
  *(start+d)= '\0';
 return start;
}

#endif