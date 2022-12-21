#ifndef _SPACE_C
#define _SPACE_C
/* verifica se existe espaço em branco */
#include "../../include/strset.h"

int space(char* string) 
{
 if(strtok(string," \t")==NULL) 
    return 1;
  else 
    return 0;
}

#endif