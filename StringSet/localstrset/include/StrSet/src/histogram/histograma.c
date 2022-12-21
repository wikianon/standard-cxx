#ifndef _HISTOGRAMA_C
#define _HISTOGRAMA_C
/* função que faz contagem de cada letra e consoantes em uma frase */
#include "../../include/strset.h"

void histogram(char *frase) 
{
 int num=0, num2=0, az=0;
 char alfabeto[]="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
 int contagens[256]; //array para as contagens das letras
 
 for(num = 0; num < 256; ++num)
 contagens[num]=0; //iniciar o array todo a zeros

 for(num = 0; frase[num] != 0; num++) 
  {
   for (num2 = 0; alfabeto[num2] != '\0'; ++num2)
    {
     char letra = frase[num];
      if (letra == alfabeto[num2])
      {
       contagens[(int)letra]++; //contabilizar para a letra corrente
       az++;
       break;
      }
    }
  }
 
 for(num = 0; num < 256; ++num)
  if (contagens[num] > 0)  //mostrar apenas os que tem pelo menos 1 letra
   printf("\n\n\tTemos %d letra(s) %c", contagens[num], (char)num);
  printf("\n\n");
} 

#endif