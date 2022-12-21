#include <iostream>

int main ()
{
    int Qtde, i, Media;
    double x=0;
    int Mat1[100];
    int Mat2[100];
    int Mat3[100];
    int Mat4[100];
    int Mat5[100];
    char Nome[100][100];

    //Usuário informa a quantidade de alunos na turma
    std::cout << "\n\tDigite a quantidade de alunos na turma: ";
    std::cin >> Qtde;

    std::cout << "\n\tDigite a média adotada na escola. Ex: 60: ";
    std::cin >> Media;

    //Entra na repetição para o usuário entrar a nota de todos os alunos
    for(i=1;i<Qtde+1;i++)
    {
        std::cout << "\n\tDigite o nome do aluno " << i << ": ";
        std::cin >> Nome[i];

        std::cout << "\n\tDigite a nota do aluno na matéria 1: ";
        std::cin >> Mat1[i];

        std::cout << "\n\tDigite a nota do aluno na matéria 2: ";
        std::cin >> Mat2[i];

        std::cout << "\n\tDigite a nota do aluno na matéria 3: ";
        std::cin >> Mat3[i];

        std::cout << "\n\tDigite a nota do aluno na matéria 4: ";
        std::cin >> Mat4[i];

        std::cout << "\n\tDigite a nota do aluno na matéria 5: ";
        std::cin >> Mat5[i];
    }
    std::cout << "\n\tObrigado! Você já informou todas as notas\n\n";


    //Escrever alunos com média em todas as matérias:

    std::cout << "\n\tAlunos com nota maior ou igual a " << Media << " em todas as matérias:\n\n";

    for(i=1;i<Qtde+1;i++)
    {
        if(Mat1[i]>=Media && Mat2[i]>=Media && Mat3[i]>=Media && Mat4[i]>=Media && Mat5[i]>=Media)
        {
            std::cout << "\tO aluno: " << Nome[i] << " foi aprovado em todas as matérias.\n\n";
        }
    }


    //Escrever alunos aprovados nas matérias 1 e 4:

    std::cout << "\tAlunos com nota maior ou igual a " << Media << " em todas as matérias:\n\n";

    for(i=1;i<Qtde+1;i++)
    {
        if(Mat1[i]>=Media && Mat4[i]>=Media)
        {
            std::cout << "\tO aluno: " << Nome[i] << " foi aprovado nas matérias 1 e 4.\n\n";
        }
    }
    //Escrever a porcentagem de aprovados na matéria 3:

    std::cout << "\tAlunos com nota maior ou igual a " << Media << " na matéria 3 em porcentagem:\n\n";

    for(i=1;i<Qtde+1;i++)
    {
        if(Mat3[i]>=Media)
        {
            x++;
        }
    }

    x=x/Qtde;
    x=x*100;

    std::cout<< "\tA porcentagem de alunos aprovados na matéria 3 e de: " << x << " .\n\n\tObrigado!!!\n\n";
}