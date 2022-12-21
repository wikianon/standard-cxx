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

#ifndef CRUD_ORGANIZATIONDAO_H
#define CRUD_ORGANIZATIONDAO_H

#include "../genericdao.h"

namespace crud {

class Organization;

class EZCRUD_EXPORT OrganizationDAO : public GenericDAO<Organization>
{
    Q_OBJECT
protected:
    explicit OrganizationDAO(QObject *parent = 0);
public:
    static OrganizationDAO * instance();
    bool insertObject(Organization *);
    bool updateObject(Organization *);
    bool deleteObject(Organization *);

    Organization * getById(qlonglong id, QObject *parent = 0);
    QList<Organization *> getByUser(qlonglong idUser, QObject *parent = 0);
    QList<Organization *> getAll(QObject *parent = 0);
    QList<Organization *> getByMatch(const QString &, QVariant, QObject *parent = 0);

};

} // namespace crud

#endif // CRUD_ORGANIZATIONDAO_H
