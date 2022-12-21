#include <stdio.h>
#include <stdlib.h>



int main(void)
{
 char * line = NULL;
 size_t len = 0;
 ssize_t read;
  
 while((read = getline(&line, &len, stdin)) != -1)
  {
    printf("Retrieved line of length %zu :\n", read);
    printf("%s", line);
  }
 
 if(line)
  free(line);
 
 return EXIT_SUCCESS;
}
 
