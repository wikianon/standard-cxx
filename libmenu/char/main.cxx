#include "libmenu.hxx"

































int main()
{
 int op;
 int tam=4;
 const char* opcoes[tam]={"Novo","Consultar","Listar","Sair"};
 //menu(tam, opcoes); // aceita limite maximo de até 15 opçoes
 menu(tam, tam, opcoes); // aceita limite igual ao tamanho do vetor
 std::cin>>op;
 return 0;
}