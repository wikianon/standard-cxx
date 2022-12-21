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

#ifndef CCONTATO_H_
#define CCONTATO_H_

#include <string>

using std::string;
/*
 *
 */
class CContato {
private:
	int idContato;
	int cpf;
	string nome;
	string email;
	int telefone;
public:
	void showContato( );
	void setCPF( int cpfTemp );
	void setNome( string nomeTemp );
	void setEmail( string emailTemp );
	void setTelefone( int telTemp );
	int getIdContato( );
	int getCPF( );
	string getNome( );
	string getEmail( );
	int getTelefone( );

	CContato();
	CContato(int idContato, int cpf, string nome, string email, int telefone);
	virtual ~CContato();
};

#endif /* CCONTATO_H_ */
