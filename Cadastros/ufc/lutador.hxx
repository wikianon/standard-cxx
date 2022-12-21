#ifndef LUTADOR_HXX
#define LUTADOR_HXX

#include <string>

struct Lutador
{
 Lutador( void );
 Lutador( const Lutador&);
 Lutador( std::string, std::string, short, short, short, short, float, float);
 
 virtual ~Lutador( void );
 Lutador& operator=( const Lutador&);
 bool operator==( Lutador&);

 std::string getNome();
 std::string getNacionalidade();
 std::string getCategoria();
 
 short getVitorias();
 short getDerrotas();
 short getEmpates();
 short getIdade();
 float getAltura();
 float getPeso();

 void setNome(std::string);
 void setNacionalidade(std::string);
 void setVitorias(short);
 void setDerrotas(short);
 void setEmpates(short);
 void setIdade(short);
 void setAltura(float);
 void setPeso(float);

 void apresenta();
 void status();
 void ganharLuta();
 void perderLuta();
 void empatarLuta();
 
 private:

 std::string nome;
 std::string nacionalidade;
 short vitorias;
 short derrotas;
 short empates;
 short idade;
 float altura;
 float peso; 
};

#endif