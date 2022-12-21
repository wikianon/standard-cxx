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

#ifndef MYSQL_WIDGET_H
#define MYSQL_WIDGET_H

#include <QObject>
#include <QString>
#include <QVariant>
#include <QSharedPointer>
#include <QSqlRecord>

#include "../objectbase.h"

namespace crud
{

class Module;
class RepositoryWidget;

class  EZCRUD_EXPORT Widget : public ObjectBase
{
    Q_OBJECT
    Q_PROPERTY(QVariant widgetName
               READ widgetName
               WRITE setWidgetName)
    Q_PROPERTY(QVariant widgetMenu
               READ widgetMenu
               WRITE setWidgetMenu)
    Q_PROPERTY(QVariant idModule
               READ idModule
               WRITE setIdModule)
    Q_PROPERTY(QVariant widgetType
               READ widgetType
               WRITE setWidgetType)
public:
    Widget(QObject *parent=0);
    Widget(qlonglong id, QObject *parent=0);
    Widget(const QSqlRecord &, QObject *parent=0);

    QVariant widgetName() const;
    void setWidgetName(QVariant);

    QVariant widgetMenu() const;
    void setWidgetMenu(QVariant);

    QVariant idModule() const;
    void setIdModule(QVariant);

    QVariant widgetType() const;
    void setWidgetType(QVariant);

private:
    QString m_widgetName;
    QString m_widgetMenu;
    qlonglong m_idModule;
    int m_widgetType;
};

} // namespace crud


#endif // MYSQL_WIDGET_H
