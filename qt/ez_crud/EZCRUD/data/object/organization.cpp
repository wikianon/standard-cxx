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

#include "organization.h"

namespace crud {

Organization::Organization(QObject *parent) :
    ObjectBase(parent)
{
}

Organization::Organization(const QSqlRecord &record, QObject *parent) :
    ObjectBase(parent)
{
    //id, nombreempresa, hostaddress, hostdatabase, hostuser, hostpassword
    setId(record.value(0));
    setName(record.value(1));
    setHostAddress(record.value(2));
    setHostDatabase(record.value(3));
    setHostUser(record.value(4));
    setHostPassword(record.value(5));
}

QVariant Organization::name() const
{
    return m_nombreempresa;
}

void Organization::setName(QVariant nombreEmpresa)
{
    m_nombreempresa = nombreEmpresa.toString();
}

QVariant Organization::hostAddress() const
{
    return m_hostaddress;
}

void Organization::setHostAddress(QVariant hostaddress)
{
    m_hostaddress = hostaddress.toString();
}

QVariant Organization::hostDatabase() const
{
    return m_hostdatabase;
}

void Organization::setHostDatabase(QVariant hostdatabase)
{
    m_hostdatabase = hostdatabase.toString();
}


QVariant Organization::hostUser() const
{
    return m_hostuser;
}

void Organization::setHostUser(QVariant hostuser)
{
    m_hostuser = hostuser.toString();
}

QVariant Organization::hostPassword() const
{
    return m_hostpassword;
}

void Organization::setHostPassword(QVariant hostpassword)
{
    m_hostpassword = hostpassword.toString();
}

} // namespace crud
