#include <stdio.h>
//este programa calcula a diferença em minutos de determinada hora inicial e final.
int mintotal(int hora, int minuto)
{
 int r = (hora*60) + minuto;
 return r;
}

int main()
{
 int h1, m1, h2, m2; //variaveis declaradas

 printf("\n\tDigite a hora:minuto inicial: ");
 scanf("%d:%d", &h1, &m1); //armazendando valores da hora inicial

 int r1 = mintotal(h1, m1);
 
 printf("\n\tDigite a hora:minuto final: ");
 scanf("%d:%d", &h2, &m2);

 int r2 = mintotal(h2, m2);

 int total = r2-r1;

 if(total < 0)
  {
   total+=1440; //calculo
   
   printf("\n\t%d minutos de diferenca!\n\n", total); //impressão do resultado
  }
  else
  printf("\n\t%d minutos de diferenca!\n\n", total); //impressão do resultado
 return 0;
}