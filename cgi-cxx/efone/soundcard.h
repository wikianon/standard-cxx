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

/* few defines for OSS */

#define AUDIO_BUFF_SIZE    256
#define AUDIO_BUFF_SIZE_IN_HEX    0x7fff0008	/* daje to bufor o dlugosci 256 */
#define AUDIO_DEV	"/dev/dsp"

/* prototypes */

int open_soundcard(char *devname,int mode);
int close_soundcard(int audiofd);
int reset_soundcard(int audiofd);
int read_from_soundcard(int audiofd,signed short *audio_buffer, int count);
int write_to_soundcard(int audiofd,signed short *audio_buffer, int count);


