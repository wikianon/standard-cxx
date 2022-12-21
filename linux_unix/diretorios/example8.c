#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
 const int layer1 = 5;
 const int layer2 = 10;
 const int outfiles = 100;
 
 char buffer[100];
 int i = 0;
 
 for(; i < layer1; ++i)
  {
    sprintf(buffer, "mkdir folder%d", i);
    system(buffer);
    
    int j = 0;
    for(; j < layer2; ++j)
     {
       sprintf(buffer, "mkdir folder%d/xyz%d", i, j);
       system(buffer);
       int k = 0;
       
       for(; k < outfiles; ++k)
        {
         sprintf(buffer, "folder%d/xyz%d/fichier%d.xml", i, j, k);
         FILE *out = fopen(buffer, "w");
         fprintf(out, "<xml>\n  <i>%d</i>\n  <j>%d</j>\n  <k>%d</k>\n</xml>", i, j, k);
         fclose(out);
        }
     }
  }
 return EXIT_SUCCESS;
}