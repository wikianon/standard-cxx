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

#ifndef CRUD_DAO_WIDGETDAO_H
#define CRUD_DAO_WIDGETDAO_H

#include "../genericdao.h"
namespace crud {

class Widget;

class EZCRUD_EXPORT WidgetDAO : public GenericDAO <Widget>
{
    Q_OBJECT
protected:
    explicit WidgetDAO(QObject *parent = 0);

public:
    static WidgetDAO * instance();
    bool insertObject(Widget *);
    bool updateObject(Widget *);
    bool deleteObject(Widget *);

    Widget * getById(qlonglong id, QObject *parent = 0);
    QList<Widget *> getAll(QObject *parent = 0);
    QList<Widget *> getByMatch(const QString &, QVariant, QObject *parent = 0);

};


} // namespace crud

#endif // CRUD_DAO_WIDGETDAO_H
