#ifndef LUTA_HXX
#define LUTA_HXX

#include "lutador.hxx"



struct Luta
{
 Luta();
 virtual ~Luta();

 Lutador getDesafiado();
 Lutador getDesafiante();
 short getRounds();
 bool getAprovada();

 void setDesafiado(Lutador DO);
 void setDesafiante(Lutador DE);
 void setRounds(short num_rounds);
 void setAprovada(short YN);

 void MarcarLuta(Lutador L1, Lutador L2);
 void lutar();

 private:

 Lutador Desafiado;
 Lutador Desafiante;
 short rounds;
 bool aprovada;
};











#endif