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

#ifndef CRUD_MODULE_H
#define CRUD_MODULE_H

#include <QObject>
#include <QString>
#include <QVariant>
#include <QList>
#include <QSqlRecord>

#include "../objectbase.h"

namespace crud
{

class Widget;

class EZCRUD_EXPORT Module : public ObjectBase
{
    Q_OBJECT
    Q_PROPERTY(QVariant  moduleName
               READ moduleName
               WRITE setModuleName)
    Q_PROPERTY(QVariant  moduleMenu
               READ moduleMenu
               WRITE setModuleMenu)
public:
    Module(QObject *parent=0);
    Module(qlonglong id, QObject *parent=0);
    Module(const QSqlRecord &, QObject *parent=0);

    QVariant moduleName() const;
    void setModuleName(QVariant);

    QVariant moduleMenu() const;
    void setModuleMenu(QVariant);

    QList< Widget * > & widgetList();

private:
    QString  m_moduleName;
    QString  m_moduleMenu;

    // Un modulo para varios widgets
    QList< Widget * > m_widgets;
};

} // namespace crud


#endif // CRUD_MODULE_H
