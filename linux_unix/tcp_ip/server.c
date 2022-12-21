/*
 * servres.c: recebe nome ou número da porta e protocolo e informa
 * todos os dados contidos na base, /etc/services.
 *
 * Para compilar:
 * cc -Wall servres.c -o servres
 *
 * Alexandre Fiori
 */
#include <stdio.h>
#include <netdb.h>
#include <ctype.h>
#include <stdlib.h>

//necessita dois argumentos na linha de comando

int main(int argc, char**argv)
{
 struct servent* se;
 
 const char* proto = argv[2]; //opcional
 if(argc < 2)
  {
   fprintf(stderr,
           "use: %s porta [protocolo]\n"
           "exemplo: %s 80 tcp\n"
           "exemplo: %s smtp\n",
           *argv,*argv,*argv
          );
   return 1;
  }

 /* argv[1] pode conter um nome ou número de porta
  * para definir qual operação devemos utilizar é necessário isdigit()
  */

 if(isdigit(*argv[1]))
 //caso seja um número...
 se = getservbyport(htons(atoi(argv[1])), proto);
 else
 //caso seja um nome...
 se = getservbyname(argv[1], proto);
 
 if(!se)
  {
   fprintf(stderr, "serviço ou protocolo não encontrado.\n");
   return 1;
  }
  
 //imprime o resultado
 fprintf(stdout,
         "porta %d (%s), protocolo %s\n",
         ntohs(se->s_port), se->s_name, se->s_proto
        );
 return 0;
}
/*
Executando:
bash-4.4$ ./servres www
porta 80 (www), protocolo tcp

bash-4.4$./servres http
porta 80 (www), protocolo tcp

bash-4.4$ ./servres snmp
porta 161 (snmp), protocolo tcp

bash-4.4$ ./servres 53 udp
123
porta 53 (domain), protocolo udp

bash-4.4$ ./servres 110
porta 110 (pop3), protocolo tcp 
*/