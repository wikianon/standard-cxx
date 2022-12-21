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
#include "../../core/encryption.h"

#include "organizationdao.h"
#include "../object/organization.h"

namespace crud {


OrganizationDAO::OrganizationDAO(QObject *parent) :
    GenericDAO<Organization>(parent)
{
}

OrganizationDAO * OrganizationDAO::instance()
{
    static OrganizationDAO *instance = 0;
    if(! instance){
        instance = new OrganizationDAO(qApp);
    }
    return instance;
}

bool OrganizationDAO::insertObject(Organization *obj)
{
    Organization *organization = qobject_cast<Organization *>(obj);
    if(organization)
    {
        QSqlDatabase *db = crud::Session::instance()->systemDatabase();
        db->transaction();
        try {
            qlonglong idCompany = Functions::getSequence("seq_organization", db);
            organization->setId(idCompany);

            SqlQuery query(db);
            query.prepare("insert into organization (id, name, hostaddress, hostdatabase, hostuser, hostpassword)"
                          " values(:id, :name, :hostaddress, :hostdatabase, :hostuser, :hostpassword)");
            query.bindValue(":id", organization->id());
            query.bindValue(":name", organization->name());
            query.bindValue(":hostaddress", Encryption::encrypt(organization->hostAddress().toString()));
            query.bindValue(":hostdatabase", organization->hostDatabase());
            query.bindValue(":hostuser", organization->hostUser());
            query.bindValue(":hostpassword", Encryption::encrypt(organization->hostPassword().toString()));
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

bool OrganizationDAO::updateObject(Organization *obj)
{
    Organization *organization = qobject_cast<Organization *>(obj);
    if(organization)
    {
        QSqlDatabase *db = crud::Session::instance()->systemDatabase();
        db->transaction();
        try {
            SqlQuery query(db);
            query.prepare("update organization set name = :name, hostaddress = :hostaddress, "
                          " hostdatabase = :hostdatabase, hostuser = :hostuser, hostpassword = :hostpassword "
                          " where id = :id");
            query.bindValue(":id", organization->id());
            query.bindValue(":name", organization->name());
            query.bindValue(":hostaddress", Encryption::encrypt(organization->hostAddress().toString()));
            query.bindValue(":hostdatabase", organization->hostDatabase());
            query.bindValue(":hostuser", organization->hostUser());
            query.bindValue(":hostpassword", Encryption::encrypt(organization->hostPassword().toString()));
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

bool OrganizationDAO::deleteObject(Organization *)
{
    return false;
}

Organization * OrganizationDAO::getById(qlonglong id, QObject *parent)
{
    QSqlDatabase *db = crud::Session::instance()->systemDatabase();
    try {
        SqlQuery query(db);
        query.prepare("select id, name, hostaddress, hostdatabase, hostuser, hostpassword from organization where id = :id");
        query.bindValue(":id", id);
        query.exec();
        if(query.next()){
            Organization *organization = new Organization(query.record(), PARENT(parent));
            return organization;
        }
    }catch(SqlException &ex){

    }
    return 0;
}

QList<Organization *> OrganizationDAO::getByUser(qlonglong idUser, QObject *parent)
{
    QList<Organization *> list;
    QSqlDatabase *db = crud::Session::instance()->systemDatabase();
    try {
        SqlQuery query(db);
        query.prepare("select org.id, org.name, org.hostaddress, org.hostdatabase, org.hostuser, org.hostpassword "
                      " from organization org "
                      " where exists(select null from revision rev where org.id = rev.idorganization and rev.iduser = :idUser) ");
        query.bindValue(":idUser", idUser);
        query.exec();
        while(query.next()){
            list.append(new Organization(query.record(), PARENT(parent)));
        }
        return list;
    }catch(SqlException &ex){
        // deleteAll(list);
        return list;
    }
}

QList<Organization *> OrganizationDAO::getAll(QObject *parent)
{
    QList<Organization *> list;
    QSqlDatabase *db = crud::Session::instance()->systemDatabase();
    try {
        SqlQuery query(db);
        query.prepare("select id, name, hostaddress, hostdatabase, hostuser, hostpassword from organization");
        query.exec();
        while(query.next())
        {
            list.append(new Organization(query.record(), PARENT(parent)));
        }
        return list;
    }catch(SqlException &ex){
        // deleteAll(list);
        return list;
    }
}

QList<Organization *> OrganizationDAO::getByMatch(const QString &column, QVariant value, QObject *parent)
{
    QList<Organization *> list;
    QSqlDatabase *db = crud::Session::instance()->systemDatabase();
    try {
        QString sqlQuery = QString("select id, name, hostaddress, hostdatabase, hostuser, hostpassword from organization where %1 = :value").arg(column);
        SqlQuery query(db);
        query.prepare(sqlQuery);
        query.bindValue(":value", value);
        query.exec();
        while(query.next())
        {
            list.append(new Organization(query.record(), PARENT(parent)));
        }
        return list;
    }catch(SqlException &ex){
        // deleteAll(list);
        return list;
    }
}


} // namespace crud
