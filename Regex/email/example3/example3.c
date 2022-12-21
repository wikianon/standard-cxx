#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int uol(char*);
int bol(char*);
int gmail(char*);
int yahoo(char*);
int terra(char*);
int hotmail(char*);

void menu();

int main()
{
 menu();
 return 0;
}

void menu()
{
 char email[50];
 
 enum { Uol=1, Bol, Gmail, Terra, Yahoo, Hotmail, Exit }op;
 
 do{
    printf(
           "\n\tValidation e-mail"
           "\n\t[1]-Uol"
           "\n\t[2]-Bol"
           "\n\t[3]-Gmail"   
           "\n\t[4]-Earth"
           "\n\t[5]-Yahoo"
           "\n\t[6]-Hotmail"
           "\n\t[7]-Exit"
           "\n\tEnter: "
          );
    
    scanf("%d",&op);
    
    switch(op)
     {
      case Uol:
      
      do{
         printf("\n\tEnter your uol e-mail: ");
         scanf("%s", &email);
         
         printf("\n\tEmail: %s %s",email,(uol(email)?" is Valid.\n\n":" is Invalid.\n\n"));
          
        }while(!uol(email));
      
      break;
      
      case Bol:
      
      do{
         printf("\n\tEnter your bol e-mail: ");
         scanf("%s", &email);
         
         printf("\n\tEmail: %s %s",email,(bol(email)?" is Valid.\n\n":" is Invalid.\n\n"));
          
        }while(!bol(email));
      
      break;
      
      case Gmail:
      
      do{
         printf("\n\tEnter your google e-mail: ");
         scanf("%s", &email);
         
         printf("\n\tEmail: %s %s",email,(gmail(email)?" is Valid.\n\n":" is Invalid.\n\n"));
          
        }while(!gmail(email));
      
      break;
      
      case Terra:
      
      do{
         printf("\n\tEnter your earth e-mail: ");
         scanf("%s", &email);
         
         printf("\n\tEmail: %s %s",email,(terra(email)?" is Valid.\n\n":" is Invalid.\n\n"));
          
        }while(!terra(email));
      
      break;
      
      case Yahoo:
      
      do{
         printf("\n\tEnter your yahoo e-mail: ");
         scanf("%s", &email);
         
         printf("\n\tEmail: %s %s",email,(yahoo(email)?" is Valid.\n\n":" is Invalid.\n\n"));
          
        }while(!yahoo(email));
      
      break;
      
      case Hotmail:
      
      do{
         printf("\n\tEnter your hotmail e-mail: ");
         scanf("%s", &email);
         
         printf("\n\tEmail: %s %s",email,(hotmail(email)?" is Valid.\n\n":" is Invalid.\n\n"));
          
        }while(!hotmail(email));
      
      break;
      
      case Exit:
         printf("\n\tGood Bye!\n\n");
         exit(1);
      break;
        
      default:
         printf("\n\tInvalid Option!\n\n");
      break;
     }
     
   }while(1);
}

int uol(char* email)
{
 int j;
 char uol[12];
 int i = (strlen(email)-11);
 
 for( i, j = 0; j < 11; j++, i++ )uol[j] = email[i];
 
 return !strcmp(uol,"@uol.com.br");
}

int bol(char* email)
{
 int j;
 char bol[12];
 int i = (strlen(email)-11);
 
 for( i, j = 0; j < 11; j++, i++ )bol[j] = email[i];
 
 return !strcmp(bol,"@bol.com.br");
}

int gmail(char* email)
{
 int j;
 char google[11];
 int i = (strlen(email)-10);
 
 for( i, j = 0; j < 10; j++, i++ )google[j] = email[i];
 
 return !strcmp(google,"@gmail.com");
}

int yahoo(char* email)
{
 int j;
 char yahoo[14];
 int i = (strlen(email)-13);
 
 for( i, j = 0; j < 13; j++, i++ )yahoo[j] = email[i];
 
 return !strcmp(yahoo,"@yahoo.com.br");
}

int terra(char* email)
{
 int j;
 char terra[14];
 int i = (strlen(email)-13);
 
 for( i, j = 0; j < 13; j++, i++ )terra[j] = email[i];
 
 return !strcmp(terra,"@terra.com.br");
}

int hotmail(char* email)
{
 int j;
 char hotmail[16];
 int i = (strlen(email)-15);
 
 for( i, j = 0; j < 15; j++, i++ )hotmail[j] = email[i];
 
 return !strcmp(hotmail,"@hotmail.com.br");
}