#include <pwd.h>
#include <stdio.h>
#include <sys/types.h>
//Este programa pega id de um usuario logado
int main(int argc, char **argv)
{
 struct passwd *pw;
 int ch;
 extern int optind;

 while ((ch = getopt(argc, argv, "")) != EOF)
 
 switch (ch)
  {
   case '?':
   default:
    fprintf(stderr, "usage: pg_id [login]\n");
    exit(1);
  }
  
  argc -= optind;
  argv += optind;

  if(argc > 0)
   {
    if(argc > 1)
     {
      fprintf(stderr, "usage: pg_id [login]\n");
       exit(1);
     }
    if((pw = getpwnam(argv[0])) == NULL)
     {
      printf("NOUSER\n");
      exit(1);
     }
     printf("%d\n", pw->pw_uid);
   }
   else
   {
    printf("%d\n", getuid());
   }
 exit(0);
}