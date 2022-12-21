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

#include "revisiondao.h"
#include "module_revisiondao.h"
#include "widget_revisiondao.h"

#include "../object/revision.h"
#include "../object/module_revision.h"
#include "../object/widget_revision.h"

namespace crud {

RevisionDAO::RevisionDAO(QObject *parent) :
    GenericDAO<Revision>(parent)
{
}

RevisionDAO * RevisionDAO::instance()
{
    static RevisionDAO *instance = 0;
    if(! instance){
        instance = new RevisionDAO(qApp);
    }
    return instance;
}

bool RevisionDAO::insertObject(Revision *obj)
{
    QSqlDatabase *db = crud::Session::instance()->systemDatabase();
    db->transaction();
    try{
        qlonglong idRevision = Functions::getSequence("seq_revision", db);
        obj->setId(idRevision);
        QDateTime dateTime = Functions::getSystemDate();
        obj->setDateTime(dateTime);

        SqlQuery query(db);
        query.prepare("insert into revision (id, iduser, idorganization, datetime)"
                      " values(:id, :iduser, :idorganization, :datetime)");
        query.bindValue(":id", obj->id());
        query.bindValue(":iduser", obj->idUser());
        query.bindValue(":idorganization", obj->idOrganization());
        query.bindValue(":datetime", obj->dateTime());
        query.exec();

        // Insert ModuleRevision
        foreach(ModuleRevision *mod, obj->moduleList()){
            mod->setIdRevision(idRevision);
            ModuleRevisionDAO::instance()->insertObject(mod, db);
        }

        // Insert WidgetRevision
        foreach(WidgetRevision *wid, obj->widgetList()){
            wid->setIdRevision(idRevision);
            WidgetRevisionDAO::instance()->insertObject(wid, db);
        }

        db->commit();
        return true;
    }catch(SqlException &ex){
        db->rollback();
        return false;
    }
    return false;
}

bool RevisionDAO::updateObject(Revision *)
{
    return false;
}

bool RevisionDAO::deleteObject(Revision *)
{
    return false;
}

Revision * RevisionDAO::getById(qlonglong idRevision, QObject *parent)
{
    QSqlDatabase *db = crud::Session::instance()->systemDatabase();
    try{
        SqlQuery query(db);
        query.prepare("select id, iduser, idorganization, datetime from revision where id = :id");
        query.bindValue(":id", idRevision);
        query.exec();
        while(query.next()){
            Revision *revision = new Revision(query.record(), PARENT(parent));
            return revision;
        }
    }catch(SqlException &ex){

    }
    return 0;
}

Revision *RevisionDAO::getLatestRevision(qlonglong idUser, qlonglong idOrganization, QObject *parent)
{
    QSqlDatabase *db = crud::Session::instance()->systemDatabase();
    try{
        SqlQuery query(db);
        query.prepare("select id, iduser, idorganization, datetime"
                      " from revision r1 "
                      " where r1.datetime = (select max(r2.datetime) "
                      " from revision r2 "
                      " where r2.iduser = r1.iduser "
                      " and r2.idorganization = r1.idorganization ) "
                      " and r1.iduser = :iduser "
                      " and r1.idorganization = :idorganization");
        query.bindValue(":iduser", idUser);
        query.bindValue(":idorganization", idOrganization);
        query.exec();
        query.next();
        Revision *revision = new Revision(query.record(), PARENT(parent));
        return revision;
    }catch(SqlException &ex){

    }
    return 0;
}

QList<Revision *> RevisionDAO::getLatestRevision(qlonglong idUser, QObject *parent)
{
    QList<Revision *> list;
    QSqlDatabase *db = crud::Session::instance()->systemDatabase();
    try{
        SqlQuery query(db);
        query.prepare("select id, iduser, idorganization, datetime"
                      " from revision r1 "
                      " where r1.datetime = (select max(r2.datetime) "
                      " from revision r2 "
                      " where r2.iduser = r1.iduser "
                      " and r2.idorganization = r1.idorganization ) "
                      " and r1.iduser = :iduser ");
        query.bindValue(":iduser", idUser);
        query.exec();
        while(query.next()){
            list.append(new Revision(query.record(), PARENT(parent)));
        }
    }catch(SqlException &ex){
        // deleteAll(list);
    }
    return list;
}

QList<Revision *> RevisionDAO::getAll(QObject *parent)
{
    QList<Revision *> list;
    QSqlDatabase *db = crud::Session::instance()->systemDatabase();
    try{
        SqlQuery query(db);
        query.prepare("select id, iduser, idorganization, datetime from revision");
        query.exec();
        while(query.next()){
            list.append(new Revision(query.record(), PARENT(parent)));
        }
        return list;
    }catch(SqlException &ex){
        // deleteAll(list);
    }
    return list;
}

QList<Revision *> RevisionDAO::getByMatch(const QString &column, QVariant value, QObject *parent)
{
    QList<Revision *> list;
    QSqlDatabase *db = crud::Session::instance()->systemDatabase();
    try{
        QString sqlQuery = QString("select id, iduser, idorganization, datetime from revision where %1 = :value").arg(column);
        SqlQuery query(db);
        query.prepare(sqlQuery);
        query.bindValue(":value", value);
        query.exec();
        while(query.next()){
            list.append(new Revision(query.record(), PARENT(parent)));
        }
        return list;
    }catch(SqlException &ex){
        // deleteAll(list);
    }
    return list;
}



} // namespace crud
