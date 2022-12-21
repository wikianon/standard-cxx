#include <iostream>
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
 * @file       grafo1
 * @version    0.1
 * @brief      Grafo de ligações telefônicas.
 * @consult    http://www.facom.ufu.br/~madriana/EBD/
 * @author     Jean Zonta
 * @Copyright (C) 2017 Jean Zonta.
 * 
 * @end @section author Author
 *
 */

struct Grafo
{
 int verticeGr;
 Grafo *adj;
 Grafo *prox;
};

Grafo *criaNo(int vertice, Grafo *p)
{
 Grafo *x = new Grafo();
 
 x->verticeGr = vertice;
 x->prox = p;
 return x;
}

Grafo *criaGrafoNo(int vertice, Grafo *p, Grafo *g)
{
 Grafo *x = new Grafo();
 
 x->verticeGr = vertice;
 x->adj = p;
 x->prox = g;
 return x;
}

void inserirAdj(Grafo *g)
{
 Grafo *l = NULL;
 int i;
 int qtd;
 int vertice;
 
 while (g != NULL)
 {
  std::cout << "\n\tInforme o numero de ligacoes\n\tpartindo da cidade " << g->verticeGr << ": ";
  std::cin >> qtd;
  std::cout << std::endl;
  for(i=0;i < qtd; i++)
  {
   std::cout<<"\n\tCodigo da cidade de chegada: ";
   std::cin>>vertice;
   l = criaNo(vertice,l);
  }
  g->adj = l;
  g = g->prox;
 }
}

void percorrerAdj(Grafo *g)
{
 while(g != NULL)
  {
   std::cout << "\n\tCodigo cidade: " << g->verticeGr;
   
   while(g->adj != NULL)
    {
     std::cout << "\n\tDe cidade " << g->verticeGr << " para " << g->adj->verticeGr;
     g->adj = g->adj->prox;
    }
   g = g->prox;
  }
}

int main()
{
 int i;
 int qtd;
 int vertice;
 
 Grafo *g = NULL;
 
 std::cout << "\n\tInsira a quantidade de cidades (vertices): ";
 std::cin >> qtd;
 
 for(i=0;i<qtd;i++)
 {
  std::cout << "\n\tInforme o codigo da cidade: ";
  std::cin >> vertice;
  g = criaGrafoNo(vertice,NULL,g);
 }
 
 inserirAdj(g);
 percorrerAdj(g);
 std::cout << "\n\n";
 return 0;
}