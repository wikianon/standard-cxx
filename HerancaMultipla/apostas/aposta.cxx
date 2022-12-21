#include "aposta.hxx"

bool Aposta::contem(int num) const
{
 hash_set<int>::const_iterator it = numeros.find(num);
 
 if (it == numeros.end())
  return false;
 else
  return true;
}

void Aposta::geraAposta(int n)
{
 numeros.clear();
 int i=0;
 
 while ( i < n )
  {
   int x=rand()%49 + 1;
   
   if (! contem(x) )
    {
     numeros.insert(x);
     i++;
    }
  } 
}


void Aposta::imprimeAposta() const
{
 for ( hash_set<int>::const_iterator it = numeros.begin(); it != numeros.end(); it ++ )
  std::cout << *it << " ";
  std::cout << std::endl;
}

int Aposta::calculaCertos(const hash_set<int> &sorteio1) const
{
 int certos = 0;
  for ( hash_set<int>::const_iterator it = sorteio1.begin(); it != sorteio1.end(); it ++ )
   {
    if ( contem(*it) )
     certos++;
   }
 return certos;
}

int Aposta::somaNumeros() const
{
 int soma = 0;
 
 for ( hash_set<int>::const_iterator it = numeros.begin(); it != numeros.end(); it ++ )
  soma +=* it;
 return soma;
}

int Aposta::tamanho() const
{
 return numeros.size();
}


void Jogador::adicionaAposta(const Aposta &aposta1)
{
 apostas.insert(aposta1);
}

int Jogador::apostasNoNumero(int num) const
{
 int count = 0;
 
 for( hashTabAposta::iterator it = apostas.begin(); it != apostas.end(); it++)
  if ((*it).contem(num))
  count++;
 return count;
}

Aposta Jogador::melhorAposta(const Aposta &sorteio1)
{
 Aposta melhorAposta;
 int nCertos, nCertosMax=-1;
 
 for(hashTabAposta::iterator it=apostas.begin();  it!=apostas.end(); it++)
  {
   nCertos=it->calculaCertos(sorteio1.getNumeros());
   
   if(nCertos > nCertosMax)
    {
     nCertosMax=nCertos;
     melhorAposta=*it;
    }
  }
 return melhorAposta;
}