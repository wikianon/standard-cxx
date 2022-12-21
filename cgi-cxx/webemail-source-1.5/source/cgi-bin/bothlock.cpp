#include "webemail.h"

#include <stdio.h>
#include <sys/file.h>
#include <unistd.h>
#include "bothlock.h"
#include <sys/types.h>
#include <pwd.h>

#ifdef OS_SunOS
 #include <fcntl.h>
#endif


//solaris does not support flock?? 
//or that is what it looks like
#ifndef OS_SunOS 
 int lock_flock(FILE *file);
 int unlock_flock(FILE *file);
#endif

int unlock_fcntl(FILE *file);
int lock_fcntl(FILE *file, char type);

int bothlock(FILE *file, char type)
{
#ifndef OS_SunOS 
	if (lock_flock(file))
		return 1;
#endif
	if (lock_fcntl(file, type))
		return 2;
	return 0;
}

int bothunlock(FILE *file)
{
	fsync(fileno(file));

#ifndef OS_SunOS 
	if (unlock_flock(file))
		return 1;
#endif
	if (unlock_fcntl(file))
		return 2;
	return 0;
}

int lock_fcntl(FILE *file, char type)
{
	struct flock lock;
	if (type == 'w')
		lock.l_type = F_WRLCK;
	else if (type == 'r')
		lock.l_type = F_RDLCK;
	else
		return 1;

	lock.l_whence = SEEK_SET;
	lock.l_start = lock.l_len=0;

	if (fcntl(fileno(file), F_SETLKW, &lock) < 0)
	{
		//perror("fcntl");
		//printf("Error using fcntl");
		return 1;
	}
	return 0;
}

int unlock_fcntl(FILE *file)
{
	struct flock lock;
	lock.l_type = F_UNLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = lock.l_len=0;

	if (fcntl(fileno(file), F_SETLKW, &lock) < 0)
	{
		//perror("fcntl");
		//printf("Error using fcntl");
		return 1;
	}
	return 0;
}

#ifndef OS_SunOS 
int lock_flock(FILE *file)
{
	if (flock(fileno(file), LOCK_EX)<0)
		return 1;
	return 0;
	
}
int unlock_flock(FILE *file)
{
	if (flock(fileno(file), LOCK_UN)<0)
		return 1;
	return 0;
}
#endif


FILE* processlock()
{
	passwd *info;
	char lockname[400];
	FILE *file;

	info=getpwnam(globaluser);
	snprintf(lockname, 400, "%s/.webmail/flock", info->pw_dir);

	file = fopen(lockname, "w");
	if (file == NULL)
	{
		error("processlock(): Can't open flock");
		return NULL;
	}
	bothlock(file, 'w');
	return file;
}
void processunlock(FILE * file)
{
	bothunlock(file);
	fclose(file);
	return;
}
