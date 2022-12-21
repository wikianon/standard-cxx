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

#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSortFilterProxyModel>
#include <QTableView>
#include <QTabWidget>
#include <QCloseEvent>

#include <core/dynamic/viewfactory.h>
#include <core/functions.h>
#include <core/session.h>
#include <core/encryption.h>
#include <gui/navigatorbar.h>

#include <data/object/module.h>
#include <data/object/user.h>
#include <data/object/revision.h>

#include <data/dao/moduledao.h>
#include <data/dao/userdao.h>
#include <data/dao/revisiondao.h>

#include "../../data/model/accessmodemodel.h"

#include "usercrud.h"
#include "../view/userview.h"


namespace crud {

UserCrud::UserCrud(EasyCrud::CRUDFlags &crudFlags, QWidget *parent) :
    BaseCRUD(crudFlags, parent),
    m_moduleList()
{
    setObjectName("CatastroUsuario");
    m_moduleList = ModuleDAO::instance()->getAll();

    // id, datetime, loginname, username
    QSqlQueryModel *source = new QSqlQueryModel(this);
    source->setQuery(QString("select user.id,\n") +
                     QString("       user.datetime,\n") +
                     QString("       user.loginname,\n") +
                     QString("       user.username\n") +
                     QString("  from user\n") +
                     QString("order by user.id"), *crud::Session::instance()->systemDatabase());
    source->setHeaderData(0, Qt::Horizontal, tr("Codigo"));
    source->setHeaderData(1, Qt::Horizontal, tr("Fecha de inserción"));
    source->setHeaderData(2, Qt::Horizontal, tr("Nombre de Usuario"));
    source->setHeaderData(3, Qt::Horizontal, tr("Nombre Real"));

    QSortFilterProxyModel * model = new QSortFilterProxyModel(this);
    model->setSourceModel(source);
    setTableViewModel(model);

    tableView()->resizeColumnsToContents();
    tableView()->resizeRowsToContents();

    if(source->query().lastError().isValid())
        qDebug() << source->query().lastError().text();

    setWindowTitle(windowTitle());
}

bool UserCrud::insert(crud::ObjectBase *registro)
{    
    crud::User *user = qobject_cast<crud::User *>(registro);
    bool bInsert = crud::UserDao::instance()->insertObject(user);
    if(bInsert){
        UserView *userView = qobject_cast<UserView *>(tabWidget()->currentWidget());
        if(userView){
            foreach(crud::Revision *revision, userView->getNewRevisionList()){
                revision->setIdUser(user->id().toLongLong());
                crud::RevisionDAO::instance()->insertObject(revision);
            }
        }
    }
    return bInsert;
}

bool UserCrud::save(crud::ObjectBase *registro)
{
    crud::User *user = qobject_cast<crud::User *>(registro);
    bool bUpdate = crud::UserDao::instance()->updateObject(user);
    if(bUpdate){
        UserView *userView = qobject_cast<UserView *>(tabWidget()->currentWidget());
        if(userView){
            foreach(crud::Revision *revision, userView->getNewRevisionList()){
                crud::RevisionDAO::instance()->insertObject(revision);
            }
        }
    }
    return bUpdate;
}

crud::ObjectBase *UserCrud::getRegistro(qlonglong id)
{
    if(id > 0){
        User *user = UserDao::instance()->getById(id);
        if(user){
            user->setParent(this);
            user->setPassword(crud::Encryption::decrypt(user->password().toString()));
            return user;
        }
    }
    return new crud::User (this);
}

EntityView *UserCrud::getWidget()
{
    return new UserView(this);
}

QString UserCrud::formName()
{
    return "UserCrud";
}

QString UserCrud::windowTitle()
{
    return tr("Usuarios");
}

QSqlDatabase *UserCrud::getDatabase()
{
    return crud::Session::instance()->systemDatabase();
}

} // namespace crud
