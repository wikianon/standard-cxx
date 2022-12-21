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
#include "persona_categoria.h"
// Include Objects
#include "categoria.h"
// Include DAOs
#include "../dao/categoriadao.h"

namespace crud {

PersonaCategoria::PersonaCategoria(QObject *parent) :
	QObject(parent)
{

}

PersonaCategoria::PersonaCategoria(const QSqlRecord &record, QObject *parent) :
	QObject(parent)
{
	// idpersona, idcategoria
	setIdpersona(record.value(0));
	setIdcategoria(record.value(1));
}

// Getters y Setters Definitions
QVariant PersonaCategoria::idpersona() const
{
	return m_idpersona;
}

void PersonaCategoria::setIdpersona(QVariant value)
{
	m_idpersona = value.toLongLong();
}

QVariant PersonaCategoria::idcategoria() const
{
	return m_idcategoria;
}

void PersonaCategoria::setIdcategoria(QVariant value)
{
	m_idcategoria = value.toLongLong();
}

// Objects Getters y Setters Definitions
Categoria * PersonaCategoria::categoria()
{
	if (! m_categoria)
	{
		m_categoria = CategoriaDAO::instance()->getById(m_idcategoria);
	}
	return m_categoria;
}

void PersonaCategoria::setCategoria(Categoria *obj)
{
	m_categoria = obj;
}

} // namespace crud
