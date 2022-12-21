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

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>

#include "../../core/connectionpool.h"

#include "module.h"
#include "widget.h"

#include "../dao/widgetdao.h"

namespace crud
{

Module::Module(QObject *parent) :
    ObjectBase(parent),
    m_moduleName(),
    m_moduleMenu(),
    m_widgets()
{
}

Module::Module(qlonglong id, QObject *parent) :
    ObjectBase(id, parent)
{
}

Module::Module(const QSqlRecord &record, QObject *parent):
    ObjectBase(parent)
{
    //id, nombremodulo, nombremenu
    setId(record.value(0));
    setModuleName(record.value(1));
    setModuleMenu(record.value(2));
}

void Module::setModuleName(QVariant moduleName)
{
  m_moduleName = moduleName.toString();
}

QVariant Module::moduleName() const
{
    return QVariant(m_moduleName);
}

void Module::setModuleMenu(QVariant moduleMenu)
{
    m_moduleMenu = moduleMenu.toString();
}

QVariant Module::moduleMenu() const
{
    return m_moduleMenu;
}


QList< Widget * > & Module::widgetList()
{
    if(m_widgets.count() == 0){
        m_widgets = crud::WidgetDAO::instance()->getAll();
    }
    return m_widgets;
}

} // namespace crud
