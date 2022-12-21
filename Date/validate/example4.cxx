#include<stdio.h>
#include<stdlib.h>
 
int main()
{
// Idade em anos, meses e dias
int t, a=-1, m=-1, d=-1, ah=-1, mh=-1, dh=-1;

while(d<0 || m<0 || a<0)
{
 printf("\n\tInforme a data nascimento\n\tdia: ");
 scanf("%d", &d);
 
 printf("\n\tmes: ");
 scanf("%d", &m);

 printf("\n\tano: ");
 scanf("%d", &a);
}

while(dh<0 || mh<0 || ah<0)
{
 printf("\n\tInforme a data atual\n\tdia: ");
 scanf("%d", &dh);

 printf("\n\tmes: ");
 scanf("%d", &mh);

 printf("\n\tano: ");
 scanf("%d", &ah);
}

printf(
       "\n\tData de nascimento informada: %02d/%02d/%04d"
       "\n\n\tData atual informada: %02d/%02d/%04d\n",
       d, m, a, dh, mh, ah
      );

int i = (d + m*30 + a*365);
int h = (dh + mh*30 + ah*365);
int id = (h-i);
int ida = (ah-a);

//12-m+(m-mh)+(ida*12)
int mi = (12-m+mh+ida*12);

if(m>mh)
 printf("\n\tVoce tem %d anos, %d meses e %d dias\n\n",ida-1, id/30, id);
 else
 printf("\n\tVoce tem %d anos, %d meses e %d dias\n\n",ida, id/30, id); 
 
 return 0;
}