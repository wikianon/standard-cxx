#include <stdio.h>
#include <string.h>
//Como apagar um caracter dentro de uma string?
//https://pt.stackoverflow.com/questions/32355/como-apagar-um-caracter-dentro-de-uma-string
int main(void)
{
 char str1[35], str2[35];
    
 printf("Digite uma frase de ate 35 letras: ");
 fgets(str1, 35, stdin);
    
 printf("Digite outra frase de ate 35 letras: ");
 fgets(str2, 35, stdin);
    
 int size_str1 = strlen(str1)-1;
 int size_str2 = strlen(str2)-1;
    
    for (int i = 0; i < size_str1; i++)
     {
      for (int j = 0; j < size_str2; j++)
      {
       if (str1[i] == str2[j])
        {
         for (int k = i; k < size_str1; k++)
          {
           str1[k] = str1[k + 1];
          }
        }
      }
     }
     
 puts(str1);
} 
