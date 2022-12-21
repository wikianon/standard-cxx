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

#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSortFilterProxyModel>
#include <QTableView>

#include <core/session.h>
#include <data/dao/widgetdao.h>
#include <data/object/widget.h>

#include "widgetcrud.h"
#include "../view/widgetview.h"

namespace crud {

WidgetCrud::WidgetCrud(crud::EasyCrud::CRUDFlags &crudFlags, QWidget *parent) :
    BaseCRUD(crudFlags, parent)
{
    //    select id, widgetname, widgetmenu from widget
        QSqlQueryModel *source = new QSqlQueryModel(this);
        source->setQuery(QString("select id, widgetname, widgetmenu from widget"),
                         *crud::Session::instance()->systemDatabase());
        source->setHeaderData(0, Qt::Horizontal, tr("Código"));
        source->setHeaderData(1, Qt::Horizontal, tr("Nombre del Widget"));
        source->setHeaderData(2, Qt::Horizontal, tr("Nombre del Menú"));

        QSortFilterProxyModel * model = new QSortFilterProxyModel(this);
        model->setSourceModel(source);
        setTableViewModel(model);

        tableView()->resizeColumnsToContents();
        tableView()->resizeRowsToContents();

        if(source->query().lastError().isValid())
            qDebug() << source->query().lastError().text();
        setWindowTitle(windowTitle());
}

bool WidgetCrud::insert(crud::ObjectBase *registro)
{
    crud::Widget *widget = qobject_cast<crud::Widget *>(registro);
    bool bInsert = crud::WidgetDAO::instance()->insertObject(widget);
    return bInsert;
}

bool WidgetCrud::save(crud::ObjectBase *registro)
{
    crud::Widget *widget = qobject_cast<crud::Widget *>(registro);
    bool bUpdate = crud::WidgetDAO::instance()->updateObject(widget);
    return bUpdate;
}

crud::ObjectBase *WidgetCrud::getRegistro(qlonglong id)
{
    if(id > 0)
    {
        crud::Widget *widget = crud::WidgetDAO::instance()->getById(id);
        widget->setParent(this);
        return widget;
    }
    return new crud::Widget(this);
}

EntityView *WidgetCrud::getWidget()
{
    return new crud::WidgetView(this);
}

QString WidgetCrud::formName()
{
    return "WidgetCrud";
}

QString WidgetCrud::windowTitle()
{
    return tr("Widget");
}

QSqlDatabase *WidgetCrud::getDatabase()
{
    return crud::Session::instance()->systemDatabase();
}

} // namespace crud
