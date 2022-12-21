#include <iostream>

struct Node
{
  Node(int valor): valor(valor), esquerda(NULL), direita(NULL){}
  
  Node* getEsquerda()
  {
   return this->esquerda;
  }
  
  Node* getDireita()
  {
   return this->direita;
  }
  
  int getValor()
  {
   return this->valor;
  }
  
  void setEsquerda(int v)
  {
   this->esquerda = new Node(v);
  }
   
  void setDireita(int v)
  {
   this->direita = new Node(v);
  }
  
   private:
     int valor;
     Node* esquerda;
     Node* direita;         
};

struct Arvore
{
  Arvore(int valor)
  {
   raiz = new Node(valor);
  }
  
  Node* getRaiz()
  {
   return this->raiz;
  }
  
  int contar(Node* atual)
  {
   if(atual == NULL)return 0;
    else
     {
      int contador = 1;
      printf("Atual = %d\n", atual->getValor());
      contador += contar(atual->getEsquerda());
      contador += contar(atual->getDireita());
      
      return contador;
     }
  }
  
  void inserir(Node* atual, int v)
  {
   if(v > atual->getValor())
    {
     if(atual->getDireita() != NULL)inserir(atual->getDireita(), v);
      else
       {
        atual->setDireita(v);
        printf("Inserindo %d a direita de %d.\n", v, atual->getValor());
       }
    }
   else
    {
     if(atual->getEsquerda() != NULL)inserir(atual->getEsquerda(), v);
      else
       {
        atual->setEsquerda(v);
        printf("Inserindo %d a esquerda de %d.\n", v, atual->getValor());
       }
    }
  }
  
  private:
    Node* raiz;      
};

int main()
{
    int vetor[] = {5, 8, 12, 0, 17, 2, 31, 11};
    
    Arvore* tree = new Arvore(vetor[0]);
    Node* raiz = tree->getRaiz();
    
    for(int i = 1; i < 8; i++)
       tree->inserir(raiz, vetor[i]);
    
    printf("Valor da raiz = %d\n", raiz->getValor());
    
    printf("Quantidade de nodes: %d\n", tree->contar(raiz));
    
    return EXIT_SUCCESS;
}
/*
 Output:
 Inserindo 8 a direita de 5.
 Inserindo 12 a direita de 8.
 Inserindo 0 a esquerda de 5.
 Inserindo 17 a direita de 12.
 Inserindo 2 a direita de 0.
 Inserindo 31 a direita de 17.
 Inserindo 11 a esquerda de 12.
 Valor da raiz = 5
 Atual = 5
 Atual = 0
 Atual = 2
 Atual = 8
 Atual = 12
 Atual = 11
 Atual = 17
 Atual = 31
 Quantidade de nodes: 8
 */