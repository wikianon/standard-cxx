#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
int main()
{
    char string1[] = "!Ntexto.txt#";
    char string2[] = "!FTexto de exemplo#";
    char *tmp;

    printf("Texto 1 - Segunda letra: %c\n", string1[1]); // Mostra apenas o segundo caracter
    printf("Texto 2 - Segunda letra: %c\n", string2[1]); // Mostra apenas o segundo caracter

    tmp = &string1[2]; // Aponta para a posicao dois da string
    tmp[strlen(tmp)-1] = '\0'; // retira o ultimo caracter
    printf("Texto 1 - Palavra: %s\n", tmp);

    tmp = &string2[2]; // Aponta para a posicao dois da string
    tmp[strlen(tmp)-6] = '\0'; // retira os 6 ultimos caracteres
    printf("Texto 2 - Palavra: %s\n", tmp);

    /*
  Output:
  Texto 1 - Segunda letra: N
  Texto 2 - Segunda letra: F
  Texto 1 - Palavra: texto.txt
  Texto 2 - Palavra: Texto de exemplo
    */
    return 0;
}

int main()
{
char c[] = "mafagafo";
char final[10];
int x = 3, i;

for (i = 0; i < sizeof(c); i++) {
    if (i < x) final[i] = c[i];
    else if (i > x) final[i-1] = c[i];
}

final[i] = 0;
//mafgafo
printf("%s\n\n", final);
 return 0;
}
*/