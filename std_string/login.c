#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MD5_PASSWD_LEN 35

#define isMD5(passwd) (strncmp(passwd, "md5", 3) == 0 && strlen(passwd) == MD5_PASSWD_LEN)

char*  itoa(int value, char* str)
{
 char temp;
 int i =0;
 
 while (value > 0)
  {
   int digito = value % 10;
     
   str[i] = digito + '0';
   value /= 10;
   i++;
  }
  i = 0;
  int j = strlen(str) - 1;
   
 while (i < j)
  {
   temp = str[i];
   str[i] = str[j];
   str[j] = temp;
   i++;
   j--;
  }
 return str;
}
 
int main()
{
 char login[6]={0};
   
 int passwd;
 char senha[6]={0};
 
 char logins[6]="Teste";
 char senhas[6]="12345";
   
  do{
     printf("\n\tDigite o Login: ");
     scanf("%s",&login);
 
     if(strcmp(login, logins) == 0)break;
     else
     printf("\tUsuario: %s Inválido..!!\n", login);
 
     }while(strcmp(login, logins) != 0);
 
  do{
     printf("\n\tDigite sua senha: ");
     scanf("%d",&passwd);
 
     sprintf(senha,"%d",passwd);
     //snprintf(senha, 6, "%d", passwd);
       //itoa(passwd,senha);
 
     if(strcmp(senha, senhas) == 0)
     printf("\n\tUsuario e Senha Válidos\n\tAcesso autorizado..!!!\n\n");
       else
     printf("\tSenha: %s Inválida..!!\n", senha);
 
    }while(strcmp(senha, senhas) != 0);
    
   return 0;
}