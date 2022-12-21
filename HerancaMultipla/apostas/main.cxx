#include "aposta.hxx"







int main()
{
 Aposta a1;
 Jogador j1;
 int nApostas;

 std::cout << "Numero de apostas? ";
 std::cin >> nApostas;
 
 j1 = Jogador("Rui");
 
 // gera apostas e adiciona-as ao jogador
 std::cout << "A gerar apostas\n";		
 
 for(int i = 0; i < nApostas; i++)
  {
   a1.geraAposta();
   a1.imprimeAposta();
   j1.adicionaAposta(a1);	
  }
  
 std::cout << std::endl;

 // gera um sorteio (conjunto de 6 números diferentes)
 Aposta sorteio1;
 sorteio1.geraAposta(6);
 std::cout << "Numeros sorteados: ";
 sorteio1.imprimeAposta();
 
 // determina a melhor aposta
 Aposta apostaM=j1.melhorAposta(sorteio1);
 std::cout << "Melhor Aposta: ";
 apostaM.imprimeAposta();
 
 return 0;
}