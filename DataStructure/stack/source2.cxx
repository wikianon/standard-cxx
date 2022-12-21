#include<iostream>
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
 * @file       source2
 * @version    0.1
 * @brief      Lista duplamente encadeada e ordenada.
 * @consult    estruturas de dados algoritmos, análise da complexidade e implementações em java e cc++ - ana fernanda gomes ascencio & graziela santos araújo
 * @author     Jean Zonta
 * @Copyright (C) 2013-2017 Jean Zonta.
 * 
 * @end @section author Author
 *
*/
struct lista
{ 
 int num;
  lista *prox;
 lista *ant;
};

int main()
{
 int esc;
 int qtd;
 lista *aux;
 lista *fim = NULL;
 lista *inicio = NULL;
 
 do{
    std::cout << "\n\n\tLISTA DUPLAMENTE ENCADEADA E ORDENADA\n"
                 "\n\tMENU DE ESCOLHA"
                 "\n\t[1]-INSERIR NA LISTA"
                 "\n\t[2]-CONSULTAR LISTA DO INICIO AO FIM"
                 "\n\t[3]-CONSULTAR LISTA DO FIM AO INICIO"
                 "\n\t[4]-REMOVER DA LISTA"
                 "\n\t[5]-ESVAZIAR A LISTA"
                 "\n\t[6]-SAIR"
                 "\n\tESCOLHA: ";
    std::cin >> esc;
    
    if(esc == 1)
     {
      std::cout << "\n\tQUANTOS NUMEROS DESEJA INSERIR?\n\tDIGITE: ";
      std::cin >> qtd;
       
      for(int i = 0;i < qtd; i++)
       {
        std::cout << "\n\tINSIRA NA LISTA O NUMERO " << i+1 << "º: ";
        lista *novo = new lista();
        std::cin >> novo->num;
          
        if(inicio == NULL)
         {
          novo->prox = NULL;
           novo->ant = NULL;
           inicio = novo;
          fim = novo;
         }
        else
         {
          aux = inicio;
          while(aux != NULL && novo->num > aux->num)aux = aux->prox;

          if(aux == inicio)
           {
            novo->prox = inicio;
             novo->ant = NULL;
             inicio->ant = novo;
            inicio = novo;
           }
          else
          if(aux == NULL)
           {
            fim->prox = novo;
             novo->ant = fim;
             fim = novo;
            fim->prox = NULL;
           }
          else
           {
            novo->prox = aux;
             aux->ant->prox = novo;
             novo->ant = aux->ant;
            aux->ant = novo;
           }
          }
         std::cout << "\n\tNUMERO " << novo->num << " INSERIDO NA LISTA COM SUCESSO..!!!\n";
        }
       }
       
    if(esc == 2)
     {
      if(inicio == NULL)std::cout << "\n\tLISTA VAZIA ..!!!\n";
      else
       {
        std::cout << "\n\tCONSULTANDO A LISTA DO INICIO PARA O FIM: ";
        aux = inicio;
        
        while(aux != NULL)
         {
          std::cout << aux->num << " ";
          aux = aux->prox;
         }
       }
     }
       
    if(esc == 3)
     {
      if(inicio == NULL)std::cout << "\n\tLISTA VAZIA ..!!!\n";
      else
       {
        std::cout << "\n\tCONSULTANDO A LISTA DO FIM PARA O INICIO: "; 
        aux = fim;

        while(aux != NULL)
         {
          std::cout << aux->num << " ";
          aux = aux->ant;
         }
       }
     }
       
    if(esc == 4)
     {
      int achou = 0; 
      int numero = 0; 
      if(inicio == NULL)std::cout << "\n\tLISTA VAZIA ..!!!\n";
      else
       {
        std::cout << "\n\tREMOVA UM ELEMENTO DA LISTA: ";
        std::cin >> numero;
         
        aux = inicio;
        while(aux != NULL)
         {
          if(aux->num == numero)
          {
           achou = achou + 1;
             
           if(aux == inicio)
            {
             inicio = aux->prox;
              if(inicio != NULL)
              inicio->ant = NULL;
              delete(aux);
             aux = inicio;
            }
           else
           if(aux == fim)
            {
             fim = fim->ant;
              fim->prox = NULL;
              delete(aux);
             aux = NULL;
            }
           else
            {
             aux->ant->prox = aux->prox;
              aux->prox->ant = aux->ant;
              lista *aux2;
              aux2 = aux->prox;
              delete(aux);
             aux = aux2;
            }
            }
           else(aux = aux->prox);
          }
          
         if(achou == 0)
         std::cout << "\n\tNUMERO " << numero << " NÃO ENCONTRADO ..!!!\n";
          else
         std::cout << "\n\tNUMERO: " << numero << " REMOVIDO " << achou << " VEZ..!!\n";
        }
       }
       
      if(esc == 5)
       {
        if(inicio == NULL)std::cout << "\n\tLISTA VAZIA ...!!!!\n";
         else
          {
           aux = inicio;
             while(aux != NULL)
              {
               inicio = inicio -> prox;
                delete(aux);
               aux = inicio;
              }    
            std::cout << "\n\tLISTA ESVAZIADA ..!!!\n";
          }
       }
       
       if(esc < 1 || esc > 6)
       std::cout << "\n\tOPÇÃO INVÁLIDA!!!";
        else
       if(esc == 6)  
       std::cout << "\n\tGOOD BYE ...!!\n\n";
      
   }while(esc != 6);
 return 0;
}