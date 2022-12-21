/*
 * 	   This program is a schedule contact.
 *     Copyright (C) 2011  RCAlves
 * 
 *     This program is free software: you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation, either version 3 of the License, or
 *     (at your option) any later version.
 * 
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 * 
 *     You should have received a copy of the GNU General Public License
 *     along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "CContato.h"
#include <string>
#include <iostream>

using std::string;
using std::cout;
using std::endl;

CContato::CContato() {
	this->cpf = 0;
	this->email = "";
	this->idContato = 0;
	this->nome = "";
	this->telefone = 0;
}

CContato::CContato( int idContato, int cpf, string nome, string email, int telefone ){
	this->idContato = idContato;
	this->cpf = cpf;
	this->telefone = telefone;
	this->nome = nome;
	this->email = email;
}

CContato::~CContato() {
}

int CContato::getCPF( ){
	return this->cpf;
}

int CContato::getIdContato( ){
	return this->idContato;
}

int CContato::getTelefone( ){
	return this->telefone;
}

string CContato::getNome(){
	return this->nome;
}

string CContato::getEmail( ){
	return this->email;
}

void CContato::setCPF( int cpf ){
	this->cpf = cpf;
}

void CContato::setTelefone( int telefone ){
	this->telefone = telefone;
}

void CContato::setNome( string nome ){
	this->nome = nome;
}

void CContato::setEmail( string email ){
	this->email = email;
}

void CContato::showContato( ){
	cout << endl
		 << "ID: " << idContato << endl
		 << "CPF: " << cpf << endl
		 << "Nome: " << nome << endl
		 << "Email: " << email << endl
		 << "Telefone: " << telefone << endl;
}
