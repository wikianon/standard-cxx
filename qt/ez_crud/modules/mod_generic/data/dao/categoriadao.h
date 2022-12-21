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
// * Fecha: 28/09/2012 15:26:19                       *
// * Luis Valdes 2012 - Todos los Derechos Reservados *
// ****************************************************
#ifndef CATEGORIADAO_H
#define CATEGORIADAO_H

#include <data/genericdao.h>

namespace crud {

class Categoria;

class EZCRUD_EXPORT CategoriaDAO : public GenericDAO<Categoria>
{
	Q_OBJECT
protected:
	CategoriaDAO(QObject *parent = 0);
public:
	static CategoriaDAO * instance();
	bool insertObject(Categoria *obj);
	bool updateObject(Categoria *obj);
	bool deleteObject(Categoria *obj);
    Categoria * getById(qlonglong id, QObject *parent=0);
    QList<Categoria *> getByIdPersona(qlonglong idPersona, QObject *parent=0);
    QList<Categoria *> getAll(QObject *parent=0);
    QList<Categoria *> getByMatch(const QString &columnName, QVariant value, QObject *parent=0);
};

} // namespace crud
#endif // CATEGORIADAO_H
