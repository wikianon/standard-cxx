/*
 * servres.c: recebe nome ou número da porta e protocolo e informa
 * todos os dados contidos na base, /etc/services.
 *
 * Para compilar:
 * cc -Wall connect.c -o connect
 *
 * Alexandre Fiori
 */
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//define o destino da conexão, porta 80 em localhost
#define DESTINO_ADDR "127.0.0.1"
#define DESTINO_PORT 80

int main()
{
 int fd;
 struct in_addr addr;
 struct sockaddr_in sock;
 struct protoent* proto = getprotobyname("tcp");
 
 //cria o socket
 fd = socket(AF_INET, SOCK_STREAM, proto->p_proto);
 if(fd == -1)
  {
   perror("socket");
   return 1;
  }
 //limpa as estruturas que serão utilizadas
 memset(&addr, 0, sizeof(addr));
 memset(&sock, 0, sizeof(sock));
 //converte determinado endereço para network byte order

 if(!inet_aton(DESTINO_ADDR, &addr))
  {
   fprintf(stderr, "Endereço inválido!\n");
   close(fd); /*é importante fechar o socket!*/
   return 1;
  }
  
 //preenche a estrutura sockaddr_in: onde vamos conectar
 sock.sin_family = AF_INET;
 sock.sin_addr   = addr;
 sock.sin_port   = htons(DESTINO_PORT);

 //conecta
 if(connect(fd, (struct sockaddr*) &sock, sizeof(sock)) == -1)
  {
   perror("connect");
   close(fd);
   return 1;
  }
 /* agora estamos conectados em DESTINO_ADDR, DESTINO_PORT utilizando
  * o protocolo TCP
  */
 
 fprintf(stdout, "Conectado a %s, %d...\n", DESTINO_ADDR, DESTINO_PORT);
 sleep(1);
 
 //desconecta
 shutdown(fd, 2);
 close(fd);
 
 return 0;
}