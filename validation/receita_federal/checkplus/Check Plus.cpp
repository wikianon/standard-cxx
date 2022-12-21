/*
    Trabalho Interdisciplinar
    Institui��o: Pontif�cia Universidade Cat�lica de Minas Gerais - Betim
    Curso: Bacharelado em Sistemas de Informa��o
    Per�odo: Primeiro
    Data: Junho de 2009
    Disciplina: Algor�timos e T�cnicas de Programa��o
    Professor: Aluisio Silva
    Alunos: Braian Carlos da Silva Amaral
            Leandro Daniel Lemos Murta
            Lucas Fernandes Silva Campos
            Luiz Fernando Ferreira Moreira
            Luiz Henrique Diniz Sousa
    
    Objetivo do Programa:
             - Fazer Valida��o de CPF, CNPJ e T�tulo Eleitoral;
             - Descobrir D�gitos Verificadores;
             - Gerar N�meros Aleat�rios e V�lidos.
*/

/*
  Programa Principal
*/

//Incluindo Bibliotecas
//Extras
#include <Cores.h>
#include <Limpar.h>
#include <Creditos.h>
#include <Pause.h>

//Verificadoras
#include <CPFVerifica.h>
#include <CNPJVerifica.h>
#include <TEVerifica.h>

//Geradoras de D�gitos
#include <DigCPF.h>
#include <DigCNPJ.h>
#include <DigTitulo.h>

//Geradoras de N�meros
#include <GerarCPF.h>
#include <GerarCNPJ.h>
#include <GerarTitulo.h>

//Declarando Variaveis
int Erro;
char OpcC[0], sair;
int OpcI[0];

// Inicio do Programa

using namespace std;
int main()
{
//Cores no Inicio
Cores();
//Cor do Programa
system ("color 1F");

inicio:
Erro=0;
do
{
//Menu Inicial
//Fun��o para Limpar a Tela

Limpar();
cout << "--------------------------------------\n";
cout << "-------------Check Plus---------------\n";
cout << "------- O Melhor Validador! ----------\n";
cout << "--------------------------------------\n";
cout << "Digite uma Opcao:                     \n";  
cout << " 1 - CPF\n";
cout << " 2 - CNPJ\n";
cout << " 3 - Titulo Eleitoral\n";
cout << " 4 - Creditos\n";
cout << " 5 - Sair do Programa\n";

     //IF para informar opcao errada

     Erro++;
     if(Erro > 1)
     {
             cout << "\nOpcao Invalida, Digite Novamente!\n\n";
     }
     cout << "\nInforme sua Opcao: ";
     // Pegando um caractere char do teclado, e trasformando para int
          
     OpcC[0] = getche();
     OpcI[0] = atoi (OpcC);
     OpcC[0] = ' ';
//Repetindo ate op��o estiver valida.

}while (OpcI[0] < 1 || OpcI[0] > 5);

switch (OpcI[0])
{
       case 1:
       {
            //Menu CPF
            CPF:
            Erro=0;
            do
            {

            Limpar();
            cout << "--------------------------------------\n";
            cout << "-------------Check Plus---------------\n";
            cout << "------- O Melhor Validador! ----------\n";
            cout << "--------------------------------------\n";
            cout << "----------------CPF------------------\n";
            cout << "Digite uma Opcao:                     \n";  
            cout << " 1 - Verificar\n";
            cout << " 2 - Descobrir Digitos\n";
            cout << " 3 - Gerar\n";
            cout << " 4 - Voltar ao Menu Principal\n";

            Erro++;
            if(Erro > 1)
            {
                    cout << "\nOpcao Invalida, Digite Novamente!\n\n";
            }
            cout << "\nInforme sua Opcao: ";
            OpcC[0] = getche();
            OpcI[0] = atoi (OpcC);
            OpcC[0] = ' ';

            }while (OpcI[0] < 1 || OpcI[0] > 4);
            
            switch (OpcI[0])
            {      
                   case 1:
                        {
                            //Buscar Fun��o CPFVerifica, Pausar e voltar ao menu.
                            CPFVerifica();
                            Pause();
                            goto CPF;
                        }
                   case 2:
                        {
                            //Buscar Fun��o DigCPF, Pausar e voltar ao menu.
                            DigCPF();
                            Pause();
                            goto CPF;
                        }
                   case 3:
                        {
                            // Buscar Fun��o GerarCPF, Pausar e voltar ao menu.
                            GerarCPF();
                            Pause();
                            goto CPF;
                        }
                   case 4:
                        {
                            //Op��o para ir a tela principal do programa.
                            goto inicio;
                        }
            }
       }
       case 2:
       {
            CNPJ:
            Erro=0;
            do
            {

            Limpar();
            cout << "--------------------------------------\n";
            cout << "-------------Check Plus---------------\n";
            cout << "------- O Melhor Validador! ----------\n";
            cout << "--------------------------------------\n";
            cout << "----------------CNPJ------------------\n";
            cout << "Digite uma Opcao:                     \n";  
            cout << " 1 - Verificar\n";
            cout << " 2 - Descobrir Digitos\n";
            cout << " 3 - Gerar\n";
            cout << " 4 - Voltar ao Menu Principal\n";

            Erro++;
            if(Erro > 1)
            {
                    cout << "\nOpcao Invalida, Digite Novamente!\n\n";
            }
            cout << "\nInforme sua Opcao: ";
            OpcC[0] = getche();
            OpcI[0] = atoi (OpcC);
            OpcC[0] = ' ';

            }while (OpcI[0] < 1 || OpcI[0] > 4);
            
            switch (OpcI[0])
            {
                   case 1:
                        {
                            CNPJVerifica();
                            Pause();
                            goto CNPJ;
                        }
                   case 2:
                        {
                            DigCNPJ();
                            Pause();
                            goto CNPJ;
                        }
                   case 3:
                        {
                            GerarCNPJ();
                            Pause();
                            goto CNPJ;
                        }
                   case 4:
                        {
                            goto inicio;
                        }
            }
       }
       case 3:
       {
            TE:
            Erro=0;
            do
            {

            Limpar();
            cout << "--------------------------------------\n";
            cout << "-------------Check Plus---------------\n";
            cout << "------- O Melhor Validador! ----------\n";
            cout << "--------------------------------------\n";
            cout << "---------Titulo de Eleitor------------\n";
            cout << "Digite uma Opcao:                     \n";  
            cout << " 1 - Verificar\n";
            cout << " 2 - Descobrir Digitos\n";
            cout << " 3 - Gerar\n";
            cout << " 4 - Voltar ao Menu Principal\n";

            Erro++;
            if(Erro > 1)
            {
                    cout << "\nOpcao Invalida, Digite Novamente!\n\n";
            }
            cout << "\nInforme sua Opcao: ";
            OpcC[0] = getche();
            OpcI[0] = atoi (OpcC);
            OpcC[0] = ' ';

            }while (OpcI[0] < 1 || OpcI[0] > 4);
            
            switch (OpcI[0])
            {
                   case 1:
                        {
                            TEVerifica();
                            Pause();
                            goto TE;
                        }
                   case 2:
                        {
                            DigTitulo();
                            Pause();
                            goto TE;
                        }
                   case 3:
                        {
                            GerarTitulo();
                            Pause();
                            goto TE;
                        }
                   case 4:
                        {
                            goto inicio;
                        }
            }
       }
       case 4:
       {
            //Chama a Fun��o Cr�ditos, Pausa, e Retorna ao menu principal.
            Creditos();
            Pause();
            goto inicio;
       }

       case 5:
       {
            goto fim;
       }
}

fim:
//Menu de Saida
cout << "\n\nPressione ESC para sair, ou qualquer tecla para voltar. ";
sair = getch();
//Se a op��o for ESC sai do programa, se n�o, volta ao menu principal.
if (sair == 27)
   goto sair;
else
    goto inicio;

sair:

return 0;
}

