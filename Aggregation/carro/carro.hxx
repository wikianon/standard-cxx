#ifndef _CARRO_HXX
#define _CARRO_HXX

#if defined(__linux__) || defined(__gnu_linux__)

 #include <stdio_ext.h>

#else

 #include <stdio.h>

#endif

void cbuff(void)
{
#if defined(_WIN32) || defined(_WIN64) || defined(__WINDOWS__)

 fflush(stdin);

#elif defined(__linux__) || defined(__gnu_linux__)

 __fpurge(stdin);

#else

 int ch;
 while((ch=getchar()) != EOF && ch != '\n');

#endif
}

#include "marca.hxx"
#include "modelo.hxx"
#include "motor.hxx"

struct Carro
{
 Carro(){}
 ~Carro(){}
 
  int idCarro;
  std::string descriCarro;
  
  Marca marca;
  Modelo modelo;
  Motor motor;
  
 std::string imprimeDadosCarro()
 {
  return "\n\n\tID. DO CARRO..........: " + std::to_string(idCarro) +
         "\n\n\tDESCRI. DO CARRO......: " + descriCarro +
         "\n\n\tID. DA MARCA..........: " + std::to_string(marca.idMarca) +
         "\n\n\tDESCRI. DA MARCA......: " + marca.descriMarca +
         "\n\n\tID. DO MODELO.........: " + std::to_string(modelo.idModelo) +
         "\n\n\tDESCRI. DO MODELO.....: " + modelo.descriModelo +
         "\n\n\tID. DO MOTOR..........: " + std::to_string(motor.idMotor) +
         "\n\n\tDESCRI. DO MOTOR......: " + motor.descriMotor +
         "\n\n\tDESCRI. DA POT. MOTOR.: " + motor.descriPotencia;
 }
 
};

#endif