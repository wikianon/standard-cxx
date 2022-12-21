#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
 /*
  * Programa: Cronômetro
  * Data de criação: 21/09/2015
  * Autor: Eric Cancellgliere (http://programacaoemfoco.com.br)
  * Versão: 1.0
  * Última modificação em [21/09/2015] feita por [Eric Cancellgliere]
  * https://pastebin.com/A6cnTv8A
  */
#if defined(_WIN32) || defined(_WIN64) || defined(__WINDOWS__)

#include <windows.h> //para usar o Sleep()

#endif
 
int main(void)
{
 //comando de regionalização
 setlocale(LC_ALL, "Portuguese");
 #if defined(_WIN32) || defined(_WIN64) || defined(__WINDOWS__)
 
 SetConsoleTitle("Cronometro"); //título do programa
 
 #endif
 
 int sec=0; //segundos
 int min=0; //minutos
 int hr=0; //horas
               
 do{
    #if defined(_WIN32) || defined(_WIN64) || defined(__WINDOWS__)
     system("cls");//limpa a tela
    #else
     system("clear");//limpa a tela
    #endif
    
    printf("Cronômetro\n\n\n\t\t\t\t%dh:%dm:%ds\n\n", hr,min,sec);
               
    if(sec == 60)
     {
      sec=0;
      min++;
     }
         
    if(min == 60)
     {
      min=0;
      hr++;
     }
               
    if(hr == 24)
     {
      hr=0;
     }
     
    #if defined(_WIN32) || defined(_WIN64) || defined(__WINDOWS__)
     Sleep(1000); //tem a função de pausar a execução por um determinado tempo
    #endif
     sec++;
    
   }while(1); //condição para o loop infinito
   
    #if defined(_WIN32) || defined(_WIN64) || defined(__WINDOWS__)
     system("pause");
    #endif
  return 0;
}