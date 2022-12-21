#ifndef _IOSTR_C
#define _IOSTR_C
//esta função recebe input e remove todos os espaços em branco em excesso de uma frase
#include <StrSet/include/strset.h>
//#include <StrSet/src/rdspc/rdspc.c>






























void iostr(char* string)
{
 fgets(string,1000,stdin);
   char *lstring = rdspc(string);
   printf("\t%s\n",lstring);
 free(lstring);  
}

#endif