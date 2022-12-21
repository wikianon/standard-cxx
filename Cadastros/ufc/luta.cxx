#ifndef LUTA_CXX
#define LUTA_CXX

#include "luta.hxx"
#include <cstdlib>
#include <iostream>

Luta::Luta(){}

Luta::~Luta(){}

Lutador Luta::getDesafiado()
{
    return Desafiado;
}

Lutador Luta::getDesafiante()
{
    return Desafiante;
}

short Luta::getRounds()
{
    return rounds;
}

bool Luta::getAprovada()
{
    return aprovada;
}

void Luta::setDesafiado(Lutador DO)
{
    Desafiado=DO;
}
void Luta::setDesafiante(Lutador DE)
{
    Desafiante=DE;
}

void Luta::setRounds(short num_rounds)
{
    rounds=num_rounds;
}

void Luta::setAprovada(short YN)
{
 aprovada=YN;
 
 if(aprovada==1)
  std::cout << "Luta aprovada\n";
 else
  std::cout << "Luta não aprovada\n";
}

void Luta::MarcarLuta(Lutador L1, Lutador L2)
{
 if( L1 == L2 )setAprovada(0);
 else
 setAprovada(1);
 setDesafiado(L1);
 setDesafiante(L2);
}

void Luta::lutar()
{
 if(getAprovada()==1)
  {
   Desafiado.apresenta();
   Desafiante.apresenta();
  }
  else
  std::cout << "Impossivel realizar a luta\n\n";

  short vencedor=rand()%700;

  if(vencedor<=200)
   {
    Desafiado.empatarLuta();
    Desafiante.empatarLuta();
   }
   else
  if(vencedor<=400)
   {
    Desafiado.ganharLuta();
    Desafiante.perderLuta();
   }
   else
  if(vencedor<=600)
   {
    Desafiado.perderLuta();
    Desafiante.ganharLuta();
   }
}

#endif