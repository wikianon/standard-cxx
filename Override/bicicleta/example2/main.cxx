#include "montanha.hxx"







int main(void)
{
  // Metodo com Construtor Default
  Bicicleta bike1;
  bike1.modelo = "ZR069";
  bike1.quadro = "Ferro";
  bike1.rodas = "Ferro";
  bike1.freios = "Pastilhas";
  
  std::cout<<"\n\tMetodo com Construtor Default\n";
  bike1.bicicleta();
 
  
  // Construtor Override
  Bicicleta bike2("XA 200", "Aluminio", "Pastilhas", "Aluminio");
  std::cout<<"\n\tMetodo com Construtor Override\n";
  bike2.bicicleta();
 
  // Herança - Montain Bike
  Montanha  bike3;
  bike3.modelo = "Attack 500";
  bike3.quadro = "Fibra de Carbono";
  bike3.rodas = "Aluminio";
  bike3.freios = "Disco";
  bike3.amortecedores = "R3000";
  bike3.garrafa = "Aluminio 500ml";
  bike3.marchas = "18 Velocidades";
  bike3.gps = "Discovery 2.5";
 
  // Metodo Overload e Override
  bike3.bicicleta();
  
  return 0;
}