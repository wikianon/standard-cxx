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

#ifndef REVISION_H
#define REVISION_H

#include <QObject>
#include <QVariant>
#include <QSqlRecord>

#include "../objectbase.h"

namespace crud
{

class User;
class ModuleRevision;
class WidgetRevision;

class EZCRUD_EXPORT Revision : public ObjectBase
{
    Q_OBJECT
    Q_PROPERTY(QVariant idUser
               READ idUser
               WRITE setIdUser)
    Q_PROPERTY(QVariant idOrganization
               READ idOrganization
               WRITE setIdOrganization)
    Q_PROPERTY(QVariant dateTime
               READ dateTime
               WRITE setDateTime)
public:
    explicit Revision(QObject *parent = 0);
    explicit Revision(const QSqlRecord &record, QObject *parent = 0);

    QVariant idUser() const;
    void setIdUser(QVariant);

    QVariant idOrganization() const;
    void setIdOrganization(QVariant);

    QVariant dateTime() const;
    void setDateTime(QVariant);

    QList< ModuleRevision * > & moduleList();
    QList< WidgetRevision  * > & widgetList();

    QMap<QString, QMap<QString, int > >  modulesMap();

private:
    qlonglong m_idUser;
    qlonglong m_idOrganization;
    QDateTime m_dateTime;

    QList< ModuleRevision * > m_revisionModule;
    QList< WidgetRevision  * > m_revisionWidget;

    QMap<QString, QMap<QString, int > >  m_modulesMap;
};


} // namespace crud


#endif // REVISION_H
