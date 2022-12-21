#include <list>
#include <stack> // pilha para usar na DFS
#include <vector>
#include <iostream>
std::vector<int> aux;

class Grafo
{
    int V; // número de vértices
    std::list<int> *adj; // ponteiro para um array contendo as listas de adjacências

public:
    Grafo(int V); // construtor
    void adicionarAresta(int v1, int v2); // adiciona uma aresta no grafo

    // faz uma DFS a partir de um vértice
    void dfs(int v);
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

void Grafo::dfs(int v)
{
    std::stack<int> pilha;
    bool visitados[V]; // vetor de visitados

    // marca todos como não visitados
    for(int i = 0; i < V; i++)
        visitados[i] = false;

    while(true)
    {
        if(!visitados[v])
        {
            visitados[v] = true; // marca como visitado
            pilha.push(v); // insere "v" na pilha
            aux.push_back(v);
        }

        bool achou = false;
        std::list<int>::iterator it;

        // busca por um vizinho não visitado
        for(it = adj[v].begin(); it != adj[v].end(); it++)
        {
            if(!visitados[*it])
            {
                //std::cout << *it << " ";
                achou = true;
                break;
            }

        }

        if(achou){
            v = *it; // atualiza o "v"
        }
        else
        {
            for(std::list<int>::iterator it1 = adj[v].begin(); it1 != adj[v].end(); it1++)
	    {
                if(*it1==12){
                    for (int i = 0; i < (int)aux.size(); i++)
                    {
                        std::cout << aux[i] << std::endl;
                    }
                    std::cout << "****\n";

                }
            }

            // se todos os vizinhos estão visitados ou não existem vizinhos
            // remove da pilha
            aux.erase(aux.end()-1);
            pilha.pop();
            // se a pilha ficar vazia, então terminou a busca
            if(pilha.empty())
                break;
            // se chegou aqui, é porque pode pegar elemento do topo
            v = pilha.top();
        }
    }
}

int main()
{
    int V = 13;

    Grafo grafo(V);

    // adicionando as arestas
    grafo.adicionarAresta(0,1);
    grafo.adicionarAresta(1,2);
    grafo.adicionarAresta(2,11);
    grafo.adicionarAresta(2,3);
    grafo.adicionarAresta(3,4);
    grafo.adicionarAresta(4,5);
    grafo.adicionarAresta(4,7);
    grafo.adicionarAresta(5,6);
    grafo.adicionarAresta(6,10);
    grafo.adicionarAresta(7,8);
    grafo.adicionarAresta(8,9);
    grafo.adicionarAresta(9,10);
    grafo.adicionarAresta(10,11);
    grafo.adicionarAresta(11,12);

    grafo.dfs(4);

    return 0;
} 
