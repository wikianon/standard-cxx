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

#ifndef USER_H
#define USER_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QSqlRecord>

#include "../entityclass.h"

namespace crud
{

class EZCRUD_EXPORT User : public EntityClass
{
    Q_OBJECT

    Q_PROPERTY(QVariant loginName
               READ loginName
               WRITE setLoginName)
    Q_PROPERTY(QVariant userName
               READ userName
               WRITE setUserName)
    Q_PROPERTY(QVariant password
               READ password
               WRITE setPassword)
    Q_PROPERTY(QVariant email
               READ email
               WRITE setEmail)
public:
    User(QObject *parent=0);
    User(const QSqlRecord &, QObject *parent=0);

    //getters
    QVariant loginName() const;
    QVariant userName() const;
    QVariant password() const;
    QVariant email() const;
    //setters
    void setUserName(QVariant userName);
    void setLoginName(QVariant loginName);
    void setPassword(QVariant password);
    void setEmail(QVariant email);


private:
    // QxOrm class atributes
    QString m_loginName;
    QString m_userName;
    QString m_password;
    QString m_email;
};

} // namespace crud

#endif // USER_H
