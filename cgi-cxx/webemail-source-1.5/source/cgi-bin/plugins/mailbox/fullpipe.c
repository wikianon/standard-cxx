#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "fullpipe.h"

int fullpipe_backhalf(char *path, int isarg, char *arg[], int
file_final[2]);

int fullpipe(char *path, int file_final[2])
{  
	return fullpipe_backhalf(path, 0, NULL, file_final);  
}
int fullpipe_arg(char *path, char *arg[], int file_final[2])
{  
	return fullpipe_backhalf(path, 1, arg, file_final);  
}

int fullpipe_backhalf(char *path, int isarg, char *arg[], int
file_final[2])  
//file is a pipe
{

	//file[0] = reading
	//file[1] = writing

	//stdin 0
	//stdout 1
	//stderr 2
	int file_send[2];
	int file_receive[2];
	int fork_result;
	//int file_final[2];

	if (pipe(file_send) != 0)
	{//pipe creation error
		return 1;
	}
	if (pipe(file_receive) != 0)
	{//pipe creation error
		return 1;
	}

	fork_result = fork();
	if (fork_result == (pid_t)-1)  //error in fork
	{
		return 2;
	}

	if (fork_result == (pid_t)0)  //child
	{
		close(0);  //close stdin
		dup(file_send[0]); //copy read end of pipe to stdin
		close(file_send[0]); //close read end
		close(file_send[1]); //close write end

		close(1);  //close stdout
		dup(file_receive[1]); //copy write end of pipe to stdout
		close(file_receive[1]); //close write end
		close(file_receive[0]); //close read end

		if (isarg == 0)
		{	if (execl(path, path, (char *)0)== -1)
			{return 3;}
		}
		else
		{
			if (execv(path, arg)==-1)
			{return 3;}
		}
	}
	else //parent
	{
		close(file_send[0]);  //close read end
		close(file_receive[1]); //close write end

		//setup one pipe array
		file_final[0] = file_receive[0];  //setup read
		file_final[1] = file_send[1];     //setup write
		return 0;
	}
	return 0;
}
