/* 
   Valida Título Eleitoral
   Para utilizar, deve-se incluir a biblioteca: <TEVerifica.h>
   E chamar por: TEVerifica();
   
   Desenvolvido por: Braian Carlos da Silva Amaral
                     Leandro Daniel Lemos Murta
                     Lucas Fernandes Silva Campos
                     Luiz Fernando Ferreira Moreira
                     Luiz Henrique Diniz Sousa
*/

#include <iostream>
#include <string.h>
#include <conio.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

//Dados da Função

void TEVerifica ()

{
//Declarando Variáveis
int soma1, soma2, resto;
int dig1, dig2, i;
char temp[13];
int titulo[13];
//Entrando com o Numero
cout << "\n\nO TE deve ter 12 digitos, se nao tem, acrescente ""zeros"" a esquerda.";
cout << "\nDigite o Numero do TE: ";
     for (int i=1; i<=12; i++)
     {
         if(i-1==4) cout << " ";
         if(i-1==8) cout << " ";
         temp[0] = getche();
         titulo[i] = atoi (temp);
         temp[0] = ' ';
     }
//Calculo do Primeiro Dígito

     soma1 =((titulo[1] * 9) +
             (titulo[2] * 8) +
             (titulo[3] * 7) +
             (titulo[4] * 6) +
             (titulo[5] * 5) +
             (titulo[6] * 4) +
             (titulo[7] * 3) +
             (titulo[8] * 2));
                          
     resto = (soma1 % 11);
     if (resto == 0)
     {
        if(resto == 0)
        dig1 = 1;
        else
        dig1 = 0;
     }
     else
     {
         if(resto == 1)
         dig1 == 0;
         else
         dig1 = 11 - resto;
     }

// Calculo do Segundo Dígito


     dig2 =((titulo[9] * 4) +
             (titulo[10] * 3) +
             (dig1 * 2));
             
     resto = (dig2 % 11);
     
     if(resto == 0)
     {
              if(resto == 0) 
              dig2 = 1; 
              else 
              dig2 = 0;
     }
     else
     {
         if(resto == 1) 
         dig2 = 0;
         else 
         dig2 = 11 - resto;
     }   
//Valida e Informa o Estado de Emissão
  
    
    if (dig1 == titulo[11] && dig2 == titulo[12])
    {
       cout << "\n\nTitulo Valido\n\n";
       if(titulo[9]==0 && titulo[10]==1) 
       cout << "Titulo emitido em SP\n";
	   if(titulo[9]==0 && titulo[10]==2) 
       cout << "Titulo emitido em MG\n";
	   if(titulo[9]==0 && titulo[10]==3) 
       cout << "Titulo emitido no RJ\n";
	   if(titulo[9]==0 && titulo[10]==4) 
       cout << "Titulo emitido no RS\n";
	   if(titulo[9]==0 && titulo[10]==5) 
       cout << "Titulo emitido na BA\n";
	   if(titulo[9]==0 && titulo[10]==6) 
       cout << "Titulo emitido no PR\n";
	   if(titulo[9]==0 && titulo[10]==7) 
       cout << "Titulo emitido no CE\n";
	   if(titulo[9]==0 && titulo[10]==8)  
       cout << "Titulo emitido em PE\n";
	   if(titulo[9]==0 && titulo[10]==9) 
       cout << "Titulo emitido em SC\n";
	   if(titulo[9]==1 && titulo[10]==0) 
       cout << "Titulo emitido em GO\n";
       if(titulo[9]==1 && titulo[10]==1)
       cout << "Titulo emitido no MA\n";
       if(titulo[9]==1 && titulo[10]==2)
       cout << "Titulo emitido na PB\n";
       if(titulo[9]==1 && titulo[10]==3)
       cout << "Titulo emitido na PA\n";
       if(titulo[9]==1 && titulo[10]==4)
       cout << "Titulo emitido no ES\n";
       if(titulo[9]==1 && titulo[10]==5)
       cout << "Titulo emitido no PI\n";
       if(titulo[9]==1 && titulo[10]==6)
       cout << "Titulo emitido no RN\n";
       if(titulo[9]==1 && titulo[10]==7)
       cout << "Titulo emitido em AL\n";
       if(titulo[9]==1 && titulo[10]==8)
       cout << "Titulo emitido em MT\n";
       if(titulo[9]==1 && titulo[10]==9)
       cout << "Titulo emitido no MS\n";
       if(titulo[9]==2 && titulo[10]==0)
       cout << "Titulo emitido no DF\n";
       if(titulo[9]==2 && titulo[10]==1)
       cout << "Titulo emitido em SE\n";
       if(titulo[9]==2 && titulo[10]==2)
       cout << "Titulo emitido no AM\n";
       if(titulo[9]==2 && titulo[10]==3)
       cout << "Titulo emitido em RO\n";
       if(titulo[9]==2 && titulo[10]==4)
       cout << "Titulo emitido no AC\n";
       if(titulo[9]==2 && titulo[10]==5)
       cout << "Titulo emitido em AP\n";
       if(titulo[9]==2 && titulo[10]==6)
       cout << "Titulo emitido em RR\n";
       if(titulo[9]==2 && titulo[10]==7)
       cout << "Titulo emitido em TO\n";
       if(titulo[9]==2 && titulo[10]==8)
       cout << "Titulo emitido no exterior\n";
       }    
       else
       {
       cout << "\n\nTitulo de Eleitor Invalido\n";
       }
       
    return;
}
