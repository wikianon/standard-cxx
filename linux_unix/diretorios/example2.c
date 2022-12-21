#include <stdio.h> // perror
#include <sys/stat.h> //mkdir

int main()
{
 char name[30]={0};
 
 printf("\n\tDigite o nome do diretorio que deseja criar: ");
 scanf("%s", name);
 
 if(mkdir(name,0777) == 0)
  printf("\n\tDirectory \"%s\" created successfully\n", name);
 else
  printf("\n\tDirectory \"%s\" not created\n", name);
 
 printf("\n\tDigite o nome do diretorio para remover: ");
 scanf("%s", name);
 
 if(remove(name) == 0)
  printf("\n\tDirectory \"%s\" deleted successfully\n", name);
 else
  printf("\n\tDirectory \"%s\" not deleted\n", name);
  
 return 0;
}