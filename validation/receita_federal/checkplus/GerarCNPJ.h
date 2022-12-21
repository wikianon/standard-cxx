/*
  Função Geradora de CNPJ
  Para utilizar basta incluir a biblioteca: <GerarCNPJ.h>
  E chamar por: GerarCNPJ();
  
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
void GerarCNPJ()

{
    //Declarando Variáveis
    int temp, CNPJ[12], i;
    int soma1, parte1, parte2, parte3, dig1;
    int soma2, parte5, parte6, parte7, dig2;
    int Q;
    //Entrando com a Quantidade
    cout << "\n\nDigite a Quantidade de CNPJ a Gerar: ";
    cin >> Q;
    cout << "\n\n";
    //Gerando Aleatóriamente
    srand(time (0));
    for(temp=1; temp<=Q; temp++)
    {
                for(i=1; i<=8; i++)
                         CNPJ[i]=rand()%9;
    
    CNPJ[9]  = 0;
    CNPJ[10] = 0;
    CNPJ[11] = 0;
    CNPJ[12] = rand()%9;
//Calculando o Primeiro Dígito

    soma1 = ((CNPJ[1] * 5) +
             (CNPJ[2] * 4) +
             (CNPJ[3] * 3) +
             (CNPJ[4] * 2) +
             (CNPJ[5] * 9) +
             (CNPJ[6] * 8) +
             (CNPJ[7] * 7) +
             (CNPJ[8] * 6) +
             (CNPJ[9] * 5) +
             (CNPJ[10] * 4) +
             (CNPJ[11] * 3) +
             (CNPJ[12] * 2));
             
     parte1 = int(soma1 / 11);
     parte2 = (parte1 * 11);
     parte3 = (soma1 - parte2);
     dig1   = ( 11 - parte3);
     if (dig1 > 9) dig1 = 0;

//Calculando o Segundo Dígito

     soma2 =((CNPJ[1] * 6) +
             (CNPJ[2] * 5) +
             (CNPJ[3] * 4) +
             (CNPJ[4] * 3) +
             (CNPJ[5] * 2) +
             (CNPJ[6] * 9) +
             (CNPJ[7] * 8) +
             (CNPJ[8] * 7) +
             (CNPJ[9] * 6) +
             (CNPJ[10] * 5) +
             (CNPJ[11] * 4) +
             (CNPJ[12] * 3) +
             (dig1 * 2));
             
     parte5 = int(soma2 / 11);
     parte6 = (parte5 * 11);
     parte7 = (soma2 - parte6);
     dig2   = (11 - parte7);
     if (dig2 > 9) dig2 = 0;
//Escrevendo o Numero Completo
     
        for(i=1; i<=12; i++)
        {
                 cout << CNPJ[i];
                 if(i==2) cout << ".";
                 if(i==5) cout << ".";
                 if(i==8) cout << "/";
                 }
                 cout << "-";
                 cout << dig1 << dig2 << "\n";
                 
}

    return;
}
