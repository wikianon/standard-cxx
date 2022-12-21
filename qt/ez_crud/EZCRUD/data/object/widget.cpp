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

#include "widget.h"


namespace crud
{

Widget::Widget(QObject *parent) :
    ObjectBase(parent) ,
    m_widgetName(),
    m_widgetMenu(),
    m_idModule(),
    m_widgetType()
{

}

Widget::Widget(qlonglong id, QObject *parent) :
    ObjectBase(id, parent),
    m_widgetName(),
    m_widgetMenu(),
    m_idModule(),
    m_widgetType()
{
}

Widget::Widget(const QSqlRecord &record, QObject *parent):
    ObjectBase(parent)
{
    //id, nombreformulario, nombremenu, idmodulo, tipoformulariosetId(record.value(0));
    setId(record.value(0));
    setWidgetName(record.value(1));
    setWidgetMenu(record.value(2));
    setIdModule(record.value(3));
    setWidgetType(record.value(4));
}


QVariant Widget::widgetName() const
{
    return QVariant(m_widgetName);
}

void Widget::setWidgetName(QVariant widgetName)
{
    m_widgetName = widgetName.toString();
}

QVariant Widget::widgetMenu() const
{
    return m_widgetMenu;
}

void Widget::setWidgetMenu(QVariant widgetMenu)
{
    m_widgetMenu = widgetMenu.toString();
}

QVariant Widget::idModule() const
{
    return m_idModule;
}

void Widget::setIdModule(QVariant idModulo)
{
    m_idModule = idModulo.toLongLong();
}

QVariant Widget::widgetType() const
{
    return m_widgetType;
}

void Widget::setWidgetType(QVariant tipoformulario)
{
    m_widgetType = tipoformulario.toInt();
}

} // namespace crud
