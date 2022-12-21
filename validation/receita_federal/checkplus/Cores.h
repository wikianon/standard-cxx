/*
  Função que Gera Menu Animado para Inicio do Programa
  
  Para Utilizar basta incliuir a biblioteca <Cores.h>, e
  chamar por: Cores();
  
  Desenvolvido por: Braian Carlos da Silva Amaral
                    Leandro Daniel Lemos Murta
                    Lucas Fernandes Silva Campos
                    Luiz Fernando Ferreira Moreira
                    Luiz Henrique Diniz Sousa
*/

#include <iostream>
#include <conio.h>
#include <string>
#include <ctime>
#include <stdio.h>
#include <windows.h> 

using namespace std;

void Cores ()
{
     
HANDLE hConsole;

      int k;
      int cont = 1;
      hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
      for(k = 0; k < 100; k++)
      {
          SetConsoleTextAttribute(hConsole, cont);
          system("cls");
          cout << "\n\n\n\n\n\n\n\n\n\t\t\t ***Check Plus***";
          Sleep(10);
          cont++;
          if (cont==16) { cont = 1; }


      }
      SetConsoleTextAttribute(hConsole, 7);
      system("cls");
      
return;
}
