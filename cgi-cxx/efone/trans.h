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

/* funkcja obslugujaca transmisje, wywolywana jako oddzielny watek */
void sound_transmit_thread(void *arg);
void sound_receive_thread(void *arg);

/* dodatkowe funkcje */
int data_checksum(char *sndbuff_dest,int encoded_len);

/* ALGORITHMS PROTOTYPES */
void populate_algorithm_info();
/* null codec */
void null_init();
void null_quit();
int null_encode(unsigned short *src,unsigned char *dest,int sndbuff_len);
void null_decode(unsigned char *src, unsigned short *dest,int sndbuff_len);

/* struktura opisujaca parametry polaczenia */

typedef struct clinfo {
	unsigned long int address;	/* adres odbiorcy */
	int port;						/* port odbiorcy */
} client_info;

typedef struct transinfo {
	int audiofd;  					/* deskryptor urzadzenia audio */
	int udpsocket;					/* deskryptor gniazdka udp */
	int algorithm;					/* oznaczenie algorytmu kompresji */
	client_info *clinfo;			/* wskaznik do opisu klienta */
} trans_info;

/* packet_type: 	0 - data packet
						1 - duplex switch
						2 - end of transmission
						3 - silence
	checksum:		a sum of all bytes in datafield
	datasize:		size of datafield in bytes
*/

typedef struct udppackethead {
	unsigned char packet_type;				/* this field determines packet type */
	unsigned char checksum;					/* a field for simple checksum of packet data */
	unsigned char data_size;				/* this field determines the size of data field */
	unsigned char packet_number;			/*	this is the sequence number of a packet */
} udp_packet_head;

typedef struct algorithminfo {
	int packet_size;				/* dlugosc bufora danych akceptowana przez algorytm */
	void *codec_init;				/* funkcja inicjacji kodeka */
	void *coded_quit;				/* funkcja wywolywana na zakonczenie pracy kodeka */
	void *encode;					/* compress buffer */
	void *decode;					/* decompress buffer */
} algorithm_info;


