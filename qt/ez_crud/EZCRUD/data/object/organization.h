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

#ifndef ORM_SYSTEM_EMPRESA_H
#define ORM_SYSTEM_EMPRESA_H

#include <QSqlRecord>

#include "../objectbase.h"

namespace crud {

class EZCRUD_EXPORT Organization : public ObjectBase
{
    Q_OBJECT
    Q_PROPERTY(QVariant name
               READ name
               WRITE setName)
    Q_PROPERTY(QVariant hostAddress
               READ hostAddress
               WRITE setHostAddress)
    Q_PROPERTY(QVariant hostDatabase
               READ hostDatabase
               WRITE setHostDatabase)
    Q_PROPERTY(QVariant hostUser
               READ hostUser
               WRITE setHostUser)
    Q_PROPERTY(QVariant hostPassword
               READ hostPassword
               WRITE setHostPassword)
public:
    explicit Organization(QObject *parent = 0);
    explicit Organization(const QSqlRecord &, QObject *parent = 0);

    QVariant name() const;
    void setName(QVariant);
    QVariant hostAddress() const;
    void setHostAddress(QVariant);
    QVariant hostDatabase() const;
    void setHostDatabase(QVariant);
    QVariant hostUser() const;
    void setHostUser(QVariant);
    QVariant hostPassword() const;
    void setHostPassword(QVariant);

private:
    QString m_nombreempresa;
    QString m_hostaddress;
    QString m_hostdatabase;
    QString m_hostuser;
    QString m_hostpassword;
};

} // namespace crud

#endif // ORM_SYSTEM_EMPRESA_H
