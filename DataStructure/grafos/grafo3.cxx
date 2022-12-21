#include <list>
#include <iostream>
#include <algorithm> // função find
// Grafos - Lista de adjacência
// https://gist.github.com/marcoscastro/76634e1d38d0d7eef2e1
struct Grafo
{
 Grafo(int V); // construtor
 void adicionarAresta(int v1, int v2); // adiciona uma aresta no grafo

 // obtém o grau de saída de um dado vértice
 // grau de saída é o número de arcos que saem de "v"
 int obterGrauDeSaida(int v);
 bool existeVizinho(int v1, int v2); // verifica se v2 é vizinho de v1
 
 private:
   int V; // número de vértices
   std::list<int> *adj; // ponteiro para um array contendo as listas de adjacências
};

Grafo::Grafo(int V)
{
 this->V = V; // atribui o número de vértices
 adj = new std::list<int>[V]; // cria as listas
}

void Grafo::adicionarAresta(int v1, int v2)
{
 // adiciona vértice v2 à lista de vértices adjacentes de v1
 adj[v1].push_back(v2);
}

int Grafo::obterGrauDeSaida(int v)
{
 // basta retornar o tamanho da lista que é a quantidade de vizinhos
 return adj[v].size();
}

bool Grafo::existeVizinho(int v1, int v2)
{
 if(find(adj[v1].begin(), adj[v1].end(), v2) != adj[v1].end())
 return true;
 return false;
}

int main()
{
 // criando um grafo de 4 vértices
 Grafo grafo(4);

 // adicionando as arestas
 grafo.adicionarAresta(0, 1);
 grafo.adicionarAresta(0, 3);
 grafo.adicionarAresta(1, 2);
 grafo.adicionarAresta(3, 1);
 grafo.adicionarAresta(3, 2);

 // mostrando os graus de saída
 std::cout << "Grau de saida do vertice 1: " << grafo.obterGrauDeSaida(1)
           << "\nGrau de saida do vertice 3: " << grafo.obterGrauDeSaida(3);

 // verifica se existe vizinhos
 if(grafo.existeVizinho(0, 1))
 std::cout << "\n\n1 eh vizinho de 0\n";
 else
 std::cout << "\n\n1 NAO eh vizinho de 0\n";

 if(grafo.existeVizinho(0, 2))
 std::cout << "2 eh vizinho de 0\n";
 else
 std::cout << "2 NAO eh vizinho de 0\n";

 return 0;
}