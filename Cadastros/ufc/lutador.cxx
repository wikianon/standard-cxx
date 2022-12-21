#ifndef LUTADOR_CXX
#define LUTADOR_CXX

#include <iostream>

#include "lutador.hxx"
#include "luta.hxx"

Lutador::Lutador(void):
nome(""),
nacionalidade(""),
vitorias(0),
derrotas(0),
empates(0),
idade(0),
altura(0),
peso(0){}

Lutador::Lutador( const Lutador & obj ):
nome(obj.nome),
nacionalidade(obj.nacionalidade),
vitorias(obj.vitorias),
derrotas(obj.derrotas),
empates(obj.empates),
idade(obj.idade),
altura(obj.altura),
peso(obj.peso){}

Lutador& Lutador::operator=( const Lutador & obj )
{
 this->nome = obj.nome;
 this->nacionalidade = obj.nacionalidade;
 this->vitorias = obj.vitorias;
 this->derrotas = obj.derrotas;
 this->empates = obj.empates;
 this->idade = obj.idade;
 this->altura = obj.altura;
 this->peso = obj.peso;
 
 return *this;
}

bool Lutador::operator==( Lutador &obj )
{
 return ((this->nome != obj.nome) && (this->getCategoria() != obj.getCategoria()));
}

Lutador::~Lutador(void){}

Lutador::Lutador(std::string no, std::string na, short vi, short de,short em,short id, float al, float pe):
nome(no),
nacionalidade(na),
vitorias(vi),
derrotas(de),
empates(em),
idade(id),
altura(al),
peso(pe){}
    
std::string Lutador::getNome()
{
 return nome;
}

std::string Lutador::getNacionalidade()
{
 return nacionalidade;
}

std::string Lutador::getCategoria()
{
 if(peso<=80)
 return "Peso Leve";
 else 
 if(peso<=110)
 return "Peso medio";
 else
 return "Peso pesado";
}

short Lutador::getVitorias()
{
 return vitorias;
}

short Lutador::getDerrotas()
{
 return derrotas;
}

short Lutador::getEmpates()
{
 return empates;
}

short Lutador::getIdade()
{
 return idade;
}

float Lutador::getAltura()
{
 return altura;
}

float Lutador::getPeso()
{
 return peso;
}

void Lutador::setNome(std::string nome)
{
 this->nome=nome;
}

void Lutador::setNacionalidade(std::string nacionalidade)
{
 this->nacionalidade=nacionalidade;
}

void Lutador::setVitorias(short vitorias)
{
 this->vitorias=vitorias;
}

void Lutador::setDerrotas(short derrotas)
{
 this->derrotas=derrotas;
}

void Lutador::setEmpates(short empates)
{
 this->empates=empates;
}

void Lutador::setIdade(short idade)
{
 this->idade=idade;
}

void Lutador::setAltura(float altura)
{
 this->altura=altura;
}

void Lutador::setPeso(float peso)
{
 this->peso=peso;
}

void Lutador::apresenta()
{
 std::cout << "\nNome.............: " << getNome()
           << "\nNacionalidade....: " << getNacionalidade()
           << "\nCategoria........: " << getCategoria()
           << "\nVitorias.........: " << getVitorias()
           << "\nDerrotas.........: " << getDerrotas()
           << "\nEmpates..........: " << getEmpates()
           << "\nIdade............: " << getIdade()
           << "\nAltura...........: " << getAltura()
           << "\nPeso.............: " << getPeso() 
           << "\n\n\n";
}

void Lutador::status()
{
 std::cout << getNome() << "\n"
           << getVitorias() << "\n"
           << getEmpates() << "\n"
           << getDerrotas() << "\n";
}

void Lutador::ganharLuta()
{
 setVitorias(getVitorias()+1);
}

void Lutador::perderLuta()
{
 setDerrotas(getVitorias()+1);
}

void Lutador::empatarLuta()
{
 setEmpates(getEmpates()+1);
}

#endif