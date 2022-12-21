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

#include <QDebug>

#include "objectbase.h"

namespace crud
{

/*!
    \class ObjectBase
    \brief La base para todas la entidades concretas dentro de la biblioteca
*/
ObjectBase::ObjectBase(QObject *parent) :
    QObject(parent), m_id(-1)
{
}

ObjectBase::ObjectBase(qlonglong id, QObject *parent) :
    QObject(parent), m_id(id)
{
}

ObjectBase::~ObjectBase()
{
    deleteAll(m_containers);
}

//property getters
QVariant ObjectBase::id() const
{
    return QVariant(qlonglong(m_id));
}

//property setters
void ObjectBase::setId(QVariant id)
{
    m_id = id.toLongLong();

}

} // namespace crud
