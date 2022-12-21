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

#include <QSqlDatabase>
#include "entityclass.h"



namespace crud
{

EntityClass::EntityClass(QObject *parent) :
    ObjectBase(parent), m_dateTime(), m_state()
{


}


//property getters
QVariant EntityClass::dateTime() const
{
    return QVariant(m_dateTime);
}


QVariant EntityClass::state() const
{
    return QVariant(m_state);
}

QVariant EntityClass::stateText() const
{
    if((m_state == "A") || (m_state.toLongLong() == 1))
        return tr("Activo");
    else
        return tr("Inactivo");
}

//property setters
void EntityClass::setDateTime(QVariant dateTime)
{
    m_dateTime = dateTime.toDateTime();
}


void EntityClass::setState(QVariant state)
{
    m_state = state.toString();
}

} // namespace crud
