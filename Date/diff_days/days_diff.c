#include <time.h>
#include <stdio.h>

/* Estrutura para representar uma DataHora */
typedef struct _MinhaDataHora
{
 int hora;
 int minuto;
 int segundo;
 int dia;
 int mes;
 int ano;
}MinhaDataHora;

/* Função auxiliar para definir os valores da estrutura DataHora */
void defineDataHora(MinhaDataHora *pd, int hora, int minuto, int segundo, int dia, int mes, int ano)
{
 pd->hora = hora;
 pd->minuto = minuto;
 pd->segundo = segundo;
 pd->dia = dia;
 pd->mes = mes;
 pd->ano = ano;
}

/* Função auxiliar para imprimir os valores da estrutura DataHora formatados */
void imprimeDataHora(MinhaDataHora d)
{
 printf("%02d:%02d:%02d %02d/%02d/%04d", d.hora, d.minuto, d.segundo, d.dia, d.mes, d.ano);
}

/* Função para verificação se um ano é bissexto */
int ehBissexto(int ano)
{
 /*
   Um ano só é bissexto se ele for divisível por 4 e NÃO FOR divisível por 100
   (a não ser que ele seja divisível por 100 E TAMBÉM divisível por 400).
  */
 return ((ano%4 == 0 || ano%400 == 0) && (ano % 100 != 0));
}

/* Função para contar quantos anos bissextos tem entre o ano 1/1/1 e o ano dado */
int contaBissextos(int ano)
{
 return (ano / 4) - (ano / 100) + (ano / 400);
}

/* Função para converter a DataHora dada para segundos */
unsigned long long dataParaSegundos(MinhaDataHora d)
{
 unsigned long long total = 0LL;
 
 /*
   Primeiro, calcula o tempo decorrido em segundos até a data
   sem contar os anos bissextos, considerando:
     
   31536000 = número de segundos em um ano não bissexto (365 dias)
   86400 = número de segundos em um dia (24 horas)
   3600 = número de segundos em 1 hora (60 minutos)
   60 = número de segundos em 1 minuto (60 segundos)
  */
 
 total += (d.ano - 1) * 31536000LL;
 
 int meses[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
 for(int mes = 1; mes < d.mes; mes++)
 total += meses[mes-1] * 86400LL;
 
 total += (d.dia - 1) * 86400LL;
 
 total += d.hora * 3600LL;
 
 total += d.minuto * 60LL;
 
 total += d.segundo;
 
 /*
   Então, adiciona segundos para cada dia adicional dos anos bissextos
  */
 
 /* Número de dias adicionais, para os anos bissextos anteriores ao ano atual */
 int diasExtras = contaBissextos(d.ano - 1);
 total += diasExtras * 86400LL;
 
 /* Se o mês da data já passou de fereveiro e o ano atual é bissexto, adiciona mais um dia */
 if(ehBissexto(d.ano) && (d.mes - 1) >= 2)
 total += 86400LL;
 
 return total;
}

/* Função para o cálculo da diferença em segundos entre duas datas */
unsigned long long calculaDiferencaEmSegundos(MinhaDataHora d1, MinhaDataHora d2)
{
 unsigned long long segundosd1 = dataParaSegundos(d1);
 unsigned long long segundosd2 = dataParaSegundos(d2);

 if(segundosd1 > segundosd2)
  return segundosd1 - segundosd2;
 else
  return segundosd2 - segundosd1;
}

/* Função para o cálculo da diferença em dias entre duas datas */
unsigned long long calculaDiferencaEmDias(MinhaDataHora d1, MinhaDataHora d2)
{
 unsigned long long segundos = calculaDiferencaEmSegundos(d1, d2);
 return segundos / 86400LL;
}

/* Função principal do programa */
int main(int argc, char **argv)
{
 MinhaDataHora d1, d2; 
 time_t datime = time(NULL);
 struct tm* dt = localtime(&datime);
 
 int hora = (dt->tm_hour);
 int min = (dt->tm_min);
 int sec = (dt->tm_sec);
 int dia = (dt->tm_mday);
 int mes = (dt->tm_mon+1);
 int ano = (dt->tm_year+1900);
 
 defineDataHora(&d1, 23, 05, 10, 29, 03, 1987);
 
 defineDataHora(&d2, hora, min, sec, dia, mes, ano);
 
 printf("d1: ");
 imprimeDataHora(d1);
 printf(" (em segundos: %llu)\n", dataParaSegundos(d1));
 printf("d2: ");
 imprimeDataHora(d2);
 printf(" (em segundos: %llu)\n\n", dataParaSegundos(d2));
 
 printf("Diferenca em segundos entre d1 e d2: %llu\n", calculaDiferencaEmSegundos(d1, d2));
 printf("Diferenca em dias entre d1 e d2: %llu\n", calculaDiferencaEmDias(d1, d2));
 
 return 0;
} 
/*
 Esse programa resulta na seguinte saída:
 
 d1: 00:00:00 12/03/2011 (em segundos: 63435484800)
 d2: 00:00:00 05/10/2014 (em segundos: 63548064000)
 
 Diferenca em segundos entre d1 e d2: 112579200
 Diferenca em dias entre d1 e d2: 1303
*/