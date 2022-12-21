/************************************************************


Trabalho de Grafos

Implementacao do Algoritmo de Fleury


Alunos: Anderson Luis Pontes de Goes
        Luciano Bernardes de Paula


*************************************************************/


# include <stdio.h>
# define MAX 50

int gaux[MAX+1][MAX+1];   // Grafo auxiliar
int passos[MAX+1];        // Vetor que guardara os passos

//-----------------------------------------------------------------
// Funcao que acha o Circuito Euleriano, dado um grafo conexo e
// com todos seus vertices com grau par

void EulerCircuit (int graph[MAX+1][MAX+1], int ar, int vert)
{
  int a, b, aux, vatual, tudozero, zlin, nones;
  tudozero = 1;
  vatual = vert+1;

  printf("\nEntre com o vertice inicial (escolha de 1 a %i): ", vert);

  // Tratamento de entrada
  while ((vatual > vert)||(vatual < 1))
  {
  scanf("%i", &vatual);
  if ((vatual > vert)||(vatual < 1))
       printf("\nPor favor escolha um numero de 1 a %i: ", vert);
  }

  // Inicializacoes
  for(a = 0; a < MAX+1; a++)
	     passos[a] = -1;

  passos[1] = vatual;

  for (a=0; a<=MAX; a++)
    for (b=0; b<=MAX; b++)
     gaux[a][b] = graph[a][b];

  // Enquanto nao zerou toda a matriz
  while(tudozero <= ar)
  {
    zlin = 0;

    while(zlin == 0)
    {
       b = 1;
       nones = 0;
       while((nones == 0)&&(b <= vert))
       {
	 if((gaux[vatual][b] == 1) && (b != passos[tudozero-1])&&
	      (b != passos[tudozero+1]))
	  { nones = 1; }

	 else
	  { b++; }
       }

      zlin = 1;

      // Retirando uma aresta
      if(b <= vert)
      {
       gaux[vatual][b] = 0;
       gaux[b][vatual] = 0;
       tudozero++;
       passos[tudozero] = b;
       vatual = b;
       zlin = 0;
      }

    }

    if (tudozero <= ar)
      {
	 zlin = 1;

	 // Se nao houve sucesso, refaz um passo
	 while (zlin == 1)
	 {
	   gaux[passos[tudozero]][passos[tudozero-1]] = 1;
	   gaux[passos[tudozero-1]][passos[tudozero]] = 1;
	   tudozero--;

	   nones = 0;
	   for(a = 1; a <= vert; a++)
	       if (gaux[passos[tudozero]][a] == 1) nones++;

	   if (nones > 1)
	   {
	      vatual = passos[tudozero];
	      zlin = 0;
	   }

	 }
       }

       // Se terminou...
       else
       {
	 printf("\nCaminho encontrado:\n");
	 for (aux = 1; aux <= ar; aux++)
	 printf("%i -> ", passos[aux]);
	 printf("%i", passos[ar+1]);
       }
  }
  printf("\n");
}
//---------------------------------------------------------------------


//--------------------------------------------------------------------
// Funcao que acha um Caminho Euleriano, dado um grafo conexo e com
// exatamente 2 vertices com grau impar...

void EulerPath (int graph[MAX+1][MAX+1], int ar, int vert, int imp1, int imp2)
{
  int a, b, aux, vatual, tudozero, zlin, nones;
//  int special, ini;
  tudozero = 1;
  vatual = vert+1;
//  special = -1;
  printf("\nEntre com o vertice inicial (escolha %i ou %i): ", imp1, imp2);

  a = 1;
  while (a == 1)
  {
   scanf("%i", &vatual);
   if ((vatual == imp1)||(vatual == imp2))
	{
	 a = 0;
	}
   else
   { printf("\nPor favor escolha entre %i ou %i: ", imp1, imp2);}
  }

  for(a = 0; a < MAX+1; a++)
	     passos[a] = -1;

  passos[1] = vatual;
//  ini = vatual;

  for (a=0; a<=MAX; a++)
    for (b=0; b<=MAX; b++)
     gaux[a][b] = graph[a][b];



  while(tudozero <= ar)
  {
    zlin = 0;

    while(zlin == 0)
    {
       b = 1;
       nones = 0;
       while((nones == 0)&&(b <= vert))
       {

	  if((gaux[vatual][b] == 1) && (b != passos[tudozero-1])&&
	      (b != passos[tudozero+1]))
	  {
		  nones = 1;
	  }

	  else
	   {
	      b++;
	   }

       }

      zlin = 1;
      if(b <= vert)
      {
       gaux[vatual][b] = 0;
       gaux[b][vatual] = 0;
       tudozero++;
       passos[tudozero] = b;
       vatual = b;
       zlin = 0;
      }
    }

    if (tudozero <= ar)
       {
	 zlin = 1;
	 while (zlin == 1)
	 {
	   gaux[passos[tudozero]][passos[tudozero-1]] = 1;
	   gaux[passos[tudozero-1]][passos[tudozero]] = 1;
	   tudozero--;

	   nones = 0;
	   for(a = 1; a <= vert; a++)
	       if (gaux[passos[tudozero]][a] == 1) nones++;

	   if (nones > 1)
	   {
	      if((passos[tudozero] != imp1)&&(passos[tudozero] != imp2))
	      {
		vatual = passos[tudozero];
		zlin = 0;
	      }
	      if (tudozero == 1)
	      {
	       vatual = passos[tudozero];
	       zlin = 0;
	       for (a = 3; a <= vert; a++) passos[a] = -1;
	      }
	   }

	 }

       }
       else
       {
	 printf("\nCaminho encontrado:\n");
	 for (aux = 1; aux <= ar; aux++)
	 printf("%i -> ", passos[aux]);
	 printf("%i", passos[ar+1]);
       }
  }
  printf("\n");
}