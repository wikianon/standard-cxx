#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char **argv)
{
 char *homedir = getenv("HOME");
 
 if(homedir != NULL)
  {
   printf("Home dir in enviroment %s\n", homedir);
   
   uid_t uid = getuid();
   struct passwd *pw = getpwuid(uid);
   
   if(pw == NULL)
    {
     printf("Failed\n");
     exit(EXIT_FAILURE);
    }
    
   printf("Home dir in enviroment %s\n", pw->pw_dir);
  }
 return 0;
}