#include "libmenu.hxx"



























int main()
{
 int op;
 int tam=4;
 
 const char* opcoes1[tam]={"Novo","Consultar","Listar","Sair"};
 //menu(tam, opcoes1); // aceita limite maximo de até 15 opçoes
 menu(tam, tam, opcoes1); // aceita limite igual ao tamanho do vetor
 
 std::string opcoes2[tam]={"Novo","Consultar","Listar","Sair"};
 //menu(tam, opcoes2); // aceita limite maximo de até 15 opçoes
 menu(tam, tam, opcoes2); // aceita limite igual ao tamanho do vetor
 
 std::cin>>op;
 return 0;
}