/****************************************************************************
**
** http://sourceforge.net/projects/easy-crud/
**
** Copyright (C) 2010, 2011, 2012  Luis Valdes (luisvaldes88@gmail.com)
**
** This file is part of the EZCRUD library
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**
****************************************************************************/

// ****************************************************
// * Codigo Generado por DAOBuilder                   *
// * Fecha: 28/09/2012 15:20:22                       *
// * Luis Valdes 2012 - Todos los Derechos Reservados *
// ****************************************************
#include "categoria.h"
// Include Objects
#include "persona_categoria.h"
// Include DAOs
#include "../dao/persona_categoriadao.h"

namespace crud {

Categoria::Categoria(QObject *parent) :
	ObjectBase(parent)
{

}

Categoria::Categoria(const QSqlRecord &record, QObject *parent) :
	ObjectBase(parent)
{
	// id, nombre
	setId(record.value(0));
	setNombre(record.value(1));
}

// Getters y Setters Definitions
QVariant Categoria::nombre() const
{
	return m_nombre;
}

void Categoria::setNombre(QVariant value)
{
	m_nombre = value.toString();
}

// Objects Getters y Setters Definitions
QList<PersonaCategoria *> & Categoria::persona_categoriaList()
{
	if (m_persona_categoriaList.count() == 0)
	{
		//Persona_categoriaDAO::instance()->getByCategoria(m_persona_categoriaList);
	}
	return m_persona_categoriaList;
}

void Categoria::setPersona_categoriaList(QList<PersonaCategoria *> &objList)
{
	m_persona_categoriaList = objList;
}

} // namespace crud
