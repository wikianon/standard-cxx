#include <fstream>
#include <cstdlib>
#include <iomanip>
#include <iostream>

struct funcionario // estrutura do registro de funcionário
{
 int matricula;
 std::string nome;
 std::string cargo;
 std::string local;
 int escolaridade;
 char sexo;
 char ecivil; // estado civil
 float salario;
};

int main()
{
 funcionario f;
 char sexo;
 std::ifstream funcFile("func.txt");
 
 if(!funcFile)
  {
   std::cout << "Arquivo func.dat nao pode ser aberto\n\n";
   abort();
  }
 std::cout << "\n\nSexo dos funcionarios a serem listados (M ou F) Ctrl-Z termina?";
 
 while (std::cin >> sexo)
 { 
  std::cout << "Consulta funcionario com sexo: " << sexo << "\n\n"
            << std::setiosflags(std::ios::left)
            << std::setw(12) << "Matricula"
            << std::setw(10) << "Nome"
            << std::setw(14) << "Cargo"
            << std::setw(7)  << "Esc."
            << std::setw(7)  << "Sexo"
            << std::setw(10) << "Local"
            << std::setw(10) << "E.C."
            << std::setw(8)  << "Salario\n";
     
 funcFile.clear(); // limpa "eof = final de arquivo" para proximo uso
 funcFile.seekg(0); // posiciona no inicio do arquivo
 
  funcFile >> f.matricula >> f.nome 
           >> f.cargo >> f.escolaridade
           >> f.sexo >> f.local 
           >> f.ecivil >> f.salario; // ler primeiro registro
 
  while (!funcFile.eof())
  {
   if(f.sexo == sexo) // o registro atende a condição de consulta?
   std::cout << setiosflags(std::ios::left)
             << std::setw(12) << f.matricula
             << std::setw(10) << f.nome
             << std::setw(14) << f.cargo
             << std::setw(7)  << f.escolaridade
             << std::setw(7)  << f.sexo
             << std::setw(10) << f.local
             << std::setw(10) << f.ecivil
             << std::setw(8)  << f.salario 
             <<"\n";
   funcFile >> f.matricula >> f.nome 
            >> f.cargo >> f.escolaridade
            >> f.sexo >> f.local 
            >> f.ecivil >> f.salario; // ler próximo registro
  }
  
 std::cout << "\n\nSexo dos funcionarios a serem listados (M ou F) Ctrl-Z termina? ";
 }
 return 0;
}