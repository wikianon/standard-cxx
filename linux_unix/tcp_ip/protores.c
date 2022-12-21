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

//necessita um argumento na linha de comando
int main(int argc, char**argv)
{
 struct protoent* pe;
 
 if(argc != 2)
  {
   fprintf(stderr,
           "use: %s [nome|número]\n"
           "exemplo: %s tcp\n"
           "exemplo: %s 1\n",
           *argv,*argv,*argv
          );
   return 1;
  }
 /* argv[1] pode conter um nome ou número do protocolo
  * para definir qual operação devemos utilizar é necessário isdigit()
  */
 
 if(isdigit(*argv[1]))
 //caso seja um número...
 pe = getprotobynumber(atoi(argv[1]));
 else
 //caso seja um nome...
 
 pe = getprotobyname(argv[1]);
 
 if(!pe)
  {
   fprintf(stderr, "protocolo não encontrado.\n");
   return 1;
  }
 //imprime o resultado
 fprintf(stdout, "protocolo %d (%s)\n",
 pe->p_proto, pe->p_name);
 return 0;
}
/*
Executando:
bash-4.4$ ./protores ip
protocolo 0 (ip)

bash-4.4$ ./protores tcp
protocolo 6 (tcp)

bash-4.4$ ./protores 17
protocolo 17 (udp)

bash-4.4$ ./protores 50
protocolo 50 (esp)

bash-4.4$ ./protores 19
protocolo não encontrado.
*/