/*
  Função Geradora de CPF
  Para Utilizar, basta incluir a biblioteca: <GerarCPF.h>
  E chamar por: GerarCPF();
  
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
// Dados da Função
void GerarCPF()

{
//Declarando Variáveis
    int temp, CPF[9], i;
    int soma1, parte1, parte2, parte3, dig1;
    int soma2, parte5, parte6, parte7, dig2;
    int Q;
    
//Pegando a Quantidade a Gerar de CPFs

    cout << "\nDigite a Quantidade a Gerar: ";
    cin >> Q;
    cout << "\n\n";
//Gerando Aleatoriamente os 9 primeiros dígitos.
    srand(time (0));
    for(temp=1; temp<=Q; temp++)
    {
                for(i=1; i<=9; i++)
                         CPF[i]=rand()%9;

// Calculando o Primeiro Dígito

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

// Calculando o Sugundo Dígito

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

//Escrevendo o Número Completo

        for(i=1; i<=9; i++)
        {
                 cout << CPF[i];
                 if(i-1==2) cout << ".";
                 if(i-1==5) cout << ".";
                 }
                 cout << "-";
                 cout << dig1 << dig2 << "\n";
                 if(CPF[9]==0) 
//Informando o Estado de Emissão

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


cout << "\n\n";
    return;
}
