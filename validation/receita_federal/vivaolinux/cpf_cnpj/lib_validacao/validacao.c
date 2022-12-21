/* validacao ****************************************************************/
/*                                                                          */
/* DESCRICAO                                                                */
/*    O programa validacao tem a funcao de verificar se um numero de CPF ou */
/* CNPJ eh valido.                                                          */
/*                                                                          */
/* INFORMACOES                                                              */
/*    AUTOR : Jose Lopes de Oliveira Junior                                 */
/*    SISTEMA OPERACIONAL : Slackware Linux 10.1 ( kernel 2.4.29 )          */
/*    COMPILADOR : GCC 3.3.4 ( GNU COMPILER C )                             */
/*    DATA : jul/2005                                                       */
/*    CONTATO : jlojunior@gmail.com                                         */
/*                                                                          */
/* *  A biblioteca libvalidacao acompanha este programa e eh fundamental pa-*/
/* ra o funcionamento deste.                                                */
/*                                                                          */
/****************************************************************************/

/* Bibliotecas importadas */
#include <ctype.h>
#include <stdio.h>
#include "libvalidacao.h"

/* isNumero *****************************************************************/
/* DESCRICAO                                                                */
/*    Verifica se uma String possui apenas caracteres numericos, ou seja,   */
/* se a String eh um numero valido.                                         */
/*                                                                          */
/* ENTRADA                                                                  */
/*  > char *numero : A String  a ser avaliada.                              */
/*                                                                          */
/* RETORNO                                                                  */
/*    Retorna 1 caso a String seja um numero valido ou 0 caso algum caracte-*/
/* re dessa String nao seja um digito.                                      */
/****************************************************************************/
char isNumero ( char *numero )
{
 register char i; /* Variavel de controle da iteracao */
   
 for(i = 0; i <= strlen( numero ) - 1; i++ )
  if(!isdigit(*( numero + i )))
   return 0;
   return 1;
}

/* main *********************************************************************/
/* DESCRICAO                                                                */
/*    Funcao principal do programa. Nela sao verificados os parametros pas- */
/* dos ao programa.                                                         */
/*                                                                          */
/* ENTRADA                                                                  */
/*  > int argc     : O contador de argumentos.                              */
/*  > char *argv[] : A matriz que armazena todos os argumentos.             */
/****************************************************************************/
main ( int argc, char *argv[] ) {
   /* Variaveis que armazenarao as formas por extenso do numero */   
   char numeroCardinal[ 250 ] = "\0", numeroOrdinal[ 250 ] = "\0";
   
   /* Verificando a quantidade de parametros passados */
   if ( argc < 2 ) {
      printf( "VALIDACAO : Falta um parametro.\n" );
      printf( "\tDigite \"validacao --ajuda\" para obter ajuda.\n" );
      exit( 1 );
   }
   
   if ( argc > 3 ) {
      printf( "VALIDACAO : Excesso de parametros\n" );
      printf( "\tDigite \"validacao --ajuda\" para obter ajuda.\n" );
      exit( 1 );
   }
   
   /* O usuario entrou com um parametro especial */
   if ( argc == 2 ) {
      
      /* Em caso de o usuario querer ler a ajuda do programa. */
      if ( ( ! strcmp( *( argv + 1 ), "--ajuda" ) ) || 
         ( ! strcmp( *( argv + 1 ), "-a" ) ) ) {
         printf( "Uso: validacao [opcoes] NUMERO\n" );
         printf( "\tonde: NUMERO eh um numero de CNPJ ou CPF. Lembre-\n" );
	 printf( "\tse de que NUMERO deve conter APENAS digitos.\n" );
         printf( "ou   nw [opcoes]\n" );
         printf( "Verifica a validade de um numero de CNPJ ou CPF.\n" );
	 printf( "Opcoes:\n" );
         printf( "\t--cnpj, -n      Verifica a validade de um numero de CNPJ.\n" );
         printf( "\t--cpf, -p       Verifica a validade de um numero de CPF.\n" );
	 printf( "\t--ajuda, -a     Mostra esta ajuda e sai.\n" );
         printf( "\t--versao, -v    Mostra as notas da versao e sai.\n" );
	 printf( "Exemplos:\n" );
	 printf( "\tvalidacao --cpf 11111111111\n" );
	 printf( "\tvalidacao --cnpj 11111111111111\n" );
         
         exit( 0 );
      }
   
      /* Em caso de o usuario querer ler as notas da versao do programa. */
      else if ( ( ! strcmp( *( argv + 1 ), "--versao" ) ) || 
         ( ! strcmp( *( argv + 1 ), "-v" ) ) ) {
         printf( "\tValidacao 0.5\n" );
         printf( "Escrito por Jose Lopes de Oliveira Júnior. " );
         printf( "<jlojunior@gmail.com>\n\n" );
         printf( "Copyright (C) 2005.\n" );
         
         printf( "\tEste programa é um software de livre distribuição, que pode\n" ); 
         printf( "ser copiado e distribuído sob os termos da Licença Geral\n" );
         printf( "GNU, conforme publicada pela Free Software Foundation,\n" );
         printf( "versão 2 da licença, ou (a critério do autor) qualquer versão\n" );
         printf( "posterior.\n" );
         printf( "\tEste programa é distribuído na expectativa de ser útil aos\n" );
         printf( "seus usuários, porém NÃO POSSUI NENHUMA GARANTIA, EXPLÍCITA OU\n" );
         printf( "IMPLÍCITA, COMERCIAL OU DE ATENDIMENTO A UMA DETERMINADA\n" );
         printf( "FINALIDADE. Consulte a Licença Pública Geral GNU.\n" );
         
         exit( 0 );
      }
      
      /* Parametro invalido. Imprime-se mensagem de erro. */
      else {
         printf( "VALIDACAO : Parametro invalido.\n" );
         printf( "\tDigite \"validacao --ajuda\" para obter ajuda.\n" );
	 
         exit( 1 );
      }
   }
   
   else if ( argc == 3 ) {
      /* Verificando o terceiro parametro para saber se o usu-*/
      /* ario digitou corretamente o numero so CNF ou CNPJ.   */
      if ( ! isNumero( *( argv + 2 ) ) ) {
         /* Numero invalido. */
         printf( "VALIDACAO : O terceiro parametro eh invalido.\n" );
         printf( "\tDigite \"validacao --ajuda\" para obter ajuda.\n" );
         
	 exit( 1 );
      }
      
      /* Verificando o segundo parametro para saber o que */
      /* o usuario deseja verificar : CPF ou CNPJ.        */
      /* Verificacao do CPF. */
      if ( ( ! strcmp( *( argv + 1 ) , "--cpf" ) ) || 
         ( ! strcmp( *( argv + 1 ) , "-p" ) ) ) {
         if ( strlen( *( argv + 2 ) ) != 11 ) {
            /* Numero invalido. */
            printf( "VALIDACAO : Numero de CPF invalido.\n" );
            printf( "\tDigite \"validacao --ajuda\" para obter ajuda.\n" );
            
	    exit( 1 );
         }
	 
	 if ( validaCPF( *( argv + 2 ) ) ) {
	    printf( "\tVALIDACAO : Numero de CPF valido. :)\n" );
	    
	    exit( 0 );
	 }
	 else {
	    printf( "\tVALIDACAO : Numero de CPF invalido. :(\n" );
	    
	    exit( 0 );
	 }
      }
      
      /* Verificacao do CNPJ. */
      else if ( ( ! strcmp( *( argv + 1 ) , "--cnpj" )  ) || 
         ( ! strcmp( *( argv + 1 ) , "-n" ) ) ) {
         if ( strlen( *( argv + 2 ) ) != 14 ) {
            /* Numero invalido. */
            printf( "VALIDACAO : Numero de CNPJ invalido.\n" );
            printf( "\tDigite \"validacao --ajuda\" para obter ajuda.\n" );
            
	    exit( 1 );
         }
	 
	 if ( validaCNPJ( *( argv + 2 ) ) ) {
	    printf( "\tVALIDACAO : Numero de CNPJ valido. :)\n" );
	    
	    exit( 0 );
	 }
	 else {
	    printf( "\tVALIDACAO : Numero de CNPJ invalido. :(\n" );
	    
	    exit( 0 );
	 }
      }
      
      /* Parametro invalido. Imprime-se mensagem de erro. */
      else {
         printf( "VALIDACAO : O segundo parametro eh invalido.\n" );
         printf( "\tDigite \"validacao --ajuda\" para obter ajuda.\n" );
	 
         exit( 1 );
      }
            
      /* Finalizando o programa. */
      exit( 0 );
   }
}
