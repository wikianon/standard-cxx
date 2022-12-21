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

/* Prototypes of all commands handlers */

/* powitanie format: HELO <my_hostname> */
int cmd_helo (char *buffer,int clisock);

/* wyswietlenie spisu komend serwera */
int cmd_help (char *buffer,int clisock);

/* zakonczenie polaczenia z serwerem */
int cmd_quit (char *buffer,int clisock);

/* nic nie rob. uzywane do podtrzymania razgawora */
int cmd_noop (char *buffer,int clisock);

/* przekazuje parametry polaczenia format: PARM <my_udp_port> <algorithm> */
int cmd_parm (char *buffer,int clisock);

/* protoypy funkcji */

int getinp (int clisock);
void cmd_loop(int clisock);
void client_serve ();

/* Teksty komend */

#define MSG_HELO "HELO"
#define MSG_HELP "HELP"
#define MSG_QUIT "QUIT"
#define MSG_NOOP "NOOP"
#define MSG_PARM "PARM"

/* Kody powrotu funkcji czytajacej komendy */

#define GET_UNKNOWN  1
#define GET_QUIT     2
#define GET_OK       3
#define GET_NULL		4
#define GET_TOOLONG	5

/* definicje pomocnicze */

#define WRITEN(a,b) writen(a,b,strlen(b));
#define PRBUF printf("%i %s",a,bufik)

