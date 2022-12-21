#include <stdio.h>
//https://www.scriptbrasil.com.br/script/c/daycounter.rar

//conta dias deste janeiro
int Td(int d,int m,int a)
{
 int soma=0; 
 int mes[12];
 
 mes[1]=mes[3]=mes[5]=mes[7]=mes[8]=mes[10]=mes[12]=31;
 
 mes[4]=mes[6]=mes[9]=mes[11]=30;
 
 mes[2] = (a%4 == 0) ? 29 : 28;
 
 for(int i=1;i<m;i++)soma+=mes[i];
 soma+=d-1;
 
 return soma;
}


int main()
{
 int dia,mes,ano;
 
 printf("\n\tDigite o dia (01-31): ");
 scanf("%d",&dia);
 
 printf("\n\tDigite o mes (01-12): ");
 scanf("%d",&mes);
 
 printf("\n\tDigite o ano: ");
 scanf("%d",&ano);
 
 printf("\n\tForam decorridos %d dias desde 01/01/%d\n\n",Td(dia,mes,ano),ano);
 
 return 0;
} 
