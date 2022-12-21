#include <stdio.h>
#include <stdlib.h>

//http://algoritmosurgentes.com/algoritmo.php?a=39
//http://algoritmosurgentes.com/algoritmo.php?a=46
//http://www.contadordedias.com.br/
//https://www.calendario-365.com.br/numeros-dos-dias/2000.html




int main ()
{
 int falta_dias = 0;
 int dia, mes, ano;
 int dias_mes[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
 const char *strmes[12] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
 
 do{
    printf("\n\tEnter date format dd/mm/yyyy: ");
    scanf("%d/%d/%d", &dia, &mes, &ano);
    
    if(dia > 31 || dia < 1)printf("\n\tDay %d is Invalid.!!\n\tEnter day between 01 and 31\n",dia);
    
    if(mes > 12 || mes < 1)printf("\n\tMonth %d is Invalid.!!\n\tEnter day between 01 and 12\n",mes);
    
    dias_mes[1] = (((ano % 4 == 0) && (ano % 100 != 0)) || (ano % 400 == 0)) ? 29 : 28; // atualiza dia para 29 caso ano seja bisexto
    
    if(dia > dias_mes[mes-1])printf("\n\t%s of year %d does not have %d days!!!\n\n",strmes[mes-1],ano,dia);
    
   }while(dia < 1 || mes < 1 || dia > dias_mes[mes-1]);
   
   for(int i = mes; i<12; i++)
   falta_dias+=dias_mes[i];
   
   falta_dias+=(dias_mes[mes-1]-dia); // conta os dias restantes do mes indicado na entrada padrão
   
   int dias_totais = (((ano % 4 == 0) && (ano % 100 != 0)) || (ano % 400 == 0)) ? 366 : 365; // atualiza dia+1 caso  ano seja bisexto
   
   int decorridos = (dias_totais-falta_dias);
   
   printf(
          "\n\tDe %02d/%s/%d até %02d/%s/%d."
          "\n\tForam decorridos %d dias."
          "\n\tFaltam %d dias até %d/%s/%d.\n\n",
          (dias_mes[0]-30), strmes[0], ano,
          dia, strmes[mes-1], ano,
          decorridos,
          falta_dias, dias_mes[11], strmes[11], ano  
         );
 
 return 0;
}