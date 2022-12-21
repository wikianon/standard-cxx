#include <stdio.h>

/*
int main(void)
{
 int vetor[36]= {1,2,3,4,5,6,7,8,9,10};
 int soma=1;

 for(int i = 1; i < 10; ++i)
 soma += vetor[i];
    
 printf("\n\tSoma: %d\n\n", soma);
 
 return 0;
}
*/

int main()
{
 int soma = 0;
 
 for(int i = 1; i <= 10; i++)soma += i;                 
         
 printf("\n\tSoma: %d\n\n",soma);
 
 return 0;
}