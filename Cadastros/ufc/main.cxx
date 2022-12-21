#ifndef MAIN_CXX
#define MAIN_CXX

#include "luta.hxx"
#include "lutador.hxx"




















int main(void)
{
 Lutador Pretty_Boy("Pretty Boy","França",11,2,1,31,1.75,68.9);
 Lutador Putscript("Putscript","Brasil",14,2,3,29,1.68,57.8);
 Lutador Snapshadow("Snapshadow","EUA",12,2,1,35,1.65,80.9);
 Lutador Deat_Code("Dead Code","Austrália",13,0,2,28,1.93,81.6);
 Lutador Ufocobol("Ufocobol","Brasil",5,4,3,37,1.70,119.3);
 Lutador Nerdaard("Nerdaard","EUA",12,2,4,30,1.81,105.7);
    
 Luta UEC01;
    
 UEC01.MarcarLuta( Pretty_Boy, Putscript );
    
 UEC01.lutar();

 return 0;
} 

#endif