#ifndef _SPACE_C
#define _SPACE_C

#include <StrSet/include/strset.h>































int space(char* string) 
{
 if(strtok(string," \t")==NULL) 
    return 1;
  else 
    return 0;
}

#endif