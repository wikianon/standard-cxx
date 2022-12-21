#include <iostream>




//busca binaria
int main(void)
{
 int x[10], i, n, achou, inicio, fim, meio;

 for(i=0; i<10; i++)
  {
   std::cout << "\n\tDigite um numero para a posicao " << i+1 << "º: ";
   std::cin >> x[i];
  }

 std::cout << "\n\tDigite o numero a ser buscado no vetor: ";
 std::cin >> n;

 achou = 0;
 inicio = 0;
 fim = 9;
 meio = (inicio+fim)/2;

 while(inicio <= fim && achou == 0)
  {
   if(x[meio] == n)
    achou = 1;
   else
   {
    if(n < x[meio])
     fim = meio-1;
    else
     inicio = meio+1;
     meio = (inicio+fim)/2;
   }
  }

 if(achou == 0)
  std::cout << "\n\tNumero nao encontrado no vetor!\n\n";
 else
  std::cout << "\n\tNumero encontrado na posicao " << meio+1 << "\n\n";
 return 0;
} 
