#include <stdio.h>

void dias_do_mes(int);

int main()
{
 int  ano;
 
 printf("informe um numero que represente o ano: formato (aaaa): ");
 scanf("%d",&ano);
 
 dias_do_mes(ano);
 return 0;
}

void dias_do_mes(int ano)
{
 int cont=0, b6=0, aux1, aux2;
 
 int a(ano%19);
 int b(ano/100);
 int c(ano%100);
 int d(b/4);
 int e(b%4);
 int f((b+8)/25);
 int g((b-f+1)/3);
 int h((19*a+b-d-g+15)%30);
 int i(c/4);
 int j(c%4);
 int k((32+2*e+2*i-h-j)%7);
 int l((a+11*h+22*k)/451);
 int mes((h+k-7*l+114)/31);
 int dia((h+k-7*l+114)%31+1);
 
 if((ano%4 == 0 && ano%100 != 0) || (ano%400 == 0))
 b6=29; //ano bissexto
 else
 b6=28;
 
 if(mes == 3)
 {
  printf("\nferiados que ocorreram em %d:\n",ano);
  aux1 = (b6+31+dia)-47;
  
  if(aux1>31)
   printf("\n%d/%d/%d - cinzas\n%d/%d/%d - carnaval\n",aux1-30,mes-1,ano,aux1-31,mes-1,ano);
  else
   printf("\n%d/%d/%d - cinzas\n",aux1+1,mes-2,ano); 
   printf("\n%d/%d/%d - carnaval\n",aux1,mes-2,ano);
   printf("\n%d/%d/%d - Sexta-feira Santa\n",dia-2,mes,ano);
   printf("\n%d/%d/%d - Pascoa\n",dia,mes,ano);
   aux2 = (dia+60)-30;
  if(aux2<=30)
   printf("\n%d/%d/%d - corpus christi\n\n",aux2, mes+1,ano);
  else
   printf("\n%d/%d/%d - corpus christi\n\n",aux2-31, mes+2,ano);
 }
 else
 if(mes == 4)
 {
  printf("\nferiados que ocorreram em %d:\n\n",ano);
  aux1 = (b6+31+dia)-47;
  if(aux1 > b6)
   printf("\n%d/%d/%d - cinzas\n%d/%d/%d - carnaval\n"aux1-27,mes-1,ano,aux1-28,mes-1,ano);
  else
   printf("\n%d/%d/%d - cinzas\n",aux1+1,mes-2,ano); 
   printf("\n%d/%d/%d - carnaval\n",aux1,mes-2,ano);
   printf("\n%d/%d/%d - Sexta-feira Santa\n",dia-2,mes,ano);
   printf("\n%d/%d/%d - Pascoa\n",dia,mes,ano);
   aux2 = (dia+60)-31;
  if(aux2 <= 31)
   printf("\n%d/%d/%d - corpus christi\n\n",aux2, mes+1,ano);
  else
   printf("\n%d/%d/%d - corpus christi\n\n",aux2-30, mes+2,ano);
 }
}