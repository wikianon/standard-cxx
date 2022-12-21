#include "efone.h"

extern FILE *stdout;

int audiofd, audiofd_state, duplex;

/* let's open a sound device in specified mode. returns -1 if failed. */
/* modes allowed: O_WRONLY, O_RDONLY or O_RDWR */

int open_soundcard(char *devname,int mode)
{
	int param;

	if (audiofd_state == 0)
	{
		if (duplex==1)
		{
			mode=O_RDWR;
		}
		if ((audiofd = open(devname, mode, 0)) == -1)  
		{ 
			DEBUG(1,"Can not open audio device!\n");  /* blad otwarcia */
			return(-1);
		} 

		/* set the sound buffer lenght */
		  
		param=AUDIO_BUFF_SIZE;
      
		if (ioctl(audiofd, SNDCTL_DSP_GETBLKSIZE, &param) == -1) 
		{ 
			DEBUG(1,"I can not set fragment size on this device\n");  
			return(-1);  
		}  

		/* set the sample type to 16 bit little endian (standard in pc sound
		cards */

		param = AFMT_S16_LE;  
		if (ioctl(audiofd, SNDCTL_DSP_SETFMT, &param)==-1)  
		{ 
			DEBUG(1,"I can not sample type on this device\n");  
			return(-1);  
		}  

		if (param != AFMT_S16_LE)  
		{  
			DEBUG(1,"Can not set desired sample type on device\n");  
			return(-1);  
		} 

		/* set the card to mono mode */

		param = 0;
		if (ioctl(audiofd, SNDCTL_DSP_STEREO, &param)==-1)
		{ 
			DEBUG(1,"I can not set mono on this device\n");  
			return(-1);  
		}  
		if (param != 0)  
		{
			DEBUG(1,"This device doesn't support mono mode. Sheesh!\n");  
			return(-1);  
		} 

		/* set the sampling frequency */
		param = 44000;
		if (ioctl(audiofd, SNDCTL_DSP_SPEED, &param)==-1)  
		{ 
			DEBUG(1,"I can not set rate on this device\n");  
			return(-1);  
		}  
		if (param != 8000)  
		{  
			printf("Sampling speed set to %i",param);
		} 
		audiofd_state = mode;
		
		if(ioctl(audiofd,SNDCTL_DSP_GETCAPS,&param)==-1)
		{
			DEBUG(1,"I can not check capabilities of this device\n");  
			return(-1);  
		}
		if(!(param&DSP_CAP_DUPLEX))
		{
			DEBUG(1,"I need a full duplex card, sorry.\n");  
			return(-1);  
		}
	}
	else
	{
		if (audiofd_state==O_RDWR || audiofd_state==mode)
		{
			return(audiofd);
		}
		else
		{
			DEBUG(1, "Audiofd already opened in oposite direction.\n");  
			return(-1);
		}
	}
	return(audiofd);
}

/* this function reads from audio card on given descriptor */
	
int read_from_soundcard(int audiofd,signed short *audio_buffer, int count)
{
   int len;  
	
	if ((len = read(audiofd, audio_buffer, count)) == -1)  
	{  
		DEBUG(1, "Cannot read from audio device!\n");  
		return(-1);
	}
	return (len);
}

/* this function writes to audio card on given descriptor */

int write_to_soundcard(int audiofd,signed short *audio_buffer, int count)
{
   int len;  
	
	if ((len = write(audiofd, audio_buffer, count)) == -1)  
	{  
		DEBUG(1, "Cannot write to audio device!\n");  
		return(-1);
	}
	return (len);
}

int reset_soundcard(int audiofd)
{
	/* resetujemy driver */
	
	if (ioctl(audiofd, SNDCTL_DSP_RESET,0)==-1)  
	{ 
		DEBUG(1,"I can not use ioctl on this device\n");  
		return(-1);  
	}  
	return(1);
}

int close_soundcard(int audiofd)
{
	if (ioctl(audiofd, SNDCTL_DSP_SYNC, 0) < 0) 
	{
		DEBUG(1,"Can not flush device!\n");
		return(-1);
	}
	if (close(audiofd) < 0)
	{
		DEBUG(1,"Can not close sound device!\n");
		return(-1);
	}
   audiofd_state = 0;
	return(1);
}


int main ()
{
	int a;
	signed short audiobuff[512];

	audiofd_state=0;
	duplex=1;
	
	audiofd=open_soundcard("/dev/dsp",O_RDWR);
	
	for (a=0;a<512;a++)
	{
		read_from_soundcard(audiofd,audiobuff, sizeof(audiobuff));
		write(STDOUT_FILENO,audiobuff,sizeof(audiobuff)-1);
	}
}


