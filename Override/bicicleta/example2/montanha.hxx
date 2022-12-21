#include "bicicleta.hxx"

struct Montanha: Bicicleta
{
   Montanha(){} 
   std::string marchas;
   std::string amortecedores;
   std::string garrafa;
   std::string gps;
 
 //Metodo Override
  void bicicleta()
  {
   std::cout << "\n\tBicicleta.....: " + modelo
             << "\n\tQuadro........: " + quadro
             << "\n\tRodas.........: " + rodas
             << "\n\tFreios........: " + freios
             << "\n\tMarchas.......: " + marchas
             << "\n\tAmortecedores.: " + freios
             << "\n\tGarrafa.......: " + garrafa
             << "\n\tGps...........: " + gps
             << "\n\n";     
  }
};