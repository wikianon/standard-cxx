/* libvalidacao.h ***********************************************************/
/*                                                                          */
/* DESCRICAO                                                                */
/*    Biblioteca do programa validacao responsavel por implementar as roti- */
/* nas de validacao de CNPJ e CPF.                                          */
/*                                                                          */
/* INFORMACOES                                                              */
/*    AUTOR : Jose Lopes de Oliveira Junior                                 */
/*    SISTEMA OPERACIONAL : Slackware Linux 10.1 ( kernel 2.4.29 )          */
/*    COMPILADOR : GCC 3.3.4 ( GNU COMPILER C )                             */
/*    DATA : jul/2005                                                       */
/*    CONTATO : jlojunior@gmail.com                                         */
/*                                                                          */
/****************************************************************************/

/* Bibliotecas importadas */
#include <stdio.h>

/* ctoi *********************************************************************/
/* DESCRICAO                                                                */
/*    Funcao que retorna o valor inteiro de um digito a partir de sua repre-*/
/* sentacao em caracter.                                                    */
/*                                                                          */
/* ENTRADA                                                                  */
/*  > char ch : Caracter que sera convertido.                               */
/*                                                                          */
/* RETORNO                                                                  */
/*    Retorna a representacao em inteiro do caracter. Caso o parametro pas- */
/* sado nao seja um digito, o valor retornado eh indefinido.                */
/****************************************************************************/
char ctoi ( char ch ) {
   return ( ch - 48 );
}

/* validaCPF ****************************************************************/
/* DESCRICAO                                                                */
/*    Funcao que verifica a validade de um numero de CPF.                   */
/*                                                                          */
/* ENTRADA                                                                  */
/*  > char *cpf - Numero de CPF a ser verificado - no tipo caracter.        */
/*                                                                          */
/* RETORNO                                                                  */
/*    Retorna 1 caso o numero seja valido, ou 0 em caso contrario.          */
/****************************************************************************/
char validaCPF ( char *cpf ) {
   /* Variaveis usadas no controle das iteracoes */
   /* para o calculo dos digitos verificadores.  */
   register char i, j;
   
   /* Armazena os digitos verificadores calculados. */
   char digVer;
   
   /* Guarda o somatorio usado para se calcular */
   /* cada um dos digitos verificadores.        */
   unsigned int somatorio = 0;
   
   /* Variavel auxiliar que armazenarah */
   /* os digitos verificadores calcula- */
   /* dos como caracteres.              */
   char cpfAux[ 2 ];
   
   /* Executando o calculo do primeiro digito */
   /* verificador do numero de CPF.           */
   
   /* Calculando o somatorio-base do CPF e */
   /* guardando-o na variavel somatorio.   */
   for ( i = 0, j = 10; i <= 8; i++, j-- )
      somatorio += ctoi( *( cpf + i ) ) * j;
   
   /* Concluindo o calculo do primeiro digito verificador. */
   ( ( somatorio % 11 ) < 2 ) ? ( digVer = 0 ) :
      ( digVer = 11 - ( somatorio % 11 ) );
   
   /* Convertendo o digito verificador encontrado */
   /* para caracter, para futura comparacao.      */
   sprintf( cpfAux, "%d", digVer );
   
   /* Reinicializando o somatorio */
   somatorio = 0;
   
   /* Executando o calculo do segundo digito */
   /* verificador do numero de CPF.          */
   
   /* Calculando o somatorio-base do CPF e */
   /* guardando-o na variavel somatorio.   */
   for ( i = 0, j = 11; i <= 9; i++, j-- )
      somatorio += ctoi( *( cpf + i ) ) * j;
   
   /* Concluindo o calculo do se- */
   /* gundo digito verificador.   */
   ( ( somatorio % 11 ) < 2 ) ? ( digVer = 0 ) :
      ( digVer = 11 - ( somatorio % 11 ) );
   
   /* Convertendo o digito verificador encontrado */
   /* para caracter, para futura comparacao.      */
   sprintf( cpfAux, "%s" "%d", cpfAux, digVer );
   
   /* Caso os digitos vericadores encontrados pelo programa forem */
   /* iguais aos do numero digitado pelo usuario, o numero eh va- */
   /* lido. Caso contrario, nao.                                  */
   if ( ( *( cpf + 9 ) == *( cpfAux + 0 ) ) && 
      ( *( cpf + 10 ) == *( cpfAux + 1 ) ) ) 
      return 1;
   else
      return 0;
}

/* validaCNPJ ***************************************************************/
/* DESCRICAO                                                                */
/*    Funcao que verifica a validade de um numero de CNPJ.                  */
/*                                                                          */
/* ENTRADA                                                                  */
/*  > char *cnpj - Numero de CNPJ a ser verificado - no tipo caracter.      */
/*                                                                          */
/* RETORNO                                                                  */
/*    Retorna 1 caso o numero seja valido, ou 0 em caso contrario.          */
/****************************************************************************/
char validaCNPJ ( char *cnpj ) {
   /* Variaveis usadas no controle das iteracoes */
   /* para o calculo dos digitos verificadores.  */
   register char i, j;
   
   /* Armazena os digitos verificadores calculados. */
   char digVer;
   
   /* Guarda o somatorio usado para se calcular */
   /* cada um dos digitos verificadores.        */
   unsigned int somatorio = 0;
   
   /* Variavel auxiliar que armazenarah */
   /* os digitos verificadores calcula- */
   /* dos como caracteres.              */
   char cnpjAux[ 2 ];
   
   /* Executando o calculo do primeiro digito */
   /* verificador do numero de CNPJ.          */
   
   /* Calculando o somatorio-base do CNPJ e */
   /* guardando-o na variavel somatorio.    */
   for ( i = 0, j = 5; j >= 2; i++, j-- )
      somatorio += ctoi( *( cnpj + i ) ) * j;
   
   for ( j = 9; j >= 2; i++, j-- )
      somatorio += ctoi( *( cnpj + i ) ) * j;
   
   /* Concluindo o calculo do primeiro digito verificador.   */
   ( ( somatorio % 11 ) < 2 ) ? ( digVer = 0 ) :
      ( digVer = 11 - ( somatorio % 11 ) );
   
   /* Convertendo o digito verificador encontrado */
   /* para caracter, para futura comparacao.      */
   sprintf( cnpjAux, "%d", digVer );
   
   /* Reinicializando o somatorio */
   somatorio = 0;
   
   /* Executando o calculo do segundo digito */
   /* verificador do numero de CNPJ.         */
   
   /* Calculando o somatorio-base do CNPJ e */
   /* guardando-o na variavel somatorio.    */
   for ( i = 0, j = 6; j >= 2; i++, j-- )
      somatorio += ctoi( *( cnpj + i ) ) * j;
   
   for ( j = 9; j >= 2; i++, j-- )
      somatorio += ctoi( *( cnpj + i ) ) * j;
   
   /* Concluindo o calculo do segundo digito verificador. */
   ( ( somatorio % 11 ) < 2 ) ? ( digVer = 0 ) :
      ( digVer = 11 - ( somatorio % 11 ) );
   
   /* Convertendo o digito verificador encontrado */
   /* para caracter, para futura comparacao.      */
   sprintf( cnpjAux, "%s" "%d", cnpjAux, digVer );
   
   /* Caso os digitos vericadores encontrados pelo programa forem */
   /* iguais aos do numero digitado pelo usuario, o numero eh va- */
   /* lido. Caso contrario, nao.                                  */
   if ( ( *( cnpj + 12 ) == *( cnpjAux + 0 ) ) && 
      ( *( cnpj + 13 ) == *( cnpjAux + 1 ) ) ) 
      return 1;
   else
      return 0;
}
