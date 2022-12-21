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
#include <string>

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::string;

CMenu::CMenu( ) {
	this->idContato = 0;
}

CMenu::~CMenu() {
}

void CMenu::showMenu( ){
//	system( "cls" ); Descomente caso execute o exe direto no windows e nao no eclipse, este comando limpa a tela!

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

/*****************************************************************/
/********************** Inicio do metodo add *********************/
/*****************************************************************/
void CMenu::addContato( ){
	cout << endl
	     << "*********************" << endl
		 << "* Adicionar Contato *" << endl
		 << "*********************" << endl << endl;

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
/*****************************************************************/
/************************* Fim do metodo add *********************/
/*****************************************************************/


/*****************************************************************/
/******************** Inicio dos metodos read ********************/
/*****************************************************************/
void CMenu::readContato( ){
	cin.clear();
	cin.ignore();

	int opcao = 0;

	cout << endl
		 << "****************" << endl
		 << "* Ler Contatos *" << endl
		 << "****************" << endl << endl
		 << "Selecione: " << endl
		 << "0 - Voltar ao menu Principal" << endl
		 << "1 - Por CPF Contato" << endl
		 << "2 - Por Nome Contato" << endl
		 << "3 - Por Telefone Contato" << endl
		 << "4 - Todos" << endl
		 << "Escolha uma opcao(0-4): ";

	cin >> opcao;

	switch (opcao) {
	case 0:
		showMenu();
		break;
	case 1:
		readCPFContato();
		break;
	case 2:
		readNomeContato();
		break;
	case 3:
		readTelefoneContato();
		break;
	case 4:
		readTodos();
		break;
	default:
		showMenu();
		break;
	}
}

void CMenu::readCPFContato( ){
	cin.clear();
	cin.ignore();

	int opcao = 0;

	cout << endl
		 << "**********************" << endl
		 << "* Ler Contato por CPF *" << endl
		 << "**********************" << endl << endl
		 << "Selecione: " << endl
		 << "0 - Voltar ao menu Ler Contatos" << endl
		 << "1 - Inserir CPF Contato" << endl
		 << "Escolha uma opcao(0-1): ";

	cin >> opcao;

	int cpfContato = 0;
	if( opcao == 0 ){
		readContato();
	}
	else {
		cout << endl << "Insira o cpf do contato: ";
		cin >> cpfContato;
	}

	int totalContatos = lista.size();
	CContato contatoTemp;
	for (int i = 0; i < totalContatos; ++i) {
		contatoTemp = lista[i];
		if(contatoTemp.getCPF() == cpfContato){
			contatoTemp.showContato();
		}
	}

	cout << endl;
	system( "pause" );
	showMenu();
}

void CMenu::readNomeContato( ){
	cin.clear();
	cin.ignore();

	int opcao = 0;

	cout << endl
		 << "************************" << endl
		 << "* Ler Contato por Nome *" << endl
		 << "************************" << endl << endl
		 << "Selecione: " << endl
		 << "0 - Voltar ao menu Ler Contatos" << endl
		 << "1 - Inserir Nome Contato" << endl
		 << "Escolha uma opcao(0-1): ";

	cin >> opcao;

	string nomeContato;
	if( opcao == 0 ){
		readContato();
	}
	else {
		cin.clear();
		cin.ignore();

		cout << endl << "Insira o nome do contato: ";
		getline(cin, nomeContato);
	}

	int totalContatos = lista.size();
	CContato contatoTemp;
	for (int i = 0; i < totalContatos; ++i) {
		contatoTemp = lista[i];
		if(contatoTemp.getNome() == nomeContato){
			contatoTemp.showContato();
		}
	}

	cout << endl;
	system( "pause" );
	showMenu();
}

void CMenu::readTelefoneContato( ){
	cin.clear();
	cin.ignore();

	int opcao = 0;

	cout << endl
		 << "****************************" << endl
		 << "* Ler Contato por Telefone *" << endl
		 << "****************************" << endl << endl
		 << "Selecione: " << endl
		 << "0 - Voltar ao menu Ler Contatos" << endl
		 << "1 - Inserir Telefone Contato" << endl
		 << "Escolha uma opcao(0-1): ";

	cin >> opcao;

	int telefoneContato = 0;
	if( opcao == 0 ){
		readContato();
	}
	else {
		cout << endl << "Insira o Telefone do contato: ";
		cin >> telefoneContato;
	}

	int totalContatos = lista.size();
	CContato contatoTemp;
	for (int i = 0; i < totalContatos; ++i) {
		contatoTemp = lista[i];
		if(contatoTemp.getTelefone() == telefoneContato){
			contatoTemp.showContato();
		}
	}

	cout << endl;
	system( "pause" );
	showMenu();
}

void CMenu::readTodos( ){
	cin.clear();
	cin.ignore();

	int opcao = 0;

	cout << endl
		 << "**********************" << endl
		 << "* Ler Todos Contatos *" << endl
		 << "**********************" << endl << endl
		 << "Selecione: " << endl
		 << "0 - Voltar ao menu Ler Contatos" << endl
		 << "1 - Ler Todos os Contatos" << endl
		 << "Escolha uma opcao(0-1): ";

	cin >> opcao;

	if( opcao == 0 ){
		readContato();
	}
	else {
		int totalContatos = lista.size();
		CContato contatoTemp;
		for (int i = 0; i < totalContatos; ++i) {
			contatoTemp = lista[i];
			contatoTemp.showContato();
			cout << endl;
			system( "pause" );
			cin.clear();
			cin.ignore();
		}
	}

	cout << endl;
	showMenu();
}
/*****************************************************************/
/*********************** Fim dos metodos read ********************/
/*****************************************************************/


/*****************************************************************/
/******************** Inicio dos metodos update ******************/
/*****************************************************************/
void CMenu::updateContato( ){
	cin.clear();
	cin.ignore();

	int opcao = 0;

	cout << endl
			<< "**********************" << endl
			<< "* Atualizar Contatos *" << endl
			<< "**********************" << endl << endl
			<< "Selecione: " << endl
			<< "0 - Voltar ao menu Principal" << endl
			<< "1 - Por CPF Contato" << endl
			<< "2 - Por Nome Contato" << endl
			<< "3 - Por Telefone Contato" << endl
			<< "4 - Todos" << endl
			<< "Escolha uma opcao(0-4): ";

	cin >> opcao;

	switch (opcao) {
	case 0:
		showMenu();
		break;
	case 1:
		updateCPFContato();
		break;
	case 2:
		updateNomeContato();
		break;
	case 3:
		updateTelefoneContato();
		break;
	case 4:
		updateTodos();
		break;
	default:
		showMenu();
		break;
	}
}

void CMenu::updateCPFContato( ){
	cin.clear();
	cin.ignore();

	int opcao = 0;

	cout << endl
		 << "*****************************" << endl
		 << "* Atualizar Contato por CPF *" << endl
		 << "*****************************" << endl << endl
		 << "Selecione: " << endl
		 << "0 - Voltar ao menu Atualizar Contatos" << endl
		 << "1 - Inserir CPF Contato" << endl
		 << "Escolha uma opcao(0-1): ";

	cin >> opcao;

	int cpfContato = 0;
	if( opcao == 0 ){
		updateContato();
	}
	else {
		cout << endl << "Insira o cpf do contato: ";
		cin >> cpfContato;
	}

	int atualizaContato = -1;
	int totalContatos = lista.size();
	CContato contatoTemp;
	for (int i = 0; i < totalContatos; ++i) {
		contatoTemp = lista[i];
		if(contatoTemp.getCPF() == cpfContato){
			atualizaContato = i;
			break;
		}
	}

	if(atualizaContato > -1 ){
		CContato tempCont = lista.at(atualizaContato);

		int cpfTemp;
		string nomeTemp;
		string emailTemp;
		int telTemp;

		cin.clear();
		cin.ignore();

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

		tempCont.setCPF(cpfTemp);
		tempCont.setNome(nomeTemp);
		tempCont.setEmail(emailTemp);
		tempCont.setTelefone(telTemp);

		lista.erase(lista.begin()+atualizaContato);
		lista.insert(lista.begin()+atualizaContato,tempCont);
	}else if(atualizaContato == -1){
		cout << "Nada foi atualizado, contato não encontrado na lista!" << endl;
	}else{
		cout << endl << "Contato atualizado com sucesso!!!" << endl;
		system( "pause" );
		showMenu();
	}
}

void CMenu::updateNomeContato( ){
	cin.clear();
	cin.ignore();

	int opcao = 0;

	cout << endl
		 << "******************************" << endl
		 << "* Atualizar Contato por Nome *" << endl
		 << "******************************" << endl << endl
		 << "Selecione: " << endl
		 << "0 - Voltar ao menu Atualizar Contatos" << endl
		 << "1 - Inserir Nome Contato" << endl
		 << "Escolha uma opcao(0-1): ";

	cin >> opcao;

	string nomeContato;
	if( opcao == 0 ){
		updateContato();
	}
	else {
		cin.clear();
		cin.ignore();

		cout << endl << "Insira o nome do contato: ";
		getline(cin, nomeContato);
	}

	int atualizaContato = -1;
	int totalContatos = lista.size();
	CContato contatoTemp;
	for (int i = 0; i < totalContatos; ++i) {
		contatoTemp = lista[i];
		if(contatoTemp.getNome() == nomeContato){
			atualizaContato = i;
			break;
		}
	}

	if(atualizaContato > -1 ){
		CContato tempCont = lista.at(atualizaContato);

		int cpfTemp;
		string nomeTemp;
		string emailTemp;
		int telTemp;

		cin.clear();
		cin.ignore();

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

		tempCont.setCPF(cpfTemp);
		tempCont.setNome(nomeTemp);
		tempCont.setEmail(emailTemp);
		tempCont.setTelefone(telTemp);

		lista.erase(lista.begin()+atualizaContato);
		lista.insert(lista.begin()+atualizaContato,tempCont);
	}else if(atualizaContato == -1){
		cout << "Nada foi atualizado, contato não encontrado na lista!" << endl;
	}else{
		cout << endl << "Contato atualizado com sucesso!!!" << endl;
		system( "pause" );
		showMenu();
	}
}

void CMenu::updateTelefoneContato( ){
	cin.clear();
	cin.ignore();

	int opcao = 0;

	cout << endl
		 << "**********************************" << endl
		 << "* Atualizar Contato por Telefone *" << endl
		 << "**********************************" << endl << endl
		 << "Selecione: " << endl
		 << "0 - Voltar ao menu Atualizar Contatos" << endl
		 << "1 - Inserir Telefone Contato" << endl
		 << "Escolha uma opcao(0-1): ";

	cin >> opcao;

	int telefoneContato = 0;
	if( opcao == 0 ){
		updateContato();
	}
	else {
		cout << endl << "Insira o Telefone do contato: ";
		cin >> telefoneContato;
	}

	int atualizaContato = -1;
	int totalContatos = lista.size();
	CContato contatoTemp;
	for (int i = 0; i < totalContatos; ++i) {
		contatoTemp = lista[i];
		if(contatoTemp.getTelefone() == telefoneContato){
			atualizaContato = i;
			break;
		}
	}

	if(atualizaContato > -1 ){
		CContato tempCont = lista.at(atualizaContato);

		int cpfTemp;
		string nomeTemp;
		string emailTemp;
		int telTemp;

		cin.clear();
		cin.ignore();

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

		tempCont.setCPF(cpfTemp);
		tempCont.setNome(nomeTemp);
		tempCont.setEmail(emailTemp);
		tempCont.setTelefone(telTemp);

		lista.erase(lista.begin()+atualizaContato);
		lista.insert(lista.begin()+atualizaContato,tempCont);
	}else if(atualizaContato == -1){
		cout << "Nada foi atualizado, contato não encontrado na lista!" << endl;
	}else{
		cout << endl << "Contato atualizado com sucesso!!!" << endl;
		system( "pause" );
		showMenu();
	}
}

void CMenu::updateTodos( ){
	cin.clear();
	cin.ignore();

	int opcao = 0;

	cout << endl
		 << "****************************" << endl
		 << "* Atualizar Todos Contatos *" << endl
		 << "****************************" << endl << endl
		 << "Selecione: " << endl
		 << "0 - Voltar ao menu Atualizar Contatos" << endl
		 << "1 - Atualizar Todos os Contatos" << endl
		 << "Escolha uma opcao(0-1): ";

	cin >> opcao;

	if( opcao == 0 ){
		updateContato();
	}
	else {
		int totalContatos = lista.size();
		for (int i = 0; i < totalContatos; ++i) {
			CContato tempCont = lista.at(i);

			int cpfTemp;
			string nomeTemp;
			string emailTemp;
			int telTemp;

			cin.clear();
			cin.ignore();

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

			tempCont.setCPF(cpfTemp);
			tempCont.setNome(nomeTemp);
			tempCont.setEmail(emailTemp);
			tempCont.setTelefone(telTemp);

			lista.erase(lista.begin()+i);
			lista.insert(lista.begin()+i,tempCont);

			system( "pause" );
		}

		cout << endl << "Contatos atualizados com sucesso!!!" << endl;
		system( "pause" );
	}
	showMenu();
}
/*****************************************************************/
/*********************** Fim dos metodos update ******************/
/*****************************************************************/

/*****************************************************************/
/******************** Inicio dos metodos delete ******************/
/*****************************************************************/
void CMenu::deleteContato( ){
	cin.clear();
	cin.ignore();

	int opcao = 0;

	cout << endl
			<< "********************" << endl
			<< "* Deletar Contatos *" << endl
			<< "********************" << endl << endl
			<< "Selecione: " << endl
			<< "0 - Voltar ao menu Principal" << endl
			<< "1 - Por CPF Contato" << endl
			<< "2 - Por Nome Contato" << endl
			<< "3 - Por Telefone Contato" << endl
			<< "4 - Todos" << endl
			<< "Escolha uma opcao(0-4): ";

	cin >> opcao;

	switch (opcao) {
	case 0:
		showMenu();
		break;
	case 1:
		deleteCPFContato();
		break;
	case 2:
		deleteNomeContato();
		break;
	case 3:
		deleteTelefoneContato();
		break;
	case 4:
		deleteTodos();
		break;
	default:
		showMenu();
		break;
	}
}

void CMenu::deleteCPFContato( ){
	cin.clear();
	cin.ignore();

	int opcao = 0;

	cout << endl
		 << "***************************" << endl
		 << "* Deletar Contato por CPF *" << endl
		 << "***************************" << endl << endl
		 << "Selecione: " << endl
		 << "0 - Voltar ao menu Deletar Contato" << endl
		 << "1 - Inserir CPF Contato" << endl
		 << "Escolha uma opcao(0-1): ";

	cin >> opcao;

	int cpfContato = 0;
	if( opcao == 0 ){
		deleteContato();
	}
	else {
		cout << endl << "Insira o cpf do contato: ";
		cin >> cpfContato;
	}

	int deletaContato = -1;
	int totalContatos = lista.size();
	CContato contatoTemp;
	for (int i = 0; i < totalContatos; ++i) {
		contatoTemp = lista[i];
		if(contatoTemp.getCPF() == cpfContato){
			deletaContato = i;
			break;
		}
	}

	if(deletaContato > -1 ){
		lista.erase(lista.begin()+deletaContato);
	}else if(deletaContato == -1){
		cout << "Nada foi deletado, contato não encontrado na lista!" << endl;
	}else{
		cout << endl << "Contato deletado com Sucesso!!!" << endl;
		system( "pause" );
		showMenu();
	}
}

void CMenu::deleteNomeContato( ){
	cin.clear();
	cin.ignore();

	int opcao = 0;

	cout << endl
		 << "****************************" << endl
		 << "* Deletar Contato por Nome *" << endl
		 << "****************************" << endl << endl
		 << "Selecione: " << endl
		 << "0 - Voltar ao menu Deletar Contato" << endl
		 << "1 - Inserir Nome Contato" << endl
		 << "Escolha uma opcao(0-1): ";

	cin >> opcao;

	string nomeContato;
	if( opcao == 0 ){
		deleteContato();
	}
	else {
		cin.clear();
		cin.ignore();

		cout << endl << "Insira o nome do contato: ";
		getline(cin, nomeContato);
	}

	int deletaContato = -1;
	int totalContatos = lista.size();
	CContato contatoTemp;
	for (int i = 0; i < totalContatos; ++i) {
		contatoTemp = lista[i];
		if(contatoTemp.getNome() == nomeContato){
			deletaContato = i;
			break;
		}
	}

	if(deletaContato > -1 ){
		lista.erase(lista.begin()+deletaContato);
	}else if(deletaContato == -1){
		cout << "Nada foi deletado, contato não encontrado na lista!" << endl;
	}else{
		cout << endl << "Contato deletado com Sucesso!!!" << endl;
		system( "pause" );
		showMenu();
	}
}

void CMenu::deleteTelefoneContato( ){
	cin.clear();
	cin.ignore();

	int opcao = 0;

	cout << endl
		 << "********************************" << endl
		 << "* Deletar Contato por Telefone *" << endl
		 << "********************************" << endl << endl
		 << "Selecione: " << endl
		 << "0 - Voltar ao menu Deletar Contato" << endl
		 << "1 - Inserir Telefone Contato" << endl
		 << "Escolha uma opcao(0-1): ";

	cin >> opcao;

	int telefoneContato = 0;
	if( opcao == 0 ){
		deleteContato();
	}
	else {
		cout << endl << "Insira o Telefone do contato: ";
		cin >> telefoneContato;
	}

	int deletaContato = -1;
	int totalContatos = lista.size();
	CContato contatoTemp;
	for (int i = 0; i < totalContatos; ++i) {
		contatoTemp = lista[i];
		if(contatoTemp.getTelefone() == telefoneContato){
			deletaContato = i;
			break;
		}
	}

	if(deletaContato > -1 ){
		lista.erase(lista.begin()+deletaContato);
	}else if(deletaContato == -1){
		cout << "Nada foi deletado, contato não encontrado na lista!" << endl;
	}else{
		cout << endl << "Contato deletado com Sucesso!!!" << endl;
		system( "pause" );
		showMenu();
	}
}

void CMenu::deleteTodos( ){
	cin.clear();
	cin.ignore();

	int opcao = 0;

	cout << endl
		 << "**************************" << endl
		 << "* Deletar Todos Contatos *" << endl
		 << "**************************" << endl << endl
		 << "Selecione: " << endl
		 << "0 - Voltar ao menu Deletar Contato" << endl
		 << "1 - Deletar Todos os Contatos" << endl
		 << "Escolha uma opcao(0-1): ";

	cin >> opcao;

	if( opcao == 0 ){
		deleteContato();
	}
	else {
		int totalContatos = lista.size();
		for (int i = 0; i < totalContatos; ++i) {
			lista.erase(lista.begin()+i);
		}
		cout << endl << "Contatos deletados com Sucesso!!!" << endl;
	}

	system( "pause" );
	showMenu();
}

/*****************************************************************/
/*********************** Fim dos metodos delete ******************/
/*****************************************************************/
