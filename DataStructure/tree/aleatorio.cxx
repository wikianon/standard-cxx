#include <cstdio>
#include <cstdlib>

struct Arvore
{
 int elemento;  
 Arvore* esquerda;
 Arvore* direita;
};

Arvore* insereElementos(Arvore* Arv, int elemento)
{
 //Caso a árvore esteja vazia.
  if(Arv == NULL)
   {
    Arv = new Arvore(); //Aloca memória para a árvore
    Arv->elemento = elemento;
    Arv->esquerda = NULL;
    Arv->direita = NULL;
   }
  else
   {
    if(elemento < Arv->elemento)
     Arv->esquerda = insereElementos(Arv->esquerda, elemento);
    else 
    if(elemento > Arv->elemento)
     Arv->direita = insereElementos(Arv->direita, elemento);
   }
  return Arv;
}

void exibirOrd(Arvore *Arv)
{ //Imprime os valores da árvore em órdem crescente.
  if(Arv != NULL)
   {
    exibirOrd(Arv->esquerda);
    printf("%d ",Arv->elemento);
    exibirOrd(Arv->direita);    
   }
}

int main()
{
   Arvore* minhaArvore = NULL;
   int aleatorio = 0;

   for(int i=0; i<=20; i++)
   {
    aleatorio = rand()%250; //Irá gerar 20 números aleatórios menores que 250.
    minhaArvore = insereElementos(minhaArvore, aleatorio);
   }
   
   exibirOrd(minhaArvore);
   printf("\n");
  return 0;
} 
/*
 Output:
 13 27 40 43 59 85 112 133 136 149 165 171 172 176 190 211 236 242  
 */