#include <iostream>
//Código que gera um cpf aleatório e cria os dígitos verificadores válidos.
int main()
{
 int y,i,dv1,dv2,total,cpfnum[10];

 // geracpf
 std::cout << "***************************\n"
              " Gerador automatico de cpf \n"
              "  por mbmaciel@gmail.com   \n"
              "***************************\n";

 // gera os numeros do cpf
 srand(time(NULL));
 for(i = 0; i <= 8; i++)
  {
   cpfnum[i] = rand() % 9;
   // imprime os numeros
   std::cout <<  cpfnum[i] ;
  }
  
 // primeiro digito verificador
 total = 0;
 y = 10;
 
 for(i = 0; i <= 8; i++)
  {
   total = total + (cpfnum[i] * y);
   y--;
  }
  
 if((total % 11) < 2)
  {
   dv1 = 0;
  }
  else
  {
   dv1 = 11 - (total % 11);
  }
  
 // Segundo digito verificador
 y = 11;
 total = 0;
 
 for(i = 0; i <=8; i++)
  {
   total = total + (cpfnum[i] * y);
   y--;
  }

 total = total + (dv1 * y);
 
 if((total % 11) < 2)
  {
   dv2 = 0;
  } 
  else
  {
   dv2 = 11 - (total % 11);
  }
  
  std::cout << "-" << dv1 << dv2 << "\n" ;

 return 0;
}