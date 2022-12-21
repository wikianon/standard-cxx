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
#ifndef CATEGORIA_H
#define CATEGORIA_H

#include <QSqlRecord>
#include <data/objectbase.h>

namespace crud {

// Objects Forward Declarations

class PersonaCategoria;

class EZCRUD_EXPORT Categoria : public ObjectBase
{
	Q_OBJECT
	Q_PROPERTY(QVariant nombre
		READ nombre
		WRITE setNombre)

public:
// Constructors
	explicit Categoria(QObject *parent = 0);
	explicit Categoria(const QSqlRecord &, QObject *parent = 0);

// Getters & Setters
	QVariant nombre() const;
	void setNombre(QVariant);

// Objects Getters & Setters
	QList<PersonaCategoria *> & persona_categoriaList();
	void setPersona_categoriaList(QList<PersonaCategoria *> &);

private:
// Private Members
	QString m_nombre;

private:
// Private Object Members
	QList<PersonaCategoria *> m_persona_categoriaList;
// Other Methodsprivate:
	bool validate();

};

} // namespace crud

#endif // CATEGORIA_H
