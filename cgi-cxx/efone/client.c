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

int cl_connect (char *buffer)
{
	char bufik[1024];
	char *buff;
	char *strtok_holder;
	pthread_t t_client_transmit,t_server_transmit;
	int a;

	if (send_state != 0)
	{
		printf("I'm already connected. Hangup first!\n");
		return(GET_OK);
	}

	buff=strtok_r(buffer, " ",&strtok_holder);
	buff=strtok_r(NULL, " ",&strtok_holder);
	printf("Connecting to %s\n",buff);
	
	remote_info.address=ascii_to_addr(buff);
	remote_info.port=MY_SERVER_UDP_PORT;

	if ((client_thread_socket_tcp=create_client_tcp(buff)) < 0)
	{
		printf("Error while connecting.\n");
		return(GET_OK);
	}
	printf("Connected successfuly.\n");

	a=nreadline (client_thread_socket_tcp, bufik, 1024); /* lets eat servers initial */
	WRITEN(client_thread_socket_tcp,"HELO elysium.saska\n"); /* response */
	a=nreadline (client_thread_socket_tcp, bufik, 1024);
	if ((a=strncmp(bufik,"+OK",3)) != 0)
	{
		printf("Error while setting up transmission.\n");
		close(client_thread_socket_tcp);
		return(GET_OK);
	}
	a=nreadline (client_thread_socket_tcp, bufik, 1024);

	memset(bufik,0,1024);
	snprintf(bufik,1024,"PARM %i\n",client_compression_mode);
	WRITEN(client_thread_socket_tcp,bufik);
	
	a=nreadline (client_thread_socket_tcp, bufik, 1024);
		if ((a=strncmp(bufik,"+OK",3)) != 0)
	{
		printf("Error while greeting.\n");
		close(client_thread_socket_tcp);
		return(GET_OK);
	}
   
	cl_trinfo.udpsocket=create_socket_udp(MY_CLIENT_UDP_PORT);
	cl_trinfo.audiofd=open_soundcard(AUDIO_DEV,O_RDONLY);
	cl_trinfo.clinfo=&remote_info;
	cl_trinfo.algorithm=client_compression_mode;

	pthread_create( &t_client_transmit, NULL, (void*) &sound_transmit_thread,&cl_trinfo);
	send_state=1;

	se_trinfo.udpsocket=create_socket_udp(MY_SERVER_UDP_PORT);
	se_trinfo.audiofd=open_soundcard(AUDIO_DEV,O_WRONLY);
	se_trinfo.clinfo=NULL; /* brak danych o kliencie */
   se_trinfo.algorithm=client_compression_mode;

	pthread_create( &t_server_transmit, NULL, (void*) &sound_receive_thread,&se_trinfo);
	receive_state=1;
	server_state=5;

	return(GET_OK);
}

int cl_hangup (char *buffer)
{
	char bufik[1024];
	int a;

   if (server_state != 2 && server_state !=5)
	{
		printf("We are not connected. Connect first!\n");
		return(GET_OK);
	}

	send_state=2;
	while(send_state==2);
	{
	}
	send_state=0;
	receive_state=2;
	while(receive_state==2);
	{
	}
	if (server_state==5)
	{
		WRITEN(client_thread_socket_tcp,"QUIT\n");
		a=nreadline (client_thread_socket_tcp, bufik, 1024);
		close(client_thread_socket_tcp);
	}
   
	printf("Connection closed\n");
	close_soundcard(audiofd);
	server_state=0;
	return(GET_OK);
}



int cl_quit (char *buffer)
{
	if (server_state != 0)
	{
		printf("We are connected. Hangup first!\n");
		return(GET_OK);
	}
	return(GET_QUIT);
}

int cl_compress (char *buffer)
{
	return(GET_OK);
}

int cl_mode (char *buffer)
{
	return(GET_OK);
}

int cl_status (char *buffer)
{
	printf("Current compression algorithm: %i\n",client_compression_mode);
	printf("Current transmission mode: %i\n",transmission_direction);
	printf("Current server state: %i\n",server_state);
	printf("Current server receive state: %i\n",receive_state);
	printf("Current client send state: %i\n",send_state);
	return(GET_OK);
}

int cl_help (char *buffer)
{
	printf("List of available commands:\n");
	printf("connect <hostname> - connects to the remote host.\n");
	printf("hangup - breaks current connection.\n");
	printf("compress <algorithm> - sets transmission algorithm.\n");
	printf("mode <mode> - sets transmission mode.\n");	
	printf("help - prints this help screen.\n");
	printf("quit - exits efone.\n");
	return(GET_OK);
}


int clinp ()
{
//	char buffer[512];
	char temp_buffer[512];
	char *buff,*buffer;
	int   a,n;
	char *strtok_holder;

//	buff=fgets (buffer, sizeof(buffer),stdin);
	
	buffer=readline("efone>");
	if (buffer!=NULL)
	{

		n=strlen(buffer);
		if (n>512)
		{
			a=GET_TOOLONG;
		}
		if (n!=0)
		{
			add_history(buffer);
			bcopy(buffer,temp_buffer,n);
			buff=strtok_r(temp_buffer, " ",&strtok_holder);
			for (a=0;a<strlen(buff);a++)
			{
				buff[a]=tolower(buff[a]);
			}

			a=GET_UNKNOWN;
			if (strncmp(CL_CONNECT,buff,strlen(CL_CONNECT)) == 0)	a=cl_connect(buffer);
			else if (strncmp(CL_HANGUP,buff,strlen(CL_HANGUP)) == 0)	a=cl_hangup(buffer);
			else if (strncmp(CL_QUIT,buff,strlen(CL_QUIT)) == 0) a=cl_quit(buffer);
			else if (strncmp(CL_COMPRESS,buff,strlen(CL_COMPRESS)) == 0) a=cl_compress(buffer);
			else if (strncmp(CL_HELP,buff,strlen(CL_HELP)) == 0) a=cl_help(buffer);
			else if (strncmp(CL_MODE,buff,strlen(CL_MODE)) == 0) a=cl_mode(buffer);
			else if (strncmp(CL_STATUS,buff,strlen(CL_STATUS)) == 0) a=cl_status(buffer);
		}
		else
		{
			a=GET_NULL;
		}
	free(buffer);
	}
	else
	{
		a=GET_NULL;
	}
	return(a);
}

