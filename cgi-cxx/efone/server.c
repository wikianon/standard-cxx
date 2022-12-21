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

/* all commands supported by server, some may be just stubs for now */

#include "efone.h"

int cmd_helo (char *buffer,int clisock)
{
   char *buff;
   char bufor[1024];
   char *strtok_holder;
      
   if (server_state != 1)
	{
		WRITEN(clisock,"-ERR Wrong state.\n");
		return(GET_OK);
	}

   buff=strtok_r(buffer, " ",&strtok_holder);
   buff=strtok_r(NULL, " ",&strtok_holder);
   memset (bufor,0,1024);
   snprintf(bufor,1024,"+OK Welcome my friend from %s\n",buff);
   WRITEN(clisock,bufor);
   return (GET_OK);
}

int cmd_help (char *buffer,int clisock)
{
   WRITEN(clisock,"Here is the list of available commands:\n");
   WRITEN(clisock,"HELO, NOOP, PARM, QUIT, HELP\n\n");
   return (GET_OK);
}

int cmd_quit (char *buffer,int clisock)
{
   WRITEN(clisock,"+OK It was a pleasure to speak with you. Bye.\n");
	return (GET_QUIT);
}

int cmd_noop (char *buffer,int clisock)
{
   return (GET_OK);
}

int cmd_parm (char *buffer,int clisock)
{
	char *buff;
	char bufor[1024];
	char *strtok_holder;
	pthread_t t_server_transmit,t_client_transmit;

	if (server_state != 1)
	{
		WRITEN(clisock,"-ERR Wrong state.\n");
		return(GET_OK);
	}

	buff=strtok_r(buffer, " ",&strtok_holder);
	buff=strtok_r(NULL, " ",&strtok_holder);
	se_trinfo.algorithm=atoi(buff);
	memset (bufor,0,1024);
	snprintf(bufor,1024,"+OK Starting transmission with compresion type %i\n",se_trinfo.algorithm);
	WRITEN(clisock,bufor);
   	
	se_trinfo.udpsocket=create_socket_udp(MY_SERVER_UDP_PORT);
	se_trinfo.audiofd=open_soundcard(AUDIO_DEV,O_WRONLY);
	se_trinfo.clinfo=NULL; /* brak danych o kliencie */

	pthread_create( &t_server_transmit, NULL, (void*) &sound_receive_thread,&se_trinfo);
	receive_state=1;
	server_state=2;

	cl_trinfo.udpsocket=create_socket_udp(MY_CLIENT_UDP_PORT);
	cl_trinfo.audiofd=open_soundcard(AUDIO_DEV,O_RDONLY);
	cl_trinfo.clinfo=&remote_info;
	cl_trinfo.algorithm=se_trinfo.algorithm;

	pthread_create( &t_client_transmit, NULL, (void*) &sound_transmit_thread,&cl_trinfo);
	send_state=1;

	return (GET_OK);
}

/* glowna funckja obslugujaca pobieranie parametrow */

void cmd_loop(int clisock)
{
   char bufor[1024];
   int a;

   memset (bufor,0,1024);
   snprintf(bufor,1024,"Efone server %s welcomes you.\n",EFONE_VERSION);
   WRITEN(clisock,bufor);

   a=GET_OK;
   while (a != GET_QUIT) 
   {
      a=getinp(clisock);
   }
}


/* funkcja pobierajaca komendy i wywolujaca odpowiednia funkcjie */

int getinp (int clisock)
{
   char buffer[512];
   char temp_buff[512];
   char *buff;
   int   a;
   char *strtok_holder;

   a=nreadline (clisock,buffer, sizeof(buffer));
   bcopy(buffer,temp_buff,512);

   buff=strtok_r(temp_buff, " ",&strtok_holder);
   for (a=0;a<strlen(buff);a++) {
      buff[a]=toupper(buff[a]);
   }

   a=GET_UNKNOWN;
   if (strncmp(MSG_HELO,buff,4) == 0) a=cmd_helo(buffer,clisock);
   if (strncmp(MSG_HELP,buff,4) == 0) a=cmd_help(buffer,clisock);
   if (strncmp(MSG_QUIT,buff,4) == 0) a=cmd_quit(buffer,clisock);
   if (strncmp(MSG_NOOP,buff,4) == 0) a=cmd_noop(buffer,clisock);
   if (strncmp(MSG_PARM,buff,4) == 0) a=cmd_parm(buffer,clisock);

   if (a==GET_UNKNOWN) {
   WRITEN(clisock,"Unknown command! \n");
   }
   return (a);
}

void client_serve ()
{
      DETACH;
      cmd_loop(server_thread_socket);

      close(server_thread_socket);
      server_state=0;   /* let the server accept new connections again */ 
}


