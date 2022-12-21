#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
/*
 algoritmo que faz a leitura de um arquivo txt e retorna todo o seu conteúdo, 
 contando os caracteres das palavras e retornar 
 cada palavra e a quantidade de caracteres cada uma.  
*/
 
int cntchr()
{
 FILE *fp;
  char line[9000];
   size_t line_len;
    char *word;
     char lorem[9000] = "\n\tLorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod\n\ttempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam,\n\tquis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo\n\tconsequat. Duis aute irure dolor in reprehenderit in voluptate velit esse\n\tcillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non\n\tproident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
    //char lorem[] = "Jean César Fernandes Zonta";
    
 fp = fopen("cntchrs.txt", "w");
  fwrite(lorem, sizeof(char), sizeof(lorem) - 1, fp);
   fclose(fp);  
    fp = fopen("cntchrs.txt", "r");
    
    if (fp == NULL)
    return 1;
    
    while (fgets(line, sizeof(line), fp) != NULL) 
    {
        line_len = strlen(line);
        if (line_len > 0 && line[line_len-1] == '\n') 
        {
         line[--line_len] = '\0';
        }
        word = strtok(line, " ,.\n\t");
        while (word != NULL) 
        {
         printf("%s: %zu\n", word, strlen(word));
         word = strtok(NULL, " ,.\n\t");
        }
    }
    
  fclose(fp);
 return 0;
}

int cntchrs(char *input)
{
    FILE *fp;
    char line[9000];
    size_t line_len;
    char *word;
    char lorem[9000];
    
    strcpy(lorem,input);
    //sprintf(lorem,"%s",input);
    
    fp = fopen("filecntchrs.txt", "w");
    fwrite(lorem, sizeof(char), sizeof(lorem)-1, fp);
    fclose(fp);
    
    fp = fopen("filecntchrs.txt", "r");
    if (fp == NULL)
        return 1;
    
    while (fgets(line, sizeof(line), fp) != NULL) 
    {
        line_len = strlen(line);
        if (line_len > 0 && line[line_len-1] == '\n') 
        {
         line[--line_len] = '\0';
        }
        
        word = strtok(line, " ,.\n\t");
        while (word != NULL) 
        {
         printf("%s: %zu\n", word, strlen(word));
         word = strtok(NULL, " ,.\n\t");
        }
    }

   fclose(fp);
   /*O arquivo filecntchrs.txt foi aberto com a codificação UTF-8, mas continha caracteres inválidos.
Se for configurado como somente leitura, salvá-lo poderá destruir seu conteúdo.
Reabra o arquivo com a codificação correta escolhida ou habilite o modo de leitura-escrita novamente através menu para ser capaz de editá-lo.
*/
}

int main()
{
 char lorem[9000] = "\n\tLorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod\n\ttempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam,\n\tquis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo\n\tconsequat. Duis aute irure dolor in reprehenderit in voluptate velit esse\n\tcillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non\n\tproident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
 
 //cntchr();
 cntchrs(lorem);
 
 return 0; 
}