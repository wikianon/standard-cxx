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

#include <QApplication>
#include <QSqlDatabase>
#include <QSqlRecord>

#include "../../core/sql/sqlquery.h"

#include "../../core/session.h"
#include "../../core/functions.h"

#include "widgetdao.h"
#include "../object/widget.h"

namespace crud {


WidgetDAO::WidgetDAO(QObject *parent) :
    GenericDAO<Widget>(parent)
{
}

WidgetDAO * WidgetDAO::instance()
{
    static WidgetDAO *instance = 0;
    if(! instance)
    {
        instance = new WidgetDAO(qApp);
    }
    return instance;
}

bool WidgetDAO::insertObject(Widget *obj)
{
    Widget *widget = qobject_cast<Widget *>(obj);
    if(widget)
    {
        QSqlDatabase *db = crud::Session::instance()->systemDatabase();
        db->transaction();
        try{
            qlonglong idWidget = Functions::getSequence("seq_widget", db);
            widget->setId(idWidget);

            SqlQuery query(db);
            query.prepare("insert into widget (id, widgetname, widgetmenu, idmodule, widgettype)"
                          " values(:id, :widgetname, :widgetmenu, :idmodule, :widgettype)");
            query.bindValue(":id", widget->id());
            query.bindValue(":widgetname", widget->widgetName());
            query.bindValue(":widgetmenu", widget->widgetMenu());
            query.bindValue(":idmodule", widget->idModule());
            query.bindValue(":widgettype", widget->widgetType());
            query.exec();
            db->commit();
            return true;
        }catch(SqlException &ex){
            db->rollback();
            return false;
        }
    }
    return false;
}

bool WidgetDAO::updateObject(Widget *obj)
{
    Widget *widget = qobject_cast<Widget *>(obj);
    if(widget)
    {
        QSqlDatabase *db = crud::Session::instance()->systemDatabase();
        db->transaction();
        try{
            SqlQuery query(db);
            query.prepare("update widget set widgetname = :widgetname, widgetmenu = :widgetmenu, "
                          " idmodule = :idmodule, widgettype = :widgettype where id = :id");
            query.bindValue(":id", widget->id());
            query.bindValue(":widgetname", widget->widgetName());
            query.bindValue(":widgetmenu", widget->widgetMenu());
            query.bindValue(":idmodule", widget->idModule());
            query.bindValue(":widgettype", widget->widgetType());
            query.exec();
            db->commit();
            return true;
        }catch(SqlException &ex){
            db->rollback();
            return false;
        }
    }
    return false;
}

bool WidgetDAO::deleteObject(Widget *)
{
    return false;
}

Widget * WidgetDAO::getById(qlonglong id, QObject *parent)
{
    QSqlDatabase *db = crud::Session::instance()->systemDatabase();
    try{
        SqlQuery query(db);
        query.prepare("select id, widgetname, widgetmenu, idmodule, widgettype from widget where id = :id");
        query.bindValue(":id", id);
        query.exec();
        while(query.next())
        {
            Widget *widget = new Widget(query.record(), PARENT(parent));
            return widget;
        }
    }catch(SqlException &ex){

    }
    return 0;
}

QList<Widget *> WidgetDAO::getAll(QObject *parent)
{
    QList<Widget *> list;
    QSqlDatabase *db = crud::Session::instance()->systemDatabase();
    try{
        SqlQuery query(db);
        query.prepare("select id, widgetname, widgetmenu, idmodule, widgettype from widget");
        query.exec();
        while(query.next()){
            list.append(new Widget(query.record(), PARENT(parent)));
        }
        return list;
    }catch(SqlException &ex){
        // deleteAll(list);
        return list;
    }
}

QList<Widget *> WidgetDAO::getByMatch(const QString &column, QVariant value, QObject *parent)
{
    QList<Widget *> list;
    QSqlDatabase *db = crud::Session::instance()->systemDatabase();
    try{
        QString sqlQuery = QString("select id, widgetname, widgetmenu, idmodule, widgettype from widget where %1 = :value").arg(column);
        SqlQuery query(db);
        query.prepare(sqlQuery);
        query.bindValue(":value", value);
        query.exec();
        while(query.next())
        {
            list.append(new Widget(query.record(), PARENT(parent)));
        }
        return list;
    }catch(SqlException &ex){
        // deleteAll(list);
        return list;
    }
}



} // namespace crud
