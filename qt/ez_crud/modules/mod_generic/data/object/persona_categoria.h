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
#ifndef PERSONA_CATEGORIA_H
#define PERSONA_CATEGORIA_H

#include <QSqlRecord>
#include <data/objectbase.h>

namespace crud {

// Objects Forward Declarations

class Categoria;

class EZCRUD_EXPORT PersonaCategoria : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QVariant idpersona
		READ idpersona
		WRITE setIdpersona)
	Q_PROPERTY(QVariant idcategoria
		READ idcategoria
		WRITE setIdcategoria)

public:
// Constructors
    explicit PersonaCategoria(QObject *parent = 0);
    explicit PersonaCategoria(const QSqlRecord &, QObject *parent = 0);

// Getters & Setters
	QVariant idpersona() const;
	void setIdpersona(QVariant);
	QVariant idcategoria() const;
	void setIdcategoria(QVariant);

// Objects Getters & Setters
	Categoria * categoria();
	void setCategoria(Categoria *);

private:
// Private Members
	qlonglong m_idpersona;
	qlonglong m_idcategoria;

private:
// Private Object Members
	Categoria *m_categoria;
// Other Methodsprivate:
	bool validate();

};

} // namespace crud

#endif // PERSONA_CATEGORIA_H
