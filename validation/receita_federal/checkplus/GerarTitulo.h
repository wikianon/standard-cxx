/*
  Função Geradora de Titulo Eleitoral
  Para utilizar, inclua a biblioteca: <GerarTitulo.h>
  E chame por: GerarTitulo();
  
  Desenvolvido por:  Braian Carlos da Silva Amaral
                     Leandro Daniel Lemos Murta
                     Lucas Fernandes Silva Campos
                     Luiz Fernando Ferreira Moreira
                     Luiz Henrique Diniz Sousa
*/

#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <conio.h>
using namespace std;
//Dados da Função
void GerarTitulo ()

{
//Declarando variáveis
     int Q;
     int temp, titulo[12], i;
     int soma1, dig1, dig2, resto;
//Entrando com Dados
cout << "\n\nDigite a quantidade de TE a gerar: ";
cin >> Q;
cout << "\n\n";
    
//Gerando Aleatoriamente
    
    srand(time (0));
    for(temp=1; temp<=Q; temp++)
    {
                for(i=1; i<=8; i++)
                         titulo[i]=rand()%9;

//Numero [9] pode ser somente 0,1 e 2
    titulo[9] = rand()%3;
    do
    {
              //Numero[10] pode ser somente 0-9 e se [9] for igual a 2
              //[10] não pode ser igual a 9.
              titulo[10] = rand()%9;
    }while (titulo[9]==0 && titulo[10]<=1 || titulo[9]==2 && titulo[10]==9); 

// Primeiro Dígito

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

// Segundo Dígito
     
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

// Escrever Título
   

       for(i=1; i<=10; i++)
       {
                cout << titulo[i];
                if(i==4) cout << " ";
                if(i==8) cout << " ";
       }
       cout << dig1 << dig2;
//Informando Estado de Emissão       
       cout << "\n";
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
       
       cout << "\n\n";
}

    return;
}
