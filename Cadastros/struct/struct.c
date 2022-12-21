#include <stdio.h>
 
typedef struct animal TAnimal;
 
typedef struct jogador 
{
 char nome[50];
 TAnimal* animal;
}TJogador;
 
struct animal
{
 char nome[50];  
 TJogador* jogador;
};
 
int main(void)
{
 TJogador jogador = { .nome = "abc" };
 TAnimal animal = { .nome = "hipopo", .jogador = &jogador };
 jogador.animal = &animal;
 
 printf("\n\t%s\n\n",jogador.animal);
}