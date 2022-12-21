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

#ifndef CRUD_CONNECTIONPOOL_H
#define CRUD_CONNECTIONPOOL_H

#include <QObject>
#include <QSqlDatabase>

#include "../global/ezcrud_global.h"

namespace crud {

class Organization;

class EZCRUD_EXPORT ConnectionPool : public QObject
{
    Q_OBJECT
public:
    static ConnectionPool * instance();

    QSqlDatabase * systemConnection();
    QSqlDatabase * organizationConnection(Organization *org);

private:
    ConnectionPool(QObject *parent);
    // database connections
    QSqlDatabase m_systemConnection;
    QSqlDatabase m_organizationConnection;
};

} // namespace crud

#endif // CRUD_CONNECTIONPOOL_H
