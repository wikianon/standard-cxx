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
 * @file       source1
 * @version    0.1
 * @brief      Algoritmos de Busca Sequencial Ordenada e nao Ordenada.
 * @consult    estruturas de dados algoritmos, análise da complexidade e implementações em java e cc++ - ana fernanda gomes ascencio & graziela santos araújo
 * @author     Jean Zonta
 * @Copyright (C) 2013-2017 Jean Zonta.
 * 
 * @end @section author Author
 *
*/
int main()
{
 int i, n, esc, qtd, achou, X[10];

 while (esc != 3)
  {
   std::cout << "\n\tALGORITMOS DE BUSCA SEQUENCIAL"
                "\n\tORDENADA E NÃO ORDENADA\n"
                "\n\tMENU DE ESCOLHA"
                "\n\t[1]-NUMEROS NÃO ORDENADOS "
                "\n\t[2]-NUMEROS ORDENADOS "
                "\n\t[3]-SAIR"
                "\n\tESCOLHA: ";
   std::cin >> esc;
         
   if(esc == 1)
    {
     std::cout << "\n\tVETOR COM NUMEROS NÃO ORDENADOS!!!!\n\tQUANTOS NUMEROS DESEJA INSERIR?\n\tDIGITE: ";
     std::cin >> qtd;
     
     for(i = 0; i < qtd; i++)
      {
       std::cout << "\n\tDIGITE O NUMERO " << i+1 << "º: ";
       std::cin >> X[i];
      }
                
     do{
        std::cout << "\n\tBUSQUE UM NUMERO NO VETOR: ";
        std::cin >> n;
        i = 0;
        achou = 0;
                 
        while(i <= 9 && achou == 0)
         {
          if(X[i] == n)achou = 1;
           else
          i++;
         }
                   
        if(achou == 0)
        std::cout << "\n\tNUMERO " << n << " NÃO ENCONTRADO!!!!\n";
         else
        std::cout << "\n\tNUMERO " << n << " ENCONTRADO NA POSIÇÃO: " << i+1 << "\n";
       }while(!achou);
    }
    else      
    if(esc == 2)
     {
      std::cout << "\n\tVETOR COM NÚMEROS ORDENADOS!!!!\n\tQUANTOS NÚMEROS DESEJA INSERIR?\n\tDIGITE: ";
      std::cin >> qtd;
      for(i = 0; i < qtd; i++)
       {
        std::cout << "\n\tDIGITE O NUMERO " << i+1 << "º: ";
        std::cin >> X[i];
       }
                
     do{
        std::cout << "\n\tBUSQUE UM NUMERO NO VETOR: ";
        std::cin >> n;
        i = 0;
        achou = 0;
        
        while(i <= 9 && achou == 0 && n >= X[i])
         {
          if(X[i] == n)achou = 1;
          else
          i++;
         }
                   
        if(achou == 0)
        std::cout << "\n\tNUMERO " << n << " NÃO ENCONTRADO!!!!\n";
         else
        std::cout << "\n\tNUMERO " << n << " ENCONTRADO NA POSIÇÃO: " << i+1 << "\n";
       }while(!achou);
     }
     if(esc < 1 || esc > 3)
     std::cout << "\n\tOPÇÃO INVÁLIDA!!!";
      else
     if(esc == 3)  
     std::cout << "\n\tGOOD BYE ...!!\n\n";
  }
 return 0;
}