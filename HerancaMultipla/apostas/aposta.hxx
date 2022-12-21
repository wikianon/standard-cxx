#ifndef APOSTA_HXX_
#define APOSTA_HXX_

#include <ext/hash_set>
using __gnu_cxx::hash_set;

#include <string>
#include <iostream>

class Aposta
{
 hash_set<int> numeros;
 
 public:
  void geraAposta(int n=6);
  bool contem(int num) const;
  int calculaCertos(const hash_set<int> &sorteio1) const;
  void imprimeAposta() const;
  int somaNumeros() const;	
  int tamanho() const;	
  hash_set<int> getNumeros() const { return numeros; }	
};

struct hashAposta
{
 int operator() (const Aposta & ap) const
  {
   return ap.somaNumeros();
  }
};

struct eqAposta
{
 bool operator() (const Aposta & ap1, const Aposta & ap2) const
 {
  if( ap1.tamanho() != ap2.tamanho() )
   return false;
  
  int n = ap1.calculaCertos(ap2.getNumeros());
  
  if( n == ap1.tamanho() )
   return true;
    else
     return false;
 }
};

typedef hash_set<Aposta, hashAposta, eqAposta> hashTabAposta;

class Jogador
{
 hashTabAposta apostas;
 std::string nome;
 
 public:
  Jogador(std::string nm=""):nome(nm)  {};
  void adicionaAposta(const Aposta &aposta1);
  int apostasNoNumero(int num) const;
  Aposta melhorAposta(const Aposta &sorteio1);
};

#endif /*APOSTA_HXX_*/