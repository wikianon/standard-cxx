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
#ifndef PERSONA_CATEGORIADAO_H
#define PERSONA_CATEGORIADAO_H

#include <data/genericdao.h>

namespace crud {

class PersonaCategoria;

class EZCRUD_EXPORT PersonaCategoriaDAO : public GenericDAO<PersonaCategoria>
{
	Q_OBJECT
protected:
    PersonaCategoriaDAO(QObject *parent = 0);
public:
    static PersonaCategoriaDAO * instance();
	bool insertObject(PersonaCategoria *obj);
    bool insertObjectList(qlonglong idPersona, QList<PersonaCategoria *> &objectList);
	bool updateObject(PersonaCategoria *obj);
	bool deleteObject(PersonaCategoria *obj);
    bool deletePersonaCategoria(qlonglong idPersona);
    PersonaCategoria * getById(qlonglong id, QObject *parent=0);
    QList<PersonaCategoria *> getAll(QObject *parent=0);
    QList<PersonaCategoria *> getByMatch(const QString &columnName, QVariant value, QObject *parent=0);
};

} // namespace crud
#endif // PERSONA_CATEGORIADAO_H
