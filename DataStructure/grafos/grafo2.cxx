#include <cstdio>
#include <cstring>
/*!
 *
 * @begin @section terms_of_use Terms of Use
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3.0 of the License, or (at your option) any later version.
 *
 * @end @section terms_of_use Terms of Use
 * 
 * @begin @section author Author
 * 
 * @file        grafo2
 * @version     0.1
 * @brief       Grafo que usa fila.
 * @aplication  Encontrar o caminho mais barato.
 * @author      Jean Zonta
 * @Copyright (C) 2017 Jean Zonta.
 * 
 * @end @section author Author
 *
 */
#define MAX 11
#define TAMANHO_NOME 30

#define SAO_PAULO 0
#define NOVA_YORK 1
#define LOS_ANGELES 2
#define LONDRES 3
#define FRANKFURT 4
#define TOKIO 5
#define SYDNEY 6
#define WASHINGTON 7
#define KEBECK 8
#define MOSCOU 9
#define SINGAPURA 10
 //globais
 int inicio=0; //inicio fila
 int tamanho; //fila para o modulo
 int fila[500]; //tamanho fila (bem maior para caber as combinacoes)
 
 int GrafoCidades[MAX][MAX]; //grafo
 int CorCidades[MAX];  //cidades acessadas
 char NomeCidades[MAX][TAMANHO_NOME]; //nome das cidades
 void inserirFILA(int);
 int removerFILA(void);
 void CaminhoNoGrafo(int);

int main()
{
 //inicia grafo com tudo -1
 for(int i=0; i< MAX; i++)
  for(int j=0; j< MAX; j++)
   GrafoCidades[i][j]= -1;
   
 //monta o grafo
 //GrafoCidades[SAO_PAULO][NOVA_YORK]= 350;
 GrafoCidades[SAO_PAULO][LONDRES]= 400;
 //GrafoCidades[NOVA_YORK][LOS_ANGELES]= 150;
 //GrafoCidades[NOVA_YORK][FRANKFURT]= 250;
 //GrafoCidades[NOVA_YORK][LONDRES]= 120;
 GrafoCidades[LONDRES][FRANKFURT]= 80;
 GrafoCidades[WASHINGTON][SINGAPURA]= 90;
 GrafoCidades[KEBECK][SYDNEY]= 80;
 //GrafoCidades[LONDRES][SYDNEY]= 500;
 GrafoCidades[SINGAPURA][MOSCOU]= 500;
 //GrafoCidades[LONDRES][SAO_PAULO]= 400;
 GrafoCidades[LONDRES][NOVA_YORK]= 120;
 GrafoCidades[FRANKFURT][TOKIO]= 500;
 GrafoCidades[LOS_ANGELES][TOKIO]= 400;
 //GrafoCidades[LOS_ANGELES][SYDNEY]= 450;
 GrafoCidades[TOKIO][SYDNEY]= 300;
 GrafoCidades[SYDNEY][TOKIO]= 500;
   
 //monta os nomes das cidades
 strcpy(NomeCidades[0],"Sao Paulo-GRU");
 strcpy(NomeCidades[1],"Nova York-JFK");
 strcpy(NomeCidades[2],"Los Angeles");
 strcpy(NomeCidades[3],"Londres-LHR");
 strcpy(NomeCidades[4],"Frankfurt");
 strcpy(NomeCidades[5],"Tokio");
 strcpy(NomeCidades[6],"Sydney");
    
 //imprimi todas as cidades na tela com o numero dela
 printf("Grafo gerado com as cidades :\n");
 for(int i=0; i < MAX; i++)
  printf("%i : %s\n", i, NomeCidades[i]);
  
 printf("\n\nTodas as cidades possiveis apartir de : %s\n",NomeCidades[KEBECK]);
   //calcula o caminho no grafo
 CaminhoNoGrafo(KEBECK);
 printf("\n");
}

void inserirFILA(int x)
{
 if( tamanho == MAX )printf("\nFila esta cheia\n");
 else
  {
   fila[ (inicio + tamanho) % MAX] = x;    //(inicio + tamanho) % MAX eh a posicao do proximo elemento
   tamanho++;         //incrementa o tamanho pq foi inserido mais um elemento
  }
}

int removerFILA(void)
{
 int aux;
 
 if(tamanho != 0)        //tamanho !0 = a fila nao vazia
  {
   aux=fila[inicio]; //quarda em aux o valor antes de removerFILA ele da fila
    fila[inicio]=-1;  //zera o valor da fila
     inicio++;         //incrementa o inicio para localizar o proximo com o modulo
     inicio = inicio % MAX; //localiza o proximo elemento com o modulo
    tamanho--;        //decrementa o tamanho pq removeu o elemento da fila
   return(aux);      //retorna o valor removido
  }
 else// tamanho da fila = 0 , fila vazia
 return(-1);    
}

// RotaMaisBarata
void CaminhoNoGrafo(int cidadeInicio)
{
 int retFILA;
 int cidadeBase;
 
 //inicia todas as cidades com a cor branca (nao visitada)
 for(int i = 0; i < MAX; i++)CorCidades[i]= 0;
  cidadeBase= cidadeInicio; //cidade inicial vai para cidadebase para calcular as ligacoes
 
 while(cidadeBase != -1)
  {
   // Marcar cidadeBase com a cor preta (ja visitada)
   CorCidades[cidadeBase]= 2;
   
   for(int j = 0;j < MAX; j++)
    if((GrafoCidades[cidadeBase][j] != -1) && (CorCidades[j]==0))
     {
      inserirFILA(j);
      CorCidades[j]==2;//cor preta (ja visitada)
     }
    // retira da fila uma cidade base (passa para a proxima cidade com ligacao)
   cidadeBase=removerFILA();
  }//while
    
  for(int i = 0;i < MAX; i++)
   if(CorCidades[i] == 2)
    printf("\n%s",NomeCidades[i]);
     printf("\n");  
}