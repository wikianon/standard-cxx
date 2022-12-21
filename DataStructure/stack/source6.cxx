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
 * @file       source6
 * @version    0.1
 * @brief      Pilha Desempliha como estrutura dinamica.
 * @consult    estruturas de dados algoritmos, análise da complexidade e implementações em java e cc++ - ana fernanda gomes ascencio & graziela santos araújo
 * @author     Jean Zonta
 * @Copyright (C) 2013-2017 Jean Zonta.
 * 
 * @end @section author Author
 *
 */

struct pilha
{
 int num;
 pilha *prox;
};

int main()
{
 int esc;
 int qtd;
 pilha *aux;
 pilha *topo = NULL;
  
  do{
     std::cout << "\n\n\tPILHA DESEMPLILHA COMO ESTRUTURA DINAMICA"
                  "\n\tMENU DE ESCOLHA"
                  "\n\t[1]-INSERIR NA PILHA"
                  "\n\t[2]-CONSULTAR TODA PILHA"
                  "\n\t[3]-REMOVER DO TOPO"
                  "\n\t[4]-DESEMPILHAR E REMOVER"
                  "\n\t[5]-ESVAZIAR A PILHA" 
                  "\n\t[6]-SAIR"
                  "\n\tESCOLHA: ";
     std::cin >> esc;
    
       if(esc == 1)
        {
         std::cout << "\n\tQUANTOS NUMEROS DESEJA INSERIR?\n\tDIGITE: ";
         std::cin >> qtd;
         for(int i = 0;i < qtd; i++)
         {
          std::cout << "\n\tINSIRA NA PILHA O NUMERO " << i+1 << "º: ";
           pilha *novo = new pilha();
            std::cin >> novo->num;
            novo->prox = topo;
           topo = novo;
          std::cout << "\n\tNUMERO " << novo->num << " INSERIDO COM SUCESSO!!!\n";
         }
        }
        
       if(esc == 2)
        {
         if(topo == NULL)std::cout << "\n\tPILHA VAZIA!!!";
         else
          {
           std::cout << "\n\tPILHA COMPLETA: ";
           aux = topo;
           while(aux != NULL)
           {   
            std::cout << aux->num << " ";
            aux = aux->prox;
           }
          }
        }
        
       if(esc == 3)
        {
         if(topo == NULL)std::cout << "\n\tPILHA VAZIA!!!";
         else
          {
           aux = topo;
            std::cout << "\n\tNUMERO: " << topo->num << " REMOVIDO COM SUCESSO!!!";
            topo = topo->prox;
           delete(aux);
          }
        }
                
       if(esc == 4)
        {
         if(topo == NULL)std::cout << "\n\tPILHA VAZIA!!!";
         else
          {
           aux = NULL;
           //std::cout<<"\n\tTOPO: "<<topo->num;
           if(topo->prox != NULL)
            {
             if(topo->prox->prox)
              aux = topo->prox->prox;
              std::cout << "\n\tNUMERO: " << topo->prox->num << " REMOVIDO COM SUCESSO!!!";
              delete topo->prox;     
             topo->prox = aux;
            }
          }
        }
        
       if(esc == 5)
        {
         if(topo == NULL)std::cout << "\n\tPILHA VAZIA!!!";
         else
          {
           aux = topo;
           while(aux != NULL)
           {
            topo = topo->prox;
             delete(aux);
            aux = topo;
           }
           std::cout << "\n\tPILHA ESVAZIADA COM SUCESSO!!!!";
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