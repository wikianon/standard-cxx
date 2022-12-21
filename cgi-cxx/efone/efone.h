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

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <pthread.h>
#include <netdb.h>
#include <sys/soundcard.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <readline/readline.h>
#include <readline/history.h>
		  
/* include all headers from program components */

#include "server.h"
#include "nethelp.h"
#include "client.h"
#include "trans.h"
#include "soundcard.h"

#define _DEBUGME_

#define EFONE_VERSION "0.01"


/* Poprty sieciowe itp.*/

#define MY_SERVER_TCP_PORT 1975                                    
#define MY_SERVER_UDP_PORT 1975
#define MY_CLIENT_UDP_PORT 1976  /* it could be the same but this way it helps testing
                                    efone with connection to itself */

/* Dodatkowe definicje pomocnicze */

#define DEBUG(a,b) fprintf( stderr, "%i %s\n", a, b) ; perror(" a ");
#define DETACH pthread_detach(pthread_self())

/* some defines for managing mutexes */
#define LOCK(a) pthread_mutex_lock ( &a )
#define UNLOCK(a) pthread_mutex_unlock (&a)
#define MUTEX(a) pthread_mutex_t a = PTHREAD_MUTEX_INITIALIZER;
#define EMUTEX(a) extern pthread_mutex_t a

#ifndef _FROM_MAIN_
/* Zmienne globalne z roznych plikow */
extern int server_state;
extern int receive_state;
extern int send_state;
extern int server_thread_socket;
extern int client_thread_socket_tcp;
extern int client_compression_mode;
extern int transmission_direction;
extern client_info local_info;
extern client_info remote_info;
extern trans_info cl_trinfo;
extern trans_info se_trinfo;
extern algorithm_info alg_info[10];
extern int audiofd;
extern int audiofd_state;
extern int duplex;
extern int transmission_dir;
EMUTEX(sound_mutex);
#endif

/* global variables holding client's and server's second side address */

client_info clinfo_cl;
client_info clinfo_se;


