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
 * @file       source5
 * @version    0.1
 * @brief      Lista do tipo fila dinamica.
 * @consult    estruturas de dados algoritmos, análise da complexidade e implementações em java e cc++ - ana fernanda gomes ascencio & graziela santos araújo
 * @author     Jean Zonta
 * @Copyright (C) 2013-2017 Jean Zonta.
 * 
 * @end @section author Author
 *
*/
struct fila
{
 int num;
 fila *prox;
};

int main()
{
 int esc;
 int qtd;
 fila *aux;
 fila *fim = NULL;
 fila *inicio = NULL;
 
 do{
    std::cout << "\n\tLISTA DO TIPO FILA DINAMICA"
                 "\n\tMENU DE ESCOLHA"
                 "\n\t[1]-INSERIR NA FILA"
                 "\n\t[2]-CONSULTAR TODA A FILA"
                 "\n\t[3]-REMOVER DA FILA"
                 "\n\t[4]-ESVAZIAR A FILA"
                 "\n\t[5]-SAIR"
                 "\n\tESCOLHA: ";
     std::cin >> esc;
     
     if(esc == 1)
      {
       std::cout << "\n\tQUANTOS NUMEROS DESEJA INSERIR?\n\tDIGITE: ";
       std::cin >> qtd;
       
       for(int i = 0; i < qtd; i++)
        {
         std::cout << "\n\tINSIRA NA FILA O NUMERO " << i+1 << "º: ";
         fila *novo = new fila();
         std::cin >> novo->num;
         novo->prox = NULL;
          
         if(inicio == NULL)
          {
           inicio = novo;
           fim = novo;
          }
         else
          {
           fim->prox = novo;
           fim = novo;
          }
         std::cout << "\n\tNUMERO " << novo->num << " INSERIDO COM SUCESSO ..!!!\n";
        } 
      }
       
     if(esc == 2)
      {
       if(inicio == NULL)std::cout << "\n\tFILA VAZIA ..!!!\n";
       else
        {
         std::cout << "\n\tFILA COMPLETA: ";
         aux = inicio;
         while (aux != NULL)
          {
           std::cout << aux->num << " ";
           aux = aux->prox;
          }
         std::cout<<std::endl; 
        }
      }
       
     if(esc == 3)
      {
       if(inicio == NULL)std::cout << "\n\tFILA VAZIA ..!!!\n";
       else
        {
         aux = inicio;
          std::cout << "\n\tNUMERO " << inicio->num << " REMOVIDO COM SUCESSO ..!!!\n";
          inicio = inicio->prox;
         delete(aux);
        }
      }
       
     if(esc == 4)
      {
       if(inicio == NULL)std::cout << "\n\tFILA VAZIA ..!!!\n";
       else
        {
         aux = inicio;
         while (aux != NULL)
          {
           inicio = inicio->prox;
            delete(aux);
           aux = inicio;
          }
         std::cout << "\n\tFILA ESVAZIADA COM SUCESSO ...!!!!\n";
        }
      }
       
      if(esc < 1 || esc > 5)
      std::cout << "\n\tOPÇÃO INVÁLIDA!!!";
       else
      if(esc == 5)  
      std::cout << "\n\tGOOD BYE ...!!\n\n";
      
   }while(esc != 5);
 return 0;
}