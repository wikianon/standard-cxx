#include <stdio.h>
#include <unistd.h>
#include <limits.h> //HOST_NAME_MAX

int main()
{
 char hostname[HOST_NAME_MAX + 1];
 
 hostname[HOST_NAME_MAX] = 0;
 
 if(gethostname(hostname, sizeof(hostname)-1) == 0)
 printf("\n\tHostname: %s\n\n", hostname);
 else
 perror("gethostname");
 
 return 0;
}
