#include <iostream>

void carter(int ano)
{
 int dia;
 int mes;
 
 //O Metodo de carter só permite calcular a data
 //da pascoa entre os anos de 1900 e 2099
 
  int a = (255 - (11 * ( ano % 19 )));
  int b = (((a - 21) % 30) + 21);
  
  if(b > 38)b=(b-1);
  
  int c = (( ano + (ano / 4) + b + 1) % 7);
  int d = ((b + 7) - c);
  
  if(d < 32)
   {
    dia = d;
    mes = 3;
   }
   else
   {
    dia = (d - 31);
    mes = 4;
   }
   
  printf("\n\tO dia da pascoa é: %02d/%02d/%4d\n\n",dia,mes,ano);
}

void oudin(int ano)
{
 //O Metodo de oudin permite calcular a data
 //da pascoa para todos os anos a partir de 1900
 
 int sec = ( ano / 100 );
 int a = (ano - (19 * ( ano / 19)));
 int b = ((sec - 17) / 25);
 int c = ((sec - ( sec / 4 )) - ((sec - b) / 3) + ((19 * a) + 15));
 int d = (c - (30 * (c / 30 )));
 int e = (d - (( d / 28 ) * (1 - (1 / 28)) * (29 / (d + 1)) * ((21 - a) / 11)));
 int f = ((ano + ( ano / 4 )) + (((e + 2) - sec) + ( sec / 4 )));
 int g = (f - (7 * (f / 7)));
 int h = (e - g);
 int mes = (3 + ((h + 40) / 44));
 int dia = ((h + 28) - (31 * (mes / 4)));

 printf("\n\tO dia da pascoa é: %02d/%02d/%4d\n\n",dia,mes,ano);
}

int main()
{
 int ano;
 
 do{
    std::cout << "\n\tdigite um ano entre 1900 e 2099: ";
    std::cin >> ano;
    
    if(ano < 1900 || ano > 2099)std::cout<<"\n\tano invalido!\n\n";
      
   }while(ano < 1900 || ano > 2099);
   
 carter(ano);
 oudin(ano);
 return 0;
}