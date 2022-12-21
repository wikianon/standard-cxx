#include<stdlib.h>
#include<stdio.h>
/*
int main(void)
{
    int a = 0;

    printf("IMPRIMINDO TABELA ASCII");

    for ( a = 1; a < 255; a++ )
    {
        printf("Decimal: %i\tOctal: %o\tHexadecimal: %x\t\t", a, a, a);

        if (a > 32)
            printf("Caractere: %c",a);

        printf("\n");
    }

    //Obs: %i imprime iteiro, %o imprime octal, %x imprime Hexadecimal, %c imprime caractere char
    return 0;
} 
*/
int main()
{
    char caracter = '\0';
    char temp = '\0';
    int binario[8] = {0};
    int controle = 0;
    int contador = 0;
    printf("\n\tDigite um numero: ");
    fscanf(stdin, " %c", &caracter);

    temp = caracter;

    /* Conversão do numero decimal em binário - INICIO */
    while ( temp > 1 ) 
    {
        binario[controle++] = temp % 2;
        temp = temp / 2;
    }
    binario[controle] = temp % 2;
	/* Conversão do numero decimal em binário - FIM */

    fprintf(stdout, "\n\tBinario: ");
    for ( contador = 7; contador >= 0; contador-- ) /* Apresenta o valor em binario na tela */
        fprintf(stdout, "%d", binario[contador]);

    fprintf(stdout, " - Decimal: %d - Hexadecimal: %x - Octal: %o - Caracter: %c\n\n", caracter, caracter, caracter, caracter);
}