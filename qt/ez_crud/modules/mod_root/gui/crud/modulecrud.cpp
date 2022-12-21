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
#include <data/object/module.h>
#include <data/dao/moduledao.h>

#include "modulecrud.h"
#include "../view/moduleview.h"

namespace crud {

ModuleCrud::ModuleCrud(EasyCrud::CRUDFlags &crudFlags, QWidget *parent) :
    BaseCRUD(crudFlags, parent)
{
//    select id, modulename, modulemenu from module
    QSqlQueryModel *source = new QSqlQueryModel(this);
    source->setQuery(QString("select id, modulename, modulemenu from module"),
                     *crud::Session::instance()->systemDatabase());
    source->setHeaderData(0, Qt::Horizontal, tr("Código"));
    source->setHeaderData(1, Qt::Horizontal, tr("Nombre del Module"));
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

bool ModuleCrud::insert(crud::ObjectBase *registro)
{
    crud::Module *module = qobject_cast<crud::Module *>(registro);
    bool bInsert = crud::ModuleDAO::instance()->insertObject(module);
    return bInsert;
}

bool ModuleCrud::save(crud::ObjectBase *registro)
{
    crud::Module *module = qobject_cast<crud::Module *>(registro);
    bool bUpdate = crud::ModuleDAO::instance()->updateObject(module);
    return bUpdate;
}

crud::ObjectBase *ModuleCrud::getRegistro(qlonglong id)
{
    if(id > 0){
        crud::Module *mod = crud::ModuleDAO::instance()->getById(id);
        mod->setParent(this);
        return mod;
    }
    return new crud::Module(this);
}

EntityView *ModuleCrud::getWidget()
{
    return new ModuleView(this);
}

QString ModuleCrud::formName()
{
    return "ModuleCrud";
}

QString ModuleCrud::windowTitle()
{
    return tr("Modulos");
}

QSqlDatabase *ModuleCrud::getDatabase()
{
    return crud::Session::instance()->systemDatabase();
}

} // namespace crud
