/*
 * resolver.c: resolve endereços para nome e vice-versa
 *
 * Para compilar:
 * cc -Wall resolver.c -o resolver
 *
 * Alexandre Fiori
 */
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//necessita ao menos um argumento na linha de comando
int main(int argc, char**argv)
{
 struct hostent* he;
 struct in_addr  ip;
 
 if(argc != 2)
  {
   fprintf(stderr,
           "use: %s addr\n"
           "exemplo: %s google.com\n",
           *argv,*argv
          );
 return 1;
}

 /* argv[1] pode ser um nome como ‘google.com’ ou um número,
  * como ’192.168.0.7’;
  * para sabermos qual operação utilizar, basta tentar transformar
  * o conteúdo de argv[1] utilizando inet_aton();
  */

 if(inet_aton(argv[1], &ip))
  {
   //caso seja um endereço IP, devemos solicitar o DNS reverso
   he = gethostbyaddr(&ip, sizeof(ip), AF_INET);
   
   if(!he)
    {
     herror("gethostbyaddr");
     return 1;
    }
   }
   else
   {
    //caso seja um nome, devemos resolver para o IP
    he = gethostbyname(argv[1]);
    
    if(!he)
     {
      herror("gethostbyname");
      return 1;
     }
   }
   
 //imprime todos os endereços retornados
 fprintf(stdout, "host: %s\n", he->h_name);
 while(*he->h_addr_list)
  {
   struct in_addr* in = (struct in_addr* )*he->h_addr_list;
   
   fprintf(stderr, "%s\n", inet_ntoa(*in));
   he->h_addr_list++;
  }
 return 0;
}
/*
Executando:
bash-4.4$ ./resolver google.com
host: google.com
64.233.187.99
72.14.207.99

bash-4.4$ ./resolver www.uol.com.br
host: www.uol.com.br
200.221.2.45

bash-4.4$ ./resolver 200.221.2.45
host: home.uol.com.br
200.221.2.45 
*/