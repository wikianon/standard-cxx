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

/* Naglowki pomocniczych funkcji sieciowych */

int readn(int fd , char *ptr, int nbytes);
int writen(int fd , char *ptr, int nbytes);
int nreadline (int fd, char *ptr, int maxlen);
int create_server_tcp() ;
int create_client_tcp(char *dest_host);
void ignore_pipe(void);
unsigned long int ascii_to_addr(char *address);
int create_socket_udp(int port);

