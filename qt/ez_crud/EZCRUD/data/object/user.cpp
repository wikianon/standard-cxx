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

#include "user.h"

namespace crud
{

User::User(QObject *parent)  :
    EntityClass(parent),
    m_loginName(),
    m_userName(),
    m_password(),
    m_email()
{

}

User::User(const QSqlRecord &record, QObject *parent)  :
    EntityClass(parent)
{
    //id, loginName, userName, password, dateTime, email
    setId(record.value(0));
    setLoginName(record.value(1));
    setUserName(record.value(2));
    setPassword(record.value(3));
    setDateTime(record.value(4));
    setEmail(record.value(5));
}

QVariant User::loginName() const
{
    return QVariant(m_loginName);
}

QVariant User::userName() const
{
    return QVariant(m_userName);
}


QVariant User::password() const
{
    return QVariant(m_password);
}

QVariant User::email() const
{
    return m_email;
}

void User::setLoginName(QVariant loginName)
{
    m_loginName = loginName.toString();
}

void User::setUserName(QVariant userName)
{
    m_userName = userName.toString();
}

void User::setPassword(QVariant password)
{
    m_password = password.toString();
}

void User::setEmail(QVariant email)
{
    m_email = email.toString();
}

} // namespace crud
