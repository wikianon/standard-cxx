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
 * @file     agenda.cxx
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
int main()
{
 int num;
 char op[2];
 char Nome[100];
      
 Arvore *raiz = NULL;
 
 menu(&num);
 
 while(num != 6) 
 {
  switch(num)
  {
    case 1:
          do{
             printf(
                    "\n\n\t*-------------------*"
                    "\n\t|  INSERT CONTACTS  |"
                    "\n\t*-------------------*\n"
                   );
             getNames(Nome);
             inserir(&raiz,Nome);
             printf("\n\tDo you want to continue (y)es or (n)o: ");
             scanf(" %s",&op);
             if(*op == 'n' || *op == 'N')menu(&num);
            }while(*op == 'y' || *op == 'Y');
            break;
    case 2:  
           if(raiz == NULL)
            {
             printf(
                    "\n\n\t*------------------------*"
                    "\n\t|  CONTACTS NOT FOUND!!  |"
                    "\n\t*------------------------*"
                   ); 
             printf("\n\n\tDo you want to continue (y)es or (n)o: ");
             scanf(" %s",&op);     
             if(*op == 'y' || *op == 'Y')menu(&num);
             else
             return 0;
            }
           else
           {
            printf(
                   "\n\n\t*-----------------*"
                   "\n\t|  VIEW CONTACTS  |"
                   "\n\t*-----------------*\n"
                  );
            ordem(raiz);
            printf("\n\n\tDo you want to continue (y)es or (n)o: ");
            scanf(" %s",&op);     
            if(*op == 'y' || *op == 'Y')menu(&num);
            else
            return 0;
           }
           break;
    case 3:
          do{
             printf(
                    "\n\n\t*------------------*"
                    "\n\t|  SEARCH CONTACT  |"
                    "\n\t*------------------*\n"
                   );
             getNames(Nome);
             busca(raiz,Nome);
             printf("\n\n\tDo you want to continue (y)es or (n)o: ");
             scanf(" %s",&op);
             if(*op == 'n' || *op == 'N')menu(&num);
      
           }while(*op == 'y' || *op == 'Y'); 
           break;
    case 4:
          do{
             printf(
                    "\n\n\t*------------------*"
                    "\n\t|  REMOVE CONTACT  |"
                    "\n\t*------------------*\n"
                   );
             getNames(Nome);
             busca(raiz,Nome);
             printf("\n\n\tDo you want to continue (y)es or (n)o: ");
             scanf(" %s",&op);
             if(*op == 'n' || *op == 'N')menu(&num);
             else
             {
              excluir(&raiz,Nome);
              printf(
                     "\n\n\t*-------------------*"
                     "\n\t|  CONTACT REMOVED  |"
                     "\n\t*-------------------*\n"
                    );    
              printf("\n\n\tDo you want to continue (y)es or (n)o: ");
              scanf(" %s",&op);    
              if(*op == 'n' || *op == 'N')menu(&num);
             }
           }while(*op == 'y' || *op == 'Y');       
           break;
    case 5:
          do{
             printf(
                    "\n\n\t*-----------------*"
                    "\n\t|  ALTER CONTACT  |"
                    "\n\t*-----------------*\n"
                   );
             getNames(Nome);
             busca(raiz,Nome);
             printf("\n\n\tDo you want to continue (y)es or (n)o: ");
             scanf(" %s",&op);
             if(*op == 'y' || *op == 'Y')
              {
               alterar(&raiz,Nome);
               printf("\n\n\tDo you want to continue (y)es or (n)o: ");
               scanf(" %s",&op);
               if(*op == 'n' || *op == 'N')menu(&num);
               break;
              }
             else
             menu(&num);  
            }while(*op == 'y' || *op == 'Y');   
            break;
    case 6:
           printf("\n\tGood Bye.\n\n");
           break;   
  };
 }
 return 0;
}