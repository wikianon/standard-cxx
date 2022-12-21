/* 
   Função que Verifica se um CPF é Valido.
   
   Para utilizar basta incluir a biblioteca: <CPFVerifica.h>, 
   E chamar por: CPFVerifica();
   
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

void CPFVerifica ()

{
//Declarando Variáveis

int soma1, soma2, parte1, parte2, parte3;
int parte5, parte6, parte7; 
int dig1, dig2, i, erro=0;
char temp[11];
int CPF[11];

//"Pegando" o CPF da Tela.

cout << "\n\nDigite o numero do CPF: ";

     for (int i=1; i<=11; i++)
     {
         if(i-1==3) cout << ".";
         if(i-1==6) cout << ".";
         if(i-1==9) cout << "-";
         temp[0] = getche();
         CPF[i] = atoi (temp);
         temp[0] = ' ';
     }
//Calculo do Primeiro Dígito

     soma1 =((CPF[1]*10) +
             (CPF[2] * 9) +
             (CPF[3] * 8) +
             (CPF[4] * 7) +
             (CPF[5] * 6) +
             (CPF[6] * 5) +
             (CPF[7] * 4) +
             (CPF[8] * 3) +
             (CPF[9] * 2));
             
     parte1 = int(soma1 / 11);
     parte2 = (parte1 * 11);
     parte3 = (soma1 - parte2);
     dig1   = ( 11 - parte3);
     if (dig1 > 9) dig1 = 0;

// Calculo do Segundo Dígito


     soma2 =((CPF[1]*11) +
             (CPF[2] * 10) +
             (CPF[3] * 9) +
             (CPF[4] * 8) +
             (CPF[5] * 7) +
             (CPF[6] * 6) +
             (CPF[7] * 5) +
             (CPF[8] * 4) +
             (CPF[9] * 3) +
             (dig1 * 2));
             
     parte5 = int(soma2 / 11);
     parte6 = (parte5 * 11);
     parte7 = (soma2 - parte6);
     dig2   = (11 - parte7);
     if (dig2 > 9) dig2 = 0;

//Invalida CPF com todos os dígitos iguais.
  
    
    if (dig1==CPF[10] && dig2==CPF[11])
    {
       if(CPF[1]==1 && CPF[2]==1 && CPF[3]==1 && CPF[4]==1 && CPF[5]==1 
       && CPF[6]==1 && CPF[7]==1 && CPF[8]==1 && CPF[9]==1)
       cout << "\n\nCPF Invalido - Digitos Iguais\n", erro++;        
       else if(CPF[1]==2 && CPF[2]==2 && CPF[3]==2 && CPF[4]==2 && CPF[5]==2 
       && CPF[6]==2 && CPF[7]==2 && CPF[8]==2 && CPF[9]==2)
       cout << "\n\nCPF Invalido - Digitos Iguais\n", erro++;
       else if(CPF[1]==3 && CPF[2]==3 && CPF[3]==3 && CPF[4]==3 && CPF[5]==3 
       && CPF[6]==3 && CPF[7]==3 && CPF[8]==3 && CPF[9]==3)
       cout << "\n\nCPF Invalido - Digitos Iguais\n", erro++;
       else if(CPF[1]==4 && CPF[2]==4 && CPF[3]==4 && CPF[4]==4 && CPF[5]==4 
       && CPF[6]==4 && CPF[7]==4 && CPF[8]==4 && CPF[9]==4)
       cout << "\n\nCPF Invalido - Digitos Iguais\n", erro++;
       else if(CPF[1]==5 && CPF[2]==5 && CPF[3]==5 && CPF[4]==5 && CPF[5]==5 
       && CPF[6]==5 && CPF[7]==5 && CPF[8]==5 && CPF[9]==5)
       cout << "\n\nCPF Invalido - Digitos Iguais\n", erro++;
       else if(CPF[1]==6 && CPF[2]==6 && CPF[3]==6 && CPF[4]==6 && CPF[5]==6 
       && CPF[6]==6 && CPF[7]==6 && CPF[8]==6 && CPF[9]==6)
       cout << "\n\nCPF Invalido - Digitos Iguais\n", erro++;
       else if(CPF[1]==7 && CPF[2]==7 && CPF[3]==7 && CPF[4]==7 && CPF[5]==7 
       && CPF[6]==7 && CPF[7]==7 && CPF[8]==7 && CPF[9]==7)
       cout << "\n\nCPF Invalido - Digitos Iguais\n", erro++;
       else if(CPF[1]==8 && CPF[2]==8 && CPF[3]==8 && CPF[4]==8 && CPF[5]==8 
       && CPF[6]==8 && CPF[7]==8 && CPF[8]==8 && CPF[9]==8)
       cout << "\n\nCPF Invalido - Digitos Iguais\n", erro++;
       else if(CPF[1]==9 && CPF[2]==9 && CPF[3]==9 && CPF[4]==9 && CPF[5]==9 
       && CPF[6]==9 && CPF[7]==9 && CPF[8]==9 && CPF[9]==9)
       cout << "\n\nCPF Invalido - Digitos Iguais\n", erro++;
       else if(CPF[1]==0 && CPF[2]==0 && CPF[3]==0 && CPF[4]==0 && CPF[5]==0 
       && CPF[6]==0 && CPF[7]==0 && CPF[8]==0 && CPF[9]==0)
       cout << "\n\nCPF Invalido - Digitos Iguais\n", erro++;

// Informa CPF Valido, e estado de Emissao.

       if (erro==0)
       {
       cout << "\n\nCPF Valido\n\n";
       if(CPF[9]==0) 
       cout << "CPF emitido no estado do RS\n";
	   if(CPF[9]==1) 
       cout << "CPF emitido em um desses estados: DF-GO-MS-MT-TO\n";
	   if(CPF[9]==2) 
       cout << "CPF emitido em um desses estados: AC-AM-AP-PA-RO-RR\n";
	   if(CPF[9]==3) 
       cout << "CPF emitido em um desses estados: CE-MA-PI\n";
	   if(CPF[9]==4) 
       cout << "CPF emitido em um desses estados: AL-PB-PE-RN\n";
	   if(CPF[9]==5) 
       cout << "CPF emitido em um desses estados: BA-SE\n";
	   if(CPF[9]==6) 
       cout << "CPF emitido no estado de MG\n";
	   if(CPF[9]==7) 
       cout << "CPF emitido em um desses estados: ES-RJ\n";
	   if(CPF[9]==8) 
       cout << "CPF emitido no estado de SP\n";
	   if(CPF[9]==9) 
       cout << "CPF emitido em um desses estados: PR-SC\n";
       }
       } 
    
       else
       {
       cout << "\n\nCPF Invalido";
       }
       cout << "\n\n";
    return;
}
