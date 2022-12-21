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
#include <QSqlResult>

#include "../../core/sql/sqlquery.h"

#include "../../core/session.h"
#include "../../core/functions.h"

#include "moduledao.h"
#include "../object/module.h"

namespace crud {


ModuleDAO::ModuleDAO(QObject *parent) :
    GenericDAO<Module>(parent)
{
}

ModuleDAO * ModuleDAO::instance()
{
    static ModuleDAO *instance = 0;
    if(! instance){
        instance = new ModuleDAO(qApp);
    }
    return instance;
}

bool ModuleDAO::insertObject(Module *obj)
{
    QSqlDatabase *db = crud::Session::instance()->systemDatabase();
    db->transaction();
    try{
        qlonglong idModulo = Functions::getSequence("seq_module", db);
        obj->setId(idModulo);

        SqlQuery query(db);
        query.prepare("insert into module (id, modulename, modulemenu)"
                      " values(:id, :modulename, :modulemenu)");
        query.bindValue(":id", obj->id().toLongLong());
        query.bindValue(":modulename", obj->moduleName().toString());
        query.bindValue(":modulemenu", obj->moduleMenu().toString());
        query.exec();
        db->commit();
        return true;
    }catch(SqlException &ex){
        db->rollback();
        return false;
    }
    return false;
}

bool ModuleDAO::updateObject(Module *obj)
{
    QSqlDatabase *db = crud::Session::instance()->systemDatabase();
    db->transaction();
    try{
        SqlQuery query(db);
        query.prepare("update module set modulename = :modulename, modulemenu = :modulemenu where id = :id");
        query.bindValue(":id", obj->id());
        query.bindValue(":modulename", obj->moduleName());
        query.bindValue(":modulemenu", obj->moduleMenu());
        query.exec();
        db->commit();
        return true;
    }catch(SqlException &ex){
        db->rollback();
        return false;
    }
    return false;
}

bool ModuleDAO::deleteObject(Module *)
{
    return false;
}

Module * ModuleDAO::getById(qlonglong id, QObject *parent)
{
    QSqlDatabase *db = crud::Session::instance()->systemDatabase();
    try{
        SqlQuery query(db);
        query.prepare("select id, modulename, modulemenu from module where id = :id");
        query.bindValue(":id", id);
        query.exec();
        while(query.next()){
            Module *module = new Module(query.record(), PARENT(parent));
            return module;
        }
    }catch(SqlException &ex){

    }
    return 0;
}

QList<Module *> ModuleDAO::getAll(QObject *parent)
{
    QList<Module *> list;
    QSqlDatabase *db = crud::Session::instance()->systemDatabase();
    try{
        SqlQuery query(db);
        query.prepare("select id, modulename, modulemenu from module");
        query.exec();
        while(query.next()){
            list.append(new Module(query.record(), PARENT(parent)));
        }
        return list;
    }catch(SqlException &ex){
        // deleteAll(list);
        return list;
    }
}

QList<Module *> ModuleDAO::getByMatch(const QString &column, QVariant value, QObject *parent)
{
    QList<Module *> list;
    QSqlDatabase *db = crud::Session::instance()->systemDatabase();
    try{
        QString sqlQuery = QString("select id, modulename, modulemenu from module where %1 = :value").arg(column);
        SqlQuery query(db);
        query.prepare(sqlQuery);
        query.bindValue(":value", value);
        query.exec();
        while(query.next()){
            list.append(new Module(query.record(), PARENT(parent)));
        }
        return list;
    }catch(SqlException &ex){
        // deleteAll(list);
        return list;
    }
}



} // namespace crud
