#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define ARQ_LISTA_PALAVRAS        "facil.txt"
#define LINHA_MAX_TAM             (100)
#define QTD_PALAVRAS_SORTEADAS    (10)


typedef struct lista_palavras_s
{
    char ** palavras;
    int cont;
} lista_palavras_t;

lista_palavras_t * carregar_lista_palavras( const char * arq )
{
    char linha[ LINHA_MAX_TAM + 1 ] = {0};
    int n = 0;
    FILE * pf = NULL;
    lista_palavras_t * lst = NULL;

    pf = fopen( arq, "r");

    if( !pf )
        return NULL;

    lst = (lista_palavras_t*) calloc( 1, sizeof(lista_palavras_t) );

    while( fgets( linha, LINHA_MAX_TAM, pf ) )
    {
        linha[strcspn(linha, "\n")] = 0;
        linha[strcspn(linha, "\r")] = 0;

        n++;

        lst->palavras = (char**) realloc( lst->palavras, sizeof(char*) * n );

        lst->palavras[n-1] = (char*) calloc( strlen(linha) + 1, sizeof(char) );

        strcpy( lst->palavras[n-1], linha );
    }

    fclose(pf);

    lst->cont = n;

    return lst;
}

lista_palavras_t * sortear_palavras( lista_palavras_t * lst, int qtd )
{
    int i = 0;
    lista_palavras_t * sort = NULL;

    sort = (lista_palavras_t*) calloc( 1, sizeof(lista_palavras_t) );

    sort->palavras = (char**) calloc( qtd, sizeof(char*) );

    for( i = 0; i < qtd; i++ )
        sort->palavras[i] = strdup( lst->palavras[ rand() % lst->cont ] );

    sort->cont = qtd;

    return sort;
}

void exibir_lista_palavras( lista_palavras_t * lst )
{
    int i = 0;

    for( i = 0; i < lst->cont; i++ )
        fprintf( stdout, "%d - %s\n", i + 1, lst->palavras[i] );
}

void liberar_lista_palavras( lista_palavras_t * lst )
{
    int i = 0;

    for( i = 0; i < lst->cont; i++ )
        free(lst->palavras[i]);

    free(lst->palavras);

    free(lst);
}

int main( int argc, char ** argv )
{
    lista_palavras_t * lst = NULL;
    lista_palavras_t * sort = NULL;

    srand(time(NULL));

    lst = carregar_lista_palavras( ARQ_LISTA_PALAVRAS );

    if( !lst )
    {
        fprintf( stderr, "Erro ao carregar arquivo: %s\n", ARQ_LISTA_PALAVRAS );
        return EXIT_FAILURE;
    }

    sort = sortear_palavras( lst, QTD_PALAVRAS_SORTEADAS );

    exibir_lista_palavras( sort );

    liberar_lista_palavras( lst );
    liberar_lista_palavras( sort );

    return EXIT_SUCCESS;
}