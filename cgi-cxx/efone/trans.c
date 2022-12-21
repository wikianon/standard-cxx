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

void *receive_udp (void *arg)
{
	client_info clinf;
	bcopy (arg,&clinf,sizeof(client_info));
	
	return(NULL);
}

void *send_udp (void *arg)
{
	client_info clinf;
	bcopy (arg,&clinf,sizeof(client_info));

	return (NULL);
}

void sound_transmit_thread(void *arg)
{
	trans_info *trinfo=(trans_info *) arg;
	struct sockaddr_in serv_addr;
	unsigned short *sndbuff_src;
	char *sndbuff_dest;
	int sndbuff_len,encoded_len;
	udp_packet_head p_header;
	int (*encode)(unsigned short *src,unsigned char *dest,int sndbuff_len)=alg_info[trinfo->algorithm].encode;

	sndbuff_len=alg_info[trinfo->algorithm].packet_size*sizeof(unsigned short);
	sndbuff_src=malloc(sndbuff_len);
	sndbuff_dest=malloc(sndbuff_len);

	memset((char *) &serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_addr.s_addr  = trinfo->clinfo->address;
	serv_addr.sin_port         = htons(MY_SERVER_UDP_PORT);
	serv_addr.sin_family       = AF_INET;

	p_header.packet_number=0;														/* wyzerowanie numeru pakietu */

	while(1)
	{
		if (send_state == 1)
		{
			read_from_soundcard(trinfo->audiofd,sndbuff_src,sndbuff_len);						/* read sound */
			encoded_len=(*encode)(sndbuff_src,sndbuff_dest,sndbuff_len);
			p_header.packet_type=0;														/* data packet */
			p_header.data_size=encoded_len;											/* data part lenght */
			p_header.checksum=data_checksum(sndbuff_dest,encoded_len);		/* checksum of a packet */
			sendto(trinfo->udpsocket, &p_header, sizeof(udp_packet_head), 0,(struct sockaddr *) &serv_addr, sizeof(struct sockaddr));
			sendto(trinfo->udpsocket, sndbuff_dest, sndbuff_len, 0,(struct sockaddr *) &serv_addr, sizeof(struct sockaddr));
			p_header.packet_number++;
		}
		else if(send_state == 2)
		{
			p_header.packet_type=2;														/* data packet */
			p_header.data_size=0;														/* data part lenght */
			p_header.checksum=0;															/* checksum of a packet */
			sendto(trinfo->udpsocket, &p_header, sizeof(udp_packet_head), 0,(struct sockaddr *) &serv_addr, sizeof(struct sockaddr));
			send_state=3;
			break;
		}
	}
	close(trinfo->udpsocket);
	free(sndbuff_src);
	free(sndbuff_dest);
}

void sound_receive_thread(void *arg)
{
	trans_info *trinfo=(trans_info *) arg;
	unsigned short *sndbuff_src;
	char *sndbuff_dest;
	int sndbuff_len,a;
	udp_packet_head p_header;
	void (*decode)(unsigned short *src,unsigned char *dest,int sndbuff_len)=alg_info[trinfo->algorithm].encode;
	char bufik[1024];

	sndbuff_len=alg_info[trinfo->algorithm].packet_size*sizeof(unsigned short);
	sndbuff_src=malloc(sndbuff_len);
	sndbuff_dest=malloc(sndbuff_len);

	while(1)
	{
		if (receive_state==1)
		{
			recv(trinfo->udpsocket,&p_header,sizeof(udp_packet_head),0);
			if (p_header.packet_type == 0)
			{
				recv(trinfo->udpsocket,sndbuff_src,p_header.data_size,0);
				(*decode)(sndbuff_src,sndbuff_dest,p_header.data_size);
				write_to_soundcard(trinfo->audiofd, (signed short *) sndbuff_dest, p_header.data_size);
			}
			if (p_header.packet_type == 2)
			{
				send_state=2;
				while(send_state==2);
				{
				}
				send_state=0;
				if(server_state==5)
				{
					WRITEN(client_thread_socket_tcp,"QUIT\n");
					a=nreadline (client_thread_socket_tcp, bufik, 1024);
					close(client_thread_socket_tcp);
				}
				printf("\nYour party has closed connection. Press <Enter> to continue.\n");
				close_soundcard(audiofd);
				server_state=0;
				break;
				receive_state=2;
			}
		}
		else if(receive_state == 2)
		{
			break;
		}
	}
	close(trinfo->udpsocket);
	free(sndbuff_src);
	free(sndbuff_dest);
	receive_state=0;
}

void populate_algorithm_info()
{
/* algorithm 0 - raw unsigned short samples */	
	alg_info[0].codec_init=null_init;
	alg_info[0].coded_quit=null_quit;
	alg_info[0].decode=null_decode;
	alg_info[0].encode=null_encode;
	alg_info[0].packet_size=127;
}

int data_checksum(char *sndbuff_dest,int encoded_len)
{
	return(0);
}

/* function for algorithm 0 - raw */

void null_init()
{
}

void null_quit()
{
}

int null_encode(unsigned short *src,unsigned char *dest,int sndbuff_len)
{
	bcopy(src, dest, sndbuff_len);
	return(sndbuff_len);
}

void null_decode(unsigned char *src, unsigned short *dest,int sndbuff_len)
{
	bcopy(src, dest, sndbuff_len);
}

