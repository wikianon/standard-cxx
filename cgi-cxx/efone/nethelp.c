/*
 * efone - Distributed internet phone system.
 *
 * (c) 1999,2000 Krzysztof Dabrowski
 * (c) 1999,2000 ElysiuM deeZine
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 *
 */

/* Network communication helper functions*/
#include "efone.h"

/* Main function. creates a tcp socket accepting connections */

int create_server_tcp()
{
   int mysock;
   struct sockaddr_in serv_addr;

   if ((   mysock=socket(AF_INET,SOCK_STREAM, 0)) < 0)
   {
      DEBUG (1, "Nie moge utworzyc gniazdka");
      exit(-1);
   }


   memset((char *) &serv_addr, 0, sizeof(serv_addr));
   serv_addr.sin_addr.s_addr  = htonl (INADDR_ANY);
   serv_addr.sin_port         = htons(MY_SERVER_TCP_PORT);
   serv_addr.sin_family       = AF_INET;


   if (bind(mysock,(struct sockaddr *) &serv_addr, sizeof(serv_addr)) <0)
   {
      DEBUG (1, "Nie moge podlaczyc sie do portu...");
      exit(-1);
   }

   listen (mysock,1);

   return (mysock);
}

/* funkcja tworzaca gniazdko klienta TCP */

int create_client_tcp(char *dest_host)
{
   int mysock;
   struct sockaddr_in serv_addr;
   unsigned long int adresik;

   if ((   mysock=socket(AF_INET,SOCK_STREAM, 0)) < 0)
   {
      DEBUG (1, "Nie moge utworzyc gniazdka");
      exit(-1);
   }

   adresik=ascii_to_addr(dest_host);

   memset((char *) &serv_addr, 0, sizeof(serv_addr));
   serv_addr.sin_family       = AF_INET;
   serv_addr.sin_addr.s_addr  = adresik; 
   serv_addr.sin_port         = htons(MY_SERVER_TCP_PORT);

   if (connect(mysock,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
   {
      DEBUG (1, "Nie moge polaczyc sie z serverem");
      return(-1);
   }
   else return (mysock);
}

/* 3 ponizsze funkcje sa troche zmodyfikowanymi wersjami funkcji z ksiazki 
W.R. Stevens'a "Programowanie zastosowan sieciowych w systemie Unix" */

/* Funkcja pobierajaca z deskrypotra n bajtow. Dostosowana do pracy z gniazdem
strumieniowym. Stosowac zamiast systemowej funkcji read */

int readn(int fd , char *ptr, int nbytes)
{
   int nleft, nread;

   nleft = nbytes;
   while (nleft > 0)
   {
      nread=read(fd,ptr,nleft);
      if (nread <0) return (nread); /* blad, wynik < 0 */
      else if (nread == 0) break; /* EOF */

      nleft -= nread;
      ptr += nread;
   }
   return (nbytes - nleft);       /* wynik >= 0 */
}

/* funkcja wysylajaca do deskrypotra n bajtow. Dostosowana do pracy z gniazdem
strumieniowym. Stosowac zamiast systemowej funkcji write */

int writen(int fd , char *ptr, int nbytes)
{
   int nleft, nwritten;

   nleft = nbytes;
   while (nleft > 0)
   {
      nwritten=write(fd,ptr,nleft);
      if (nwritten <=0) return (nwritten); /* blad, wynik < 0 */

      nleft -= nwritten;
      ptr += nwritten;
   }
   return (nbytes - nleft);
}

/* funkcja pobierajca z deskryptora 1 wiersz. odpowiednik systemowej funckji fgets
na koncu dopisuje znak pusty. zwraca liczbe wczytanych znakow, bez znaku pustego */

int nreadline (int fd, char *ptr, int maxlen)
{
   int n, rc;
   char c;

   for (n = 1; n < maxlen; n++)
   {
      if ( (rc = read(fd,&c,1)) == 1)
      {
         *ptr++ = c;
         if (c == '\n') break;
      }
      else if (rc == 0) 
      {
         if (n == 1) return (0); /* EOF , nic nie pobrano */
         else break;             /* EOF , cos jednak pobrano */
      }
      else return(-1);           /* blad */
   }

   *ptr = 0;
   return(n);
}

/* This ignores the SIGPIPE signal.  This is usually a good idea, since
   the default behaviour is to terminate the application.  SIGPIPE is
   sent when you try to write to an unconnected socket.  You should
   check your return codes to make sure you catch this error! */
void ignore_pipe(void)
{
  struct sigaction sig;

  sig.sa_handler = SIG_IGN;
  sig.sa_flags = 0;
  sigemptyset(&sig.sa_mask);
  sigaction(SIGPIPE,&sig,NULL);
}

/* Converts ascii text to in_addr struct.  -1 is returned if the address
   can not be found. Address can not contain the "\n" or anything except the
   actual hostname */
unsigned long int ascii_to_addr(char *address)
{
  struct hostent *host;
  struct in_addr saddr;

  /* First try it as aaa.bbb.ccc.ddd. */
  saddr.s_addr = inet_addr(address);
  if (saddr.s_addr != -1) {
    return saddr.s_addr;
  }
  host = gethostbyname(address);
  if (host != NULL) {
    return *host->h_addr_list[0];
  }
  return (-1);
}

/* Creates an UDP socket on user specified port */

int create_socket_udp(int port)
{
   int mysock;
   struct sockaddr_in serv_addr;
        
   if ( (mysock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
   {
      DEBUG(1,"server: can't open datagram socket");
      return(-1);
   }
        
   memset((char *) &serv_addr, 0, sizeof(serv_addr));
   serv_addr.sin_family      = AF_INET;
   serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
   serv_addr.sin_port        = htons(port);

   if (bind(mysock, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr)) < 0)
   {
      printf("port: %i",port);
		DEBUG(1,"server: can't bind local address");
      return(-1);
   }

   return(mysock);
}

