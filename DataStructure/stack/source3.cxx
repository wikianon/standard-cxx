#include <iostream>
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
 * @file       source4
 * @version    0.1
 * @brief      Lista Circular Simples Nao Ordenada.
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
};

int main()
{
 int esc;
 int qtd; 
 lista *aux;
 lista *ant;
 lista *fim = NULL;
 lista *inicio = NULL;

 do{
    std::cout << "\n\n\tLISTA CIRCULAR SIMPLES NÃO ORDENADA"
                 "\n\tMENU DE ESCOLHA"
                 "\n\t[1]-INSERIR NO INICIO"
                 "\n\t[2]-ISERIR NO FIM"
                 "\n\t[3]-CONSULTAR TUDO"
                 "\n\t[4]-REMOVER"
                 "\n\t[5]-ESVAZIAR"
                 "\n\t[6]-SAIR"
                 "\n\tESCOLHA: ";
    std::cin >> esc;
    
      if(esc == 1)
       {
        std::cout << "\n\tQUANTOS NUMEROS DESEJA INSERIR NO INICIO?\n\tDIGITE: ";
        std::cin >> qtd;
       
        for(int i = 0;i < qtd; i++)
         {
          std::cout << "\n\tINSIRA NO INICIO O NUMERO " << i+1 << "º: ";
          lista *novo = new lista();
          std::cin >> novo->num;

          if(inicio == NULL)
           {
            inicio = novo;
             fim = novo;
            fim->prox = inicio;
           }
          else
           {
            novo->prox = inicio;
             inicio = novo;
            fim->prox = inicio;
           }
          std::cout << "\n\tNUMERO " << novo->num << " INSERIDO COM SUCESSO!!!";
         }
       }
       
      if(esc == 2)
       {
        std::cout << "\n\tQUANTOS NUMEROS DESEJA INSERIR NO FIM?\n\tDIGITE: ";
        std::cin>>qtd;
       
        for(int i=0;i < qtd;i++)
         {
          std::cout << "\n\tINSIRA NO FIM O NUMERO " << i+1 << "º: ";
          lista *novo = new lista();
          std::cin >> novo->num;
        
          if(inicio == NULL)
           {
            inicio = novo;
             fim = novo;
            fim->prox = inicio;
           }
          else
           {
            fim->prox = novo;
             fim = novo;
            fim->prox = inicio;
           }
          std::cout << "\n\tNUMERO " << novo->num << " INSERIDO NO FIM COM SUCESSO!!!";
         }
       }
       
      if(esc == 3)
       {
        if(inicio == NULL)std::cout << "\n\tLISTA VAZIA!!!";
        else
         {
          std::cout << "\n\tLISTA COMPLETA: ";
          aux = inicio;
           do{
              std::cout << aux->num << " ";
              aux = aux->prox;
             }while(aux != inicio);
         }
       }
       
      if(esc == 4)
       {
        int achou = 0; 
        int numero = 0;
        int quantidade = 0;
        if(inicio == NULL)std::cout << "\n\tLISTA VAZIA!!!";
        else
         {
          std::cout << "\n\tREMOVA UM ELEMENTO: ";
          std::cin >> numero;
          aux = inicio;
          ant = NULL;
          aux = inicio;
          do{
             quantidade = quantidade + 1;
             aux = aux->prox;
            }while(aux != inicio);

          int elemento = 1;
          do{
             if(inicio == fim && inicio->num == numero)
             {
              delete(inicio);
              inicio = NULL;
              achou = achou + 1;
             }
            else
             {
              if(aux->num == numero)
              {
               achou = achou + 1;
               if(aux == inicio)
               {
                inicio = aux->prox;
                 fim->prox = inicio;
                 delete(aux);
                aux = inicio;
               }
               else
               if(aux == fim)
               {
                fim = ant;
                 fim->prox = inicio;
                 delete(aux);
                aux = NULL;
               }
               else
                {
                 ant->prox = aux->prox;
                  delete(aux);
                 aux = ant->prox;
                }
              }
             else
              {
               ant = aux;
               aux = aux->prox;
              }
             }
              elemento = elemento + 1;
           }while(elemento <= quantidade);
           
           if(achou == 0)
           std::cout << "\n\tNUMERO "<< numero << " NÃO ENCONTRADO!!!";
            else
           std::cout << "\n\tNUMERO "<< numero << " REMOVIDO " << achou << " VEZ!!!";
         }
       }
       
      if(esc == 5)
       {
        if(inicio == NULL)std::cout << "\n\tLISTA VAZIA!!!";
        else
         {
          aux = inicio;
           do{
              inicio = inicio->prox;
              delete(aux);
              aux = inicio;
             }while(aux != fim);
            delete(fim);
           inicio = NULL;
          std::cout << "\n\tLISTA ESVAZIADA COM SUCESSO ..!!!\n";
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