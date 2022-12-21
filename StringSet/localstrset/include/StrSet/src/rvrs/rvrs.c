#ifndef _RVRS_C
#define _RVRS_C
/* função  que mostra a palavra ou frase escrita ao contrario */
#include "../../include/strset.h"


char* rvrs(char frase[]) 
{
  int i=0, j=0;
  char aux;
  char copyed;
  
  while(frase[i] != '\0')
   i++;
   i--;
  
  while(j<i) 
  {
   aux=frase[j];
   frase[j]=frase[i];
   frase[i]=aux;
   j++;
   i--;
  }
 return frase;
} 

#endif