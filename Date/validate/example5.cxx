#include <iostream>
//calcula distancia entre dias
//https://stackoverflow.com/a/6057429/5429980
struct data
{
int dia;
int mes;
int ano;
};

unsigned long dist_dias(data inicio, data fim);
 
int main(void)
{

data dia1, dia2;
printf("\n\tDigite primeira data\n\tDia: ");
scanf("%d", &dia1.dia);

printf("\n\tMes: "); 
scanf("%d",&dia1.mes);

printf("\n\tAno: ");
scanf("%d", &dia1.ano);

printf("\n\tDigite segunda data\n\tDia: ");
scanf("%d", &dia2.dia);

printf("\n\tMes: "); 
scanf("%d",&dia2.mes);

printf("\n\tAno: ");
scanf("%d", &dia2.ano); 

printf("\n\tA distancia em dias: %lu\n\n", dist_dias(dia1, dia2));
}

int daystotal (int d, int m, int y)
{
 static char daytab[2][12] =
  {
   {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
   {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
  };
        
 int daystotal = d;
 for(int year = 1 ; year <= y ; year++)
  {
   int max_month = ( year < y ? 12 : m-1 );
   int leap = (year%4 == 0);
   if(year%100 == 0 && year%400 != 0)leap = 0;
   
   for(int month = 1 ; month <= max_month ; month++)
    {
     daystotal += daytab[leap][month-1];
    }
  }
 return daystotal;
}

unsigned long dist_dias(data inicio, data fim) 
{
 int dbissexto;
 register int i;
 unsigned long idias;
 unsigned long fdias;
 unsigned long def_anos = 0;

 int dias_mes[2][12] = {
                        {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
                        {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
                       };
       
 idias = inicio.dia;
 dbissexto = __isleap(inicio.ano);
 
 for (i = inicio.mes - 1; i > 0; --i)
  idias += dias_mes[dbissexto][i-1];
   fdias = fim.dia;
    dbissexto = __isleap(fim.ano);

 for (i = fim.mes - 1; i > 0; --i)
  fdias += dias_mes[dbissexto][i-1];    

 while (inicio.ano < fim.ano)
  def_anos += 365 + __isleap(inicio.ano++);
 
   return (def_anos - idias + fdias);
}