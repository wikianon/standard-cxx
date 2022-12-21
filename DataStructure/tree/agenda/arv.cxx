#include "arv.hxx"
/*!
 *
 * @begin @section terms_of_use Terms of Use
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3.0 of the License, or (at your option) any later version.
 *
 * @end @section terms_of_use Terms of Use
 * 
 * @begin @section author Author
 * 
 * @file     arv.cxx
 * @version  0.3
 * @brief    Árvore binária.
 * @Utility  Agenda telefonica.
 * @Original_Consult https://www.vivaolinux.com.br/script/Agenda-feita-em-C-usando-arvore-binaria
 * @Autor: Marcos Augusto
 * @Email_Autor: marcosccp04@gmail.com
 * @Modified: Jean Zonta
 * @Copyright (C) 2017 Marcos Augusto, Jean Zonta.
 * 
 * @end @section author Author
 *
 */
void inserir(Arvore **raiz ,char string[])
{
 int telefone;
 if(*raiz == NULL)
 {
  *raiz = new Arvore();
  strcpy((*raiz)->Nome,string);
  getPhones(&telefone);
  (*raiz)->telefone = telefone;
  (*raiz)->esq = NULL;
  (*raiz)->dir = NULL;              
 }
 else
 if(strcasecmp((*raiz)->Nome,string) > 0)inserir(&(*raiz)->esq,string);                     
 else
 if(strcasecmp((*raiz)->Nome,string) < 0)inserir(&(*raiz)->dir,string);
 else
 if(strcmp((*raiz)->Nome,string) == 0)
 printf(
        "\n\t*---------------------------*"
        "\n\t| NAME ALREADY REGISTERED!! |"
        "\n\t*---------------------------*\n" 
       );
}
                                                               
void ordem(Arvore *raiz)
{
 if(raiz != NULL) 
 {
  ordem((raiz)->esq);
  printf("\n\tName: ");
  printf("%s",(raiz)->Nome);
  printf("\tPhone: ");
  printf("%d",(raiz)->telefone);
  ordem((raiz)->dir);
 }
}

void busca(Arvore *raiz,char string[])
{
 if(raiz != NULL)
  {
   if(strcasecmp((raiz)->Nome,string) > 0)busca((raiz)->esq,string);
   else
   if(strcasecmp((raiz)->Nome,string) < 0)busca((raiz)->dir,string);
   else
   if(strcmp((raiz)->Nome,string) == 0)
    {
     printf(
            "\n\t*----------------*"
            "\n\t| RECORD FOUND!! |"
            "\n\t*----------------*"    
            "\n\n\tName: "
           );
     printf("%s",(raiz)->Nome);
     printf("\tPhone: ");
     printf("%d",(raiz)->telefone);
    }
  }
  else
  printf(
         "\n\t*------------------*"
         "\n\t| NAME NOT FOUND!! |"
         "\n\t*------------------*\n"
        );
}

void alterar( Arvore **raiz, char *string)
{
  if((*raiz) != NULL)
  {
   if(strcasecmp((*raiz)->Nome,string) > 0)alterar(&(*raiz)->esq,string);
    else
     if(strcasecmp((*raiz)->Nome,string) < 0)alterar(&(*raiz)->dir,string);
    else
     if(strcmp((*raiz)->Nome,string) == 0)
      {
       int telefone;
       printf(
              "\n\t*----------------*"
              "\n\t| RECORD FOUND!! |"
              "\n\t*----------------*" 
              "\n\n\tName: "
             );
       printf("%s",(*raiz)->Nome);
       printf("\tPhone: ");
       scanf(" %d",&telefone);
       (*raiz)->telefone = telefone;
       printf(
              "\n\t*----------------*"
              "\n\t| CHANGED DATA!! |"
              "\n\t*----------------*"
             );
      }
    }
  else
  printf(
         "\n\t*------------------*"
         "\n\t| NAME NOT FOUND!! |"
         "\n\t*------------------*"
        );
}

void excluir(Arvore **raiz,char string[])
{
 Arvore **aux2, *aux3;     
 if(*raiz != NULL)
 {
  if(strcasecmp((*raiz)->Nome , string) == 0)
  {
   if((*raiz)->esq == (*raiz)->dir)
   {
    free(*raiz);
    *raiz = NULL;
   }
   else
   {
    if((*raiz)->esq != NULL)
    {
     aux2 = maior_esq(*raiz);
     aux3 = *aux2;
     (*aux2) = (*aux2)->esq;
    }
    else
    {
     aux2 = menor_dir(*raiz);
     aux3 = *aux2;
     (*aux2) = (*aux2)->dir;
    }
    strcpy((*raiz)->Nome, aux3->Nome);
    free(aux3);
    aux3 = NULL;
   }
  }
  else
  {
   if(strcasecmp(string,(*raiz)->Nome) < 0)
    excluir(&(*raiz)->esq,string);
   else
    excluir(&(*raiz)->dir,string);
  }
 }
 else
  printf(
         "\n\t*------------------*"
         "\n\t| NAME NOT FOUND!! |"
         "\n\t*------------------*\n"
        );
}

Arvore** maior_esq(Arvore *raiz)
{ 
 Arvore **aux = &(raiz);
 if((*aux)->esq != NULL)
 {
   aux = &(*aux)->esq;
   while( (*aux)->dir != NULL )aux = &(*aux)->dir;
 }
 return aux;
}

Arvore** menor_dir(Arvore *raiz)
{
 Arvore **aux = &(raiz);
  if((*aux)->dir != NULL)
  {
   aux = &(*aux)->dir;
   while((*aux)->esq != NULL)aux=&(*aux)->esq;
  }
 return aux;
}

void menu(int *num)
{
 printf(
        "\n\n\t     *-----------*"
        "\n\t     | PHONEBOOK |"   
        "\n\t|---------------------|"
        "\n\t| 1 - Insert Contacts |"
        "\n\t|---------------------|"
        "\n\t| 2 - Print Contacts  |"
        "\n\t|---------------------|"
        "\n\t| 3 - Search Contacts |"
        "\n\t|---------------------|"
        "\n\t| 4 - Remove Contact  |"
        "\n\t|---------------------|"
        "\n\t| 5 - Change Contact  |"
        "\n\t|---------------------|"
        "\n\t| 6 - Exit            |"
        "\n\t*---------------------*"
        "\n\tEnter: "
       );
 scanf(" %d",num);
 getchar();
}

void getPhones(int *telefone)
{
 printf("\n\tEnter phone number: ");
 scanf(" %d",telefone);
}

void getNames(char string[])
{
 printf("\n\tEnter the name: ");
 scanf(" %[^\n]s",string); // Lê String até encontrar o ENTER.
 maiuscula(string);
}

void maiuscula(char string[])
{
 int i;
 for(i=0;i<strlen(string);i++)
 {
  if((string[i]>='a') && (string[i]<='z'))
  {
   string[i]-=32;
  }
 }
}