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

#include "efone.h"
#define _FROM_MAIN_

void client_loop();
void server_loop();
void sighup_handler();

/* server state variable   0 - server ready for connections
                           1 - server busy serving a client
									2 - server was connected, is in speaking state, only quit allowed
									3 - server is shuting down. disconnect everything ASAP
									4 - server is shut down
									5 - server HAS connected another party
	used by server threads for synchronization */
int server_state;


/* udp server state variable	  0 - server ready for connections
										  1 - server busy serving a client
										  2 - server is disconnecting
										  3 - server has disconnected
	used by receive thread for synchronization */

int receive_state;

/* udp client state variable		0 - client does nothing
											1 - client is talking to the server
											2 - client is disconnecting
	used by send thread for synchronization */
int send_state;

/* global variable , a socket that is used by the server sub-trehad for
serving client */
int server_thread_socket;

/* global variable for holding client connection tcp socket */
int client_thread_socket_tcp;

/* global variable that holds current compression mode:
   0  -  no compression */

int client_compression_mode;

/* global variable that holds state of communication 
	 0 = server listens
	 1 = client listens 
	 2 = full duplex 
	 3 = disconnect asap 
*/

int transmission_direction;

/* global variables holding information about client and server transmission */

trans_info cl_trinfo;
trans_info se_trinfo;

/* global variables holding client's and server's second side address */

client_info local_info;
client_info remote_info;

/* table of algorithms , used by codec's and decodec'c */

algorithm_info alg_info[10];

/* globalne mutexy */
MUTEX(sound_mutex);					/* mutex dostepu do karty dzwiekowej */

/* socket tcp servera */
int mysock;

/* sound handle global variable */
int audiofd;

/* sound handle description:
										0 - closed
										O_WRONLY - write
										O_RDONLY - read
										O_RDWR - read/write

*/

int audiofd_state;

/* holds the transmission duplex 
										1 = full
										0 = half
*/

int duplex;

/* hold the direction of transmission 
										0 - sending
										1 - receiving
*/

int trnasmission_dir;


int main ()
{
   pthread_t t_server;

	populate_algorithm_info();			/* set up the algorithm table */
   
	server_state=0;						/* stan serwera tcp */
   receive_state=0;			/* stan serwera udp */
   send_state=0;			/* stan klienta udp*/
	audiofd_state=0;						/* state of the sound handle */
	duplex=1;								/* full duplex */
   
	pthread_create( &t_server, NULL, (void*) &server_loop,NULL);  /* start server thread */
	ignore_pipe();							/* block sigpipe */
   client_loop();                   /* proces macierzysty */
	pthread_kill(t_server,SIGHUP);  /* kill the server thread on exit */
   while(server_state !=4)
	{
	}
	exit(0);
}


/* glowna petla obslugujaca klienta */

void client_loop(void *arg)
{

   int a;

   client_compression_mode=0; /* set compression to none */

   printf("Efone client/server %s welcomes you.\n",EFONE_VERSION);

   a=GET_OK;
   while (a != GET_QUIT) 
   {
//      printf("efone> ");
      a=clinp();
      if (a==GET_UNKNOWN) printf ("Unknown command!\n");
      if (a==GET_TOOLONG) printf ("Command too long!\n");
   
	}
}

/* glona petla obslugujaca serwer */

void server_loop (void *arg)
{
   int clilen ,clisock;
   struct sockaddr_in   cli_addr;
   pthread_t t_sub_server;
   
	signal(SIGHUP,sighup_handler);		/* set up the sighup handler for closing tcp server */
	siginterrupt(SIGHUP,1);					/* tell blocking functions to terminate on sighup */

   mysock=create_server_tcp();

   for ( ; ; ) 
   {
   
      clilen=sizeof(cli_addr);
      clisock = accept (mysock, (struct sockaddr *) &cli_addr,&clilen);
      if (clisock <0)
      {
			if (errno != EINTR)
			{
				DEBUG(1, "Error accepting socket\n");
			}
			else
			{
				printf("Everything closed successfuly!\n");
			}
			break;
      }
      
      if (server_state==0) 
      {
         server_thread_socket=clisock;
         server_state=1;
			remote_info.address=cli_addr.sin_addr.s_addr;
			remote_info.port=MY_SERVER_UDP_PORT;
         pthread_create( &t_sub_server, NULL, (void*) &client_serve,NULL);
      }
		else 
      {
         WRITEN(clisock,"-ERR I'm busy right now, try later.\n");
         close(clisock);
      }
   }
	close(mysock);
	server_state=4;
}

void sighup_handler()
{
	server_state=3;
}

