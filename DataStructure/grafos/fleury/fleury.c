/***************************************************************
Trabalho de Grafos

Implementacao do algoritmo de Fleury

Alunos: Anderson Luis Pontes de Goes
Luciano Bernardes de Paula


******************************************************************/
#include "fleury.h"
#include <stdlib.h>

int main (int argc, char *argv[])
{
 FILE *arqent;
 FILE *arqsai;
 
 int i, j, narestas, nvertices, aux;
 char teste[50];
 int grafo[MAX+1][MAX+1];
 int grau[MAX+1];
 int odd1, odd2;

 if(argc < 2)
  {
   printf("\n%s [arquivo de entrada]\n\n",argv[0]);
   exit(1);
  }
  
  i = 0;
  j = 0;
 
 // Inicializacao das matrizes com -1
 for(j=0; j<=MAX; j++)
  for(i=0; i<=MAX; i++)
   {
    grafo[j][i] = -1;
   }
 
 for(i = 0; i<=MAX; i++)
 {
  grau[i] = 0;
 }
 
 arqent = fopen(argv[1], "r");
 arqsai = fopen("resultados.txt", "a");
 
 fscanf(arqent, "%i", &nvertices);
 fscanf(arqent, "%i", &narestas);
 
 // Tratamento da entrada
 if(nvertices > MAX)
  {
   printf("\nO numero de vertices deve ser <= %i\n", MAX);
   exit(1);
  }
  
 // Inicializacao da area da matriz que sera usada
 for(i=0; i<=nvertices; i++)
  for(j=0; j<=nvertices; j++)
   {
    grafo[i][j] = 0;
   }
 aux = narestas;
 i = 0;
 j = 0;
 
 // Colocando as arestas na matriz
 while(aux > 0)
  {
   fscanf(arqent, "%i", &i);
   fscanf(arqent, "%i", &j);
   grafo[i][j] = 1;
   grafo[j][i] = 1;
   aux--;
  }
  
 // Calculando o grau dos vertices
 for(i = 1; i<=MAX; i++)
  for(j = 0; j<=MAX; j++)
   {
    if(grafo[i][j] == 1) grau[i]++;
   }

 // Verificando a quantidade de vertices com grau impar
 j = 2;
 aux = 0;
 odd1 = 0;
 odd2 = 0;
 
 for(i = 1; i<=MAX; i++)
  {
   if(grau[i]%j != 0)
    {
     aux++;
     grau[i] = -1;
    }
  }
  
 // Marcando quem sao os vertices de grau impar
 i = 1;
 
 if(aux == 2)
  {
   while((odd1 == 0)||(odd2 == 0))
    {
     if(grau[i] == -1)
      {
       if(odd1 == 0)
        {
         odd1 = i;
        }
       else
       odd2 = i;
      }
     i++;
    }
  }

 // Definindo a existencia ou nao de um Circuito ou Caminho Euleriano
 if(aux == 0)
  {
   printf(
          "\nTodos os vertices possuem grau par,"
          "portanto existe um Circuito Euleriano.\n"
          "Este programa achara este circuito e gerara um\n"
          "arquivo chamado resultados.txt contendo este circuito."
         );
  }
 else
  {
   if(aux == 2)
    {
     printf(
            "\nExistem 2 vertices com grau impar, portanto "
            "\nnao existe Circuito Euleriano,\nmas existe um Caminho Euleriano."
            "\nEste programa achara este caminho e gerara um"
            "\narquivo chamado resultados.txt contendo este caminho."
           );
    }
   else
    {
     printf(
            "\nExiste(m) %i vertice(s) com grau impar, o que "
            "impossibilita a \nexistencia de um Caminho ou Circuito Euleriano.",aux
           );
    }
  }
  printf("\n");

 // Se nao existem vertices com grau impar
 if(aux == 0)EulerCircuit(grafo, narestas, nvertices);

 // Se existem exatamente 2
 if(aux == 2)EulerPath(grafo, narestas, nvertices, odd1, odd2);

 fprintf(arqsai, "Grafo do arquivo: %s\n", argv[1]);
  for(i=1; i<=nvertices; i++)
   {
    for(j=1; j<=nvertices; j++)
     {
      printf("%i ", grafo[i][j]);
      fprintf(arqsai, "%i ", grafo[i][j]);
      if(j == nvertices)
       {
        printf("\n");
        fprintf(arqsai, "\n");
       }
     }
   }
 
 printf("\n");
 fprintf(arqsai, "\nCaminho Euleriano encontrado:\n");
 
 fprintf(arqsai, "%i", passos[1]);
  for(i = 2; i <= narestas+1; i++)
   fprintf(arqsai, " -> %i", passos[i]);
 
 fprintf(arqsai, "\n\n\n");
 fclose(arqent);
 fclose(arqsai);
 return 0;
}