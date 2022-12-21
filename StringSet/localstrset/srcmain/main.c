#include <stdio.h>
#include "../include/strset" 
//#include <strset>

int main()
{  
 //char frase2[200]= "minha   frase com   algums     espaços";
 //strjst(frase2); //OK
 //printf("\t%s\n", frase2); //OK
  
  //char *r = rdspc(frase2); //OK
  //char *r = strjst(frase2); //OK
 //rvrs(frase2); 
 //printf("\t%s\n", frase2); //OK
 
 char frase3[300];
 printf("Digite uma frase: ");
 iostr(frase3);
   
 //char frase4[300];
 //printf("Digite uma frase: ");
 //fgets(frase4,300,stdin);
 //char *r = rdspc(frase4);
 //printf("\t%s\n",r);
 //free(r);

  //char frase5[200] = "minha   frase sem   todos  os     espaços";
  //rmspc(frase5); //OK
  //printf("\n\t%s\n\n", frase5); //OK  
  
 return 0;
}