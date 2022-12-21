/* rename example */
#include <stdio.h>

int main()
{
 char oldname[64]={0};
 char newname[64]={0}; 
 
 printf("\n\tInput name file: ");
 scanf("%s", oldname);
 
 printf("\n\tOutput new name: ");
 scanf("%s", newname);
 
 if(rename(oldname, newname) == 0)
  puts("\n\tFile successfully renamed");
 else
  perror("Error renaming");
 return 0;
}