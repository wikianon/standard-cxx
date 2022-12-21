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

#ifndef ORMCLASS_H
#define ORMCLASS_H

#include <QObject>
#include <QDateTime>
#include <QVariant>

#include "objectbase.h"

namespace crud
{

class EZCRUD_EXPORT EntityClass : public ObjectBase
{
    Q_OBJECT
    Q_PROPERTY(QVariant dateTime
               READ dateTime
               WRITE setDateTime)
    Q_PROPERTY(QVariant state
               READ state
               WRITE setState)
    Q_PROPERTY(QVariant stateText
               READ stateText)
    public:
        explicit EntityClass(QObject *parent = 0);
    //property getters
    virtual QVariant dateTime() const;
    virtual QVariant state() const;
    virtual QVariant stateText() const;
    //property setters
    virtual void setDateTime(QVariant date);
    virtual void setState(QVariant state);

protected:
    QDateTime m_dateTime;
    QDateTime m_lastUpdate;
    QString m_state;

};

} // namespace crud

#endif // ORMCLASS_H
