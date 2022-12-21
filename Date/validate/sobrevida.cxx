#include <cmath>
#include <iostream>

/*
  https://gist.github.com/chaitanyav/5754292
  
  https://github.com/coreutils/coreutils/blob/master/src/uptime.c
  
   int dias(daysDiff(d1,d2)); // diferença entre datas em dias contando anos bisextos

   int idade(dias/365); //idade em anos

   int qtd_dias((dias%365)%30); // qtd dias adicionais a partir do dia de nascimento

   int qtd_meses(((dias%365)/30)); // qtd meses adicionais a partir do mes de nascimento

   int meses_totais(dias/30);

   //int meses_totais((dias/30)-qtd_dias); // total de meses decorridos nos dias - dias a mais 

   int qtd_semanas(dias/7); // qtd semanas total dos dias

   int horas(dias*24); // qtd horas total dos dias

   int minutos(horas*60); // qtd minutos total dos dias

   int segundos(minutos*60); // qtd segundos total dos dias
   
   http://vini-brown.zip.net/
   https://pt.stackoverflow.com/questions/264755/calculo-de-sobrevida-em-c
   https://www.calendario-365.com.br/calcular/29-03-1987_29-03-2018.html
   
   .------------------------------------+---------------+-----------------------+------------------------.
   |  de 29/03/1987 a 29/03/2018        | Arredondado   | Arredondado para cima | Arredondado para baixo |
   |------------------------------------+---------------+-----------------------+------------------------|
   | Número de dias    11.324 dias      | 11.324 dias   | 11.324 dias           | 11.324 dias            |
   |------------------------------------+---------------+-----------------------+------------------------|
   | Número de semanas 1.617,71 semanas | 1.618 semanas | 1.618 semanas         | 1.617 semanas          |
   |------------------------------------+---------------+-----------------------+------------------------|
   | Número de meses   372,04 meses     | 372 meses     | 373 meses             | 372 meses              |
   |------------------------------------+---------------+-----------------------+------------------------|
   | Número de anos    31 anos          | 31 anos       | 32 anos               | 31 anos                |
   '------------------------------------+---------------+-----------------------+------------------------'
   
   Número segundos: 978.393.600 segundos
   Número minutos: 16.306.560 minutos
   Número horas: 271.776 horas
*/

struct Sobrevida
{
 int d, m, y;
};

int daysTill(int month)
{
 int days=0;
 
 switch(month)
  {
   case 1: days=0; break;
   
   case 2: days=31; break;
   
   case 3: days=59; break;
   
   case 4: days=90; break;
   
   case 5: days=120; break;
   
   case 6: days=151; break;
   
   case 7: days=181; break;
   
   case 8: days=212; break;
   
   case 9: days=243; break;
   
   case 10: days=273; break;
   
   case 11: days=304; break;
   
   case 12: days=334; break;
  }
 return days;
}

int daysDiff(Sobrevida d1, Sobrevida d2)
{
 int dd1=0, dd2=0, y, yref;
 
 yref=(d1.y<d2.y)?d1.y:d2.y;
  for(y=yref;y<d1.y;y++)
  if(__isleap(y))
  dd1++;
  
  if(__isleap(d1.y) && d1.m>2) dd1++;
  dd1+=daysTill(d1.m)+d1.d+(d1.y-yref)*365;
  
  for(y=yref;y<d2.y;y++)
  if(__isleap(y))
  dd2++;
  if(__isleap(y) && d2.m>2) dd2++;
  dd2+=daysTill(d2.m)+d2.d+(d2.y-yref)*365;
  return abs(dd2-dd1);
}

int main()
{

 Sobrevida d1, d2;

   printf(
          "\n\tDigite duas datas no formato DD/MM/YYYY"
          "\n\n\tDigite data1: "
         );

   scanf("%d/%d/%d",&d1.d,&d1.m,&d1.y);

   printf(
          "\n\tDigite data2: "
         );

   scanf("%d/%d/%d",&d2.d,&d2.m,&d2.y);


   int dias(daysDiff(d1,d2)); // diferença entre datas em dias contando anos bisextos

   int idade(dias/365); //idade em anos

   int qtd_dias((dias%365)%30); // qtd dias adicionais a partir do dia de nascimento

   int qtd_meses(((dias%365)/30)); // qtd meses adicionais a partir do mes de nascimento

   int meses_totais(dias/30);

   //int meses_totais((dias/30)-qtd_dias); // total de meses decorridos nos dias - dias a mais 

   int qtd_semanas(dias/7); // qtd semanas total dos dias

   int horas(dias*24); // qtd horas total dos dias

   int minutos(horas*60); // qtd minutos total dos dias

   int segundos(minutos*60); // qtd segundos total dos dias

   /*
   int segs(dias*86400);
   int horas(segs/3600);
   int minutos((segs-(horas*3600))/60);
   int segundos((segs - (horas * 3600)) - (minutos * 60));
    */

    printf(
           "\n\t%d ano(s) %d mes(es) e %d dia(s) de vida"
           "\n\t%d horas"
           "\n\t%d minutos"
           "\n\t%d segundos"
           "\n\t%d mes(es)"
           "\n\t%d dia(s)"
           "\n\t%d semana(s)\n\n",
           idade,
           qtd_meses,
           qtd_dias,
           horas,
           minutos,
           segundos,
           meses_totais,
           dias,
           qtd_semanas
          );
  return 0;
} 
