#include <cstdio>
#include <cstring>

/*
Este programa de determinacao de um trajeto Euleriano em um digrafo:

Algoritmo de HierHolzer

Deixe u ser qualquer vertice de G. HEAD e TAIL sao pilhas. 
Inicialize HEAD = {u}, TAIL=Æ. O vertice u refere-se ao topo da pilha.

Enquanto HEAD ¹ Æ faca
{
 Enquanto (grau de saida de u na pilha HEAD > 0)  faca
 {
  Encontre um vertice v adjacente a u
  Adicione v no topo de HEAD
  Apague a aresta (u,v) da lista de adjacencia
 Decremente o grau de saida de u por 1
}
Enquanto ( (HEAD ¹ 0) e (u na pilha HEAD = 0) ) faca
{
 Remova o vertice u do topo de HEAD
 Adicione u a TAIL
 }
}

Um trajeto Euleriano estara em TAIL. 

OBS: Retirado do livro FLEISCHNER, Herbert: 
"Eulerian Graphs And Related Topics", Parte 1, volume 2, editora North-Holland, 1991.
https://www.dcce.ibilce.unesp.br/~socorro/TG/programas/Trajeto_Euleriano/Ives_Tiago/windows/
*/

#define N 300 //numero maximo de vertices

FILE *pt;     //ponteiro para arquivo de entrada

int lerarq(void);
int leialinha(FILE *pt,char *v1,char *v2);
void help(void);  //mostra help do programa
/* Estrutura de Dados

O algoritmo de HierHolzer utilizou duas pilhas para
HEAD e TAIL como foi mostrado nas secoes anteriores. 
Alem da utilizacao dessas pilhas, foi necessario a utilizacao
de uma lista que armazena as informacoes pertinentes a cada vertice. 
Cada vertice possui as seguintes informacoes:

Nome
Grau de saida
Grau de entrada
Vertices adjacentes

Cada campo (indice) da lista possui as 4 informacoes anteriores referentes a um vertice. 
Com isso, podemos utilizar apenas o indice da lista para identificar qual e o vertice em questao, 
quais sao os seus vertices adjacentes, qual o seu grau de entrada e o seu grau de saida.
*/

struct PilhaE
{
 int pilha[N];
 int topo;

 PilhaE(void){topo=-1;};
 int Insere(int v);          //insere um vertice na pilha
 int Tamanho(void) const;  //retorna o topo da pilha
 int Tiratopo(void);    //retorna o topo e o tira da pilha
 int vazia(void);
 int Topo(void){return pilha[topo];};
};

int PilhaE::vazia(void)
{
  if (topo<0) return 1;
  else return 0;
}

int PilhaE::Insere(int v)
{
 if ( topo == N )  return 0;  //estouro de pilha
  topo++;
  pilha[topo]=v;
 return 1;
}

int PilhaE::Tamanho(void) const
{
 return topo+1;
}

int PilhaE::Tiratopo(void)
{
 return pilha[topo--];
}


struct Vertice
{
 char nome[10];
  PilhaE adjacentes;
  int grau_entrada;
 int grau_saida;
};


struct lstVertice
{
 int ult;  //ultimo vertice adicionado 
 Vertice lista[N];
 
 lstVertice(void): ult(-1){};
 
 //retornar a posicao do vertice na lista
 int Inserevertice(Vertice v);
};

int lstVertice::Inserevertice(Vertice v)
{
 int i = 0;
 
  if(ult < 0) //lista vazia
   {
    lista[++ult]=v;
    return ult;
   }
   while(i <= ult)
   {
    if(strcmp(lista[i].nome,v.nome) == 0)return i;
    i++;
   }
   
  if(i > N)
  return -1;
  else
   {
    lista[++ult]=v;
    return ult;
   }
}

//variaveis globais
lstVertice listav;
//fim das variaveis globais

void help(void)
{
 printf( 
        "\n\n\t**********************************  HELP  **************************************"
        "\n\tUSO: ./grafos <arquivo> <opcao>\n\n\tOnde <arquivo> e' o nome do arqui_vo"
        "\n\tde entrada, contendo os vertices e arestas do Digrafo. Especifique na primeira"
        "\n\tlinha o numero de vertices e o numero de arestas do Digrafo."
        "\n\tExemplo:\n\t2 2\n\tv1 v2\n\tv2 v1\n\tA notacao 'v1 v2'significa que a aresta entre os vertices de nome v1 e v2"
        "\n\ttem orientacao sentido v1 -> v2.\n"
        "\n\tA <opcao> indica ao programa que o Digrafo ou as componentes do Digrafo"
        "\n\tsao Eulerianos e portanto o programa nao verificara' esse fato,"
        "\n\teconomizando tempo de processamento. Caso vc deseje fazer o preprocessamento,"
        "\n\tnao preencha esta opcao, pois ela e' habilitada por default. Se voce tiver"
        "\n\tcerteza que o Digrafo ou as compo nentes do Digrafo sao Eulerianos,"
        "\n\tmarque esta opcao como 'n' ou 'N', exemplo:\n\n\tgrafos dados.txt n."
        "\n\tUTILIDADE: Este programa determina um trajeto euleriano em um Digrafo,"
        "\n\tverificando se o mesmo e' Euleriano, informa tambem se o Digrafo e'"
        "\n\tou nao conexo.\n\n\tElaborado por: Ives Rene Venturini Pola e Tiago de Oliveira"
        "\n\t********************************** FIM HELP  **************************************\n\n"
       );
}


/* Programa Principal */
int main(int argc,char *argv[])
{
   PilhaE head,tail;
   char inicial[10];
   int vert_inic=0;
   int preprocessamento=1;
   
   if((pt = fopen(argv[1],"r")) == NULL)
   {
    help(); 
    printf("\n\tErro ao abrir arquivo..\n");
    return 0;
   }
   //verificar opcao de preprocessamento
   if(argv[2] != NULL)
   {
    if((argv[2])[0] == 'n' || (argv[2])[0] == 'N')
    preprocessamento=0;
   }
   //fim da verificacao do preprocessamento
   if (lerarq() == 0)  //ler arquivo de dados
   {
    printf("\n\tErro no arquivo de entrada...\n");
    return 0;
   }
   fclose(pt);
   
   /*
    Preprocessamento 
    O usuario pode optar por fazer o pre-processamento,
    antes da execucao do algoritmo de HierHolzer que determinara um trajeto Euleriano.
    Esse pre-processamento visa identificar se o digrafo ou os componentes,
    de um digrafo desconexo sao ou nao Eulerianos,
    ou seja, se satisfazem a equacao abaixo.
    
    Gs(vi) = Ge(vi) , onde
    G = grau
    s = saida
    e = entrada
    Para identificar se o grau de saida e igual
    ao grau de entrada de todos os vertices,
    o programa percorre a lista de vertices e verifica os campos:
    grau de saida e grau de entrada de cada vertice existente na lista.
   */

   if(preprocessamento == 1)
   {
    printf("\n\tIniciando Pre-processamento...\n");
    while(vert_inic <= N)
    {
      if(listav.lista[vert_inic].grau_entrada != listav.lista[vert_inic].grau_saida)
      {
        printf("\n\tO Digrafo nao e' Euleriano..\n\n");
        return 0;
      }
     vert_inic++;
    }
    printf("\n\tPre-processamento concluido,\n\to Digrafo ou as componentes\n\tdo Digrafo sao Eulerianas.");
   }
   
   vert_inic=0;
   
   //fim preprocessamento
   printf("\n\tEntre com o nome do vertice inicial: ");
   scanf("%s",inicial);
   
   while(strcmp(listav.lista[vert_inic].nome,inicial) != 0 && vert_inic <= N)vert_inic++;
   
   if(vert_inic > N)
   {
    printf("\n\tO vertice escolhido nao foi identificado.\n\tO programa e' case sensitive. \n");
    return 0;
   }


   //Algoritmo de Hierholzer

   head.Insere(vert_inic);

   while(head.vazia() != 1)
   {
     while(listav.lista[head.Topo()].adjacentes.Tamanho() != 0 )
     {
       head.Insere(listav.lista[head.Topo()].adjacentes.Tiratopo());
     }
     while(head.vazia() != 1 && listav.lista[head.Topo()].adjacentes.Tamanho() == 0)
     {
       tail.Insere(head.Tiratopo());
     }
   }
   //fim do Algoritmo de Hierholzer.
   
   /*
     Verificar se é desconexo. A identificacao da conectividade
     do digrafo e bastante simples, pois possui uma estrutura
     "if" dentro de um "while". Essas estruturas simplesmente
     percorrem a lista dos vertices verificando se houve alguma
     aresta nao alcancada pelo algoritmo de HierHolzer.
   */
   
   vert_inic=0;
   while(vert_inic <= listav.ult )
   {
    if(listav.lista[vert_inic++].adjacentes.Tamanho() != 0)
    {
     printf(
            "\n\tO Digrafo e' desconexo, logo nao é Euleriano.\n\tPorém suas componentes sao Eulerianas."
            "\n\tO trajeto euleriano na componente cujo\n\tvertice inicial esta contido e': "
           );
     
     while(tail.vazia() != 1 )
        printf("%s ",listav.lista[tail.Tiratopo()].nome);
     printf("\n\n");
     return 1;
    }
   }
   
   printf(
          "\n\tO Digrafo e' Conexo e Euleriano"
          "\n\tO trajeto euleriano a partir do"
          "\n\tvertice inicial escolhido e':"
         );
   
   while(tail.vazia() != 1)
   printf("%s ",listav.lista[tail.Tiratopo()].nome);
  printf("\n\n");
 return 1;
}
//Fim do programa Principal

int lerarq(void)
{
   //esta funcao le o arquivo de entrada e guarda em listavertice os vertices
   //alem de armazenar em cada vertice seus adjacentes em forma de pilha

   char ch;
   ch=getc(pt);
   while(ch!='\n') ch=getc(pt);

   char v1[10],v2[10];
   Vertice vs,ve;
   vs.grau_entrada=0;vs.grau_saida=0;
   ve.grau_entrada=0;ve.grau_saida=0;

   int fim=0;
   int i=0,j=0;
   fim = leialinha(pt,v1,v2);
   while (fim != 2)
   {
        strcpy(vs.nome,v1);
        strcpy(ve.nome,v2);

        i = listav.Inserevertice(vs);
        j = listav.Inserevertice(ve);

        listav.lista[i].grau_saida++;
        listav.lista[j].grau_entrada++;

        listav.lista[i].adjacentes.Insere(j);

        fim = leialinha(pt,v1,v2);
   }

   return 1;
}

int leialinha(FILE *pt,char *v1,char *v2)
{
   //lee uma linha do arquivo de entrada e retorna 2 strings contendo
   //o nome dos vertices, retorna por referencia.

   int i=0;
   char ch = getc(pt);
   while( (ch != ' ') && (ch != EOF) && (ch !='\n') )
   {
      v1[i++]= ch ;
      ch=getc(pt);
   }
   if(ch == EOF) return 2; //algo errado
   v1[i]='\0';
   i=0;
   ch=getc(pt);
   while(ch == ' ') ch=getc(pt);
   while( (ch != ' ') && (ch != EOF) && (ch !='\n') )
   {
      v2[i++]= ch ;
      ch=getc(pt);
   }
   v2[i]='\0';
   if (ch== ' ')
     while ( (ch != EOF) && (ch != '\n') )  ch=getc(pt);
   if (ch == EOF) return 2;  //fim de arquivo
   if (ch == '\n') return 1; //fim de linha
   return 0; //algo errado.
}