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

/* prototypy funkcji obslugujacych klienta */

/* prototypy handlerow komend */

/* funkcja nawiazujaca polaczenie format: connect <hostname> */
int cl_connect (char *buffer);
/* funkcja przerywajaca bierzace polaczenie */
int cl_hangup (char *buffer);
/* funkcja powodujaca wyjscie z programu */
int cl_quit (char *buffer);
/* funkcja ustalajaca algorytm kompresji.
   format: set <algorithm> */
int cl_compress (char *buffer);
/* funkcja ustaljaca tryb pracy
	format mode <mode>		*/
int cl_mode (char *buffer);
/* funkcja drukujaca spis dostepnych komend */
int cl_help (char *buffer);
/* funkcja drukujaca aktualny status */
int cl_status (char *buffer);

/* prototypy funkcji pomocniczych */

int clinp ();

/* Teksty komend */

#define CL_CONNECT "connect"
#define CL_HANGUP "hangup"
#define CL_QUIT "quit"
#define CL_COMPRESS "compress"
#define CL_MODE "mode"
#define CL_HELP "help"
#define CL_STATUS "status"

