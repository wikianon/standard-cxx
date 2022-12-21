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

#ifndef ORMBASE_H
#define ORMBASE_H

#include <QObject>
#include <QDateTime>
#include <QVariant>
#include <QSqlError>

#include "../global/ezcrud_global.h"

namespace crud
{

class EZCRUD_EXPORT ObjectBase : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant id
               READ id
               WRITE setId)
public:
    explicit ObjectBase(QObject *parent = 0);
    explicit ObjectBase(qlonglong id, QObject *parent = 0);
    virtual ~ObjectBase();
    //property getters
    virtual QVariant id() const;
    //property setters
    virtual void setId(QVariant id);

protected:
    // QxOrm class atributes
    long m_id;

private:
    QList<void *> m_containers;
protected:
    template <class T>
    void addElements(QList< T  * > & list)
    {
        foreach(T *element, list)
        {
            m_containers.append(element);
        }
    }
};

} // namespace crud

#endif // ORMBASE_H
