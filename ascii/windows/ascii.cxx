#include "getch.hxx"
#include "ascii.hxx"


void programa()
{
    char digito;
    
    __fpurge(stdin);
    std::cout << "Digite qualquer tecla ou pressione ESC para sair: ";
    digito = getch();
    
    if (converteAscii(digito) == 27)
    {
     std::cout << "Você pressiou ESC para sair do programa.\n";
     return;
    }
    
    std::cout << "Caracter: " << digito
              << "\nASCII   :" << converteAscii(digito) << "\n";
          
    programa();
}

int converteAscii(char digito)
{
 return (int) digito;
} 
