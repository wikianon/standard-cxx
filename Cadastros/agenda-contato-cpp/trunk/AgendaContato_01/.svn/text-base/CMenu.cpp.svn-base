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

#include "CMenu.h"
#include <iostream>
#include <vector>
#include <cstdlib>

using std::cout;
using std::cin;
using std::endl;
using std::vector;

CMenu::CMenu( ) {
	this->idContato = 0;
}

CMenu::~CMenu() {
}

void CMenu::showMenu( ){
	int opcao = 0;

	cout << "*********************************" << endl
		 << "* Bem vindo a Agenda de Contato *" << endl
		 << "*********************************" << endl
		 << endl
		 << "Selecione: " << endl
		 << "1 - Incluir Contato" << endl
		 << "2 - Ler Contato" << endl
		 << "3 - Atualizar Contato" << endl
		 << "4 - Deletar Contato" << endl
		 << "0 - Sair" << endl
		 << "Escolha uma opção(0-4): ";

	cin >> opcao;

	switch (opcao) {
	case 0:
		exit(0);
		break;
	case 1:
		this->addContato( );
		break;
	case 2:
		this->readContato( );
		break;
	case 3:
		this->updateContato( );
		break;
	case 4:
		this->deleteContato( );
		break;
	default:
		this->showMenu( );
		break;
	}
}


void CMenu::addContato( ){
	cout << endl << "Adicionando um novo Contato " << endl;

	CContato contato;

	int cpfTemp;
	string nomeTemp;
	string emailTemp;
	int telTemp;


	cout << "Entre com o CPF do contato: ";
	cin >> cpfTemp;

	cin.clear();
	cin.ignore();

	cout << "Entre com o nome do contato: ";
	getline(cin, nomeTemp);

	cout << "Entre com o email do contato: ";
	getline(cin, emailTemp);

	cout << "Entre com o tel do contato: ";
	cin >> telTemp;

	cin.clear();
	cin.ignore();

	cout << endl;

	contato = CContato(idContato++, cpfTemp, nomeTemp, emailTemp, telTemp);

	lista.push_back(contato);

	showMenu();
}

void CMenu::readContato( ){
	cout << endl << "Lendo os Contatos " << endl;

	int tam = lista.size();
	for(int i = 0; i < tam; i++){
		CContato tempCont = lista.at(i);
		tempCont.showContato();
		cout << endl;
	}

	showMenu();
}

void CMenu::updateContato( ){
	cout << endl << "Atualizando um Contato " << endl;

	int updateContato = 0;
	int tam = lista.size();
	cout << "Qual contato atualizar(max: " << tam << "): ";
	cin >> updateContato;

	if(--updateContato <= tam){
		CContato tempCont = lista.at(updateContato);

		string nomeTemp;
		string emailTemp;
		int telTemp;

		cin.clear();
		cin.ignore();

		cout << "Entre com o nome do contato: ";
		getline(cin, nomeTemp);

		cout << "Entre com o email do contato: ";
		getline(cin, emailTemp);

		cout << "Entre com o tel do contato: ";
		cin >> telTemp;

		cin.clear();
		cin.ignore();

		cout << endl;

		tempCont.setNome(nomeTemp);
		tempCont.setEmail(emailTemp);
		tempCont.setTelefone(telTemp);

		lista.erase(lista.begin()+updateContato);
		lista.push_back(tempCont);
	}else{
		cout << "Nada foi atualizado, numero maior que o tamanho da lista!" << endl;
	}

	showMenu();
}

void CMenu::deleteContato( ){
	cout << endl << "Deletando um Contato " << endl;

	int delContato = 0;
	int tam = lista.size();
	cout << "Qual contato excluir(max: " << tam << "): ";
	cin >> delContato;

	if(--delContato <= tam){
		lista.erase(lista.begin()+delContato);
	}else{
		cout << "Nada foi deletado, numero maior que o tamanho da lista!" << endl;
	}

	showMenu();
}
