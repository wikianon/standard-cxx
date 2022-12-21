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

#ifndef CRUD_USERDAO_H
#define CRUD_USERDAO_H

#include "../genericdao.h"

class QSqlDatabase;

namespace crud {

class User;

class EZCRUD_EXPORT UserDao : public GenericDAO<User>
{
    Q_OBJECT
protected:
    explicit UserDao(QObject *parent = 0);

public:
    static UserDao * instance();

    bool insertObject(User *);
    bool updateObject(User *);
    bool deleteObject(User *);

    User * getById(qlonglong id, QObject *parent = 0);
    QList<User *> getAll(QObject *parent = 0);
    QList<User *> getByMatch(const QString &, QVariant, QObject *parent = 0);
    User * getByLoginName(const QString &loginName, QSqlDatabase *db, QObject *parent = 0);
};


} // namespace crud

#endif // CRUD_USERDAO_H
