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
#include "../../core/sql/exception/sqlconnectionexception.h"
#include "../../core/sql/exception/sqlstatementexception.h"
#include "../../core/sql/exception/sqltransactionexception.h"
#include "../../core/sql/exception/sqlunknownexception.h"

#include "../../core/session.h"
#include "../../core/connectionpool.h"
#include "../../core/encryption.h"
#include "../../core/functions.h"

#include "userdao.h"
#include "../object/user.h"

namespace crud {


UserDao::UserDao(QObject *parent) :
    GenericDAO<User>(parent)
{
}

UserDao * UserDao::instance()
{
    static UserDao *instance = 0;
    if(! instance){
        instance = new UserDao(qApp);
    }
    return instance;
}

bool UserDao::insertObject(User *obj)
{
    QSqlDatabase *db = crud::Session::instance()->systemDatabase();
    db->transaction();
    try{
        qlonglong idUser = Functions::getSequence("seq_user", db);
        obj->setId(idUser);
        QDateTime dateTime = Functions::getSystemDate();
        obj->setDateTime(dateTime);

        SqlQuery query(db);
        query.prepare("insert into user (id, loginName, userName, password, datetime, email)"
                      " values(:id, :loginName, :userName, :password, :datetime, :email)");
        query.bindValue(":id", obj->id());
        query.bindValue(":loginName", obj->loginName());
        query.bindValue(":userName", obj->userName());
        query.bindValue(":password", Encryption::encrypt(obj->password().toString()));
        query.bindValue(":datetime", obj->dateTime());
        query.bindValue(":email", obj->email());
        query.exec();
        db->commit();
        return true;
    }catch(SqlException &ex){
        db->rollback();
        return false;
    }
    return false;
}

bool UserDao::updateObject(User *obj)
{
    QSqlDatabase *db = crud::Session::instance()->systemDatabase();
    db->transaction();
    try{
        SqlQuery query(db);
        query.prepare("update user set loginName = :loginName, userName = :userName, "
                      "password = :password, datetime = :datetime, email = :email where id = :id");
        query.bindValue(":id", obj->id());
        query.bindValue(":loginName", obj->loginName());
        query.bindValue(":userName", obj->userName());
        query.bindValue(":password", Encryption::encrypt(obj->password().toString()));
        query.bindValue(":datetime", obj->dateTime());
        query.bindValue(":email", obj->email());
        query.exec();
        db->commit();
        return true;
    }catch(SqlException &ex){
        db->rollback();
        return false;
    }
    return false;
}

bool UserDao::deleteObject(User *)
{
    return false;
}

User * UserDao::getById(qlonglong id, QObject *parent)
{
    QSqlDatabase *db = crud::Session::instance()->systemDatabase();
    try{
        SqlQuery query(db);
        query.prepare("select id, loginName, userName, password, datetime, email from user where id = :id");
        query.bindValue(":id", id);
        query.exec();
        while(query.next()){
            User *user = new User(query.record(), PARENT(parent));
            return user;
        }
    }catch(SqlConnectionException &ex){
        qDebug() << QString("SqlConnectionException\nMethod = %1\nErrorText = %2\nDatabaseErrorText = %3\nDriverErrorText = %4").arg(Q_FUNC_INFO, ex.errorText(), ex.databaseErrorText(), ex.driverErrorTex());
    }catch(SqlStatementException &ex){
        qDebug() << QString("SqlStatementException\nMethod = %1\nErrorText = %2\nDatabaseErrorText = %3\nDriverErrorText = %4").arg(Q_FUNC_INFO, ex.errorText(), ex.databaseErrorText(), ex.driverErrorTex());
    }catch(SqlTransactionException &ex){
        qDebug() << QString("SqlTransactionException\nMethod = %1\nErrorText = %2\nDatabaseErrorText = %3\nDriverErrorText = %4").arg(Q_FUNC_INFO, ex.errorText(), ex.databaseErrorText(), ex.driverErrorTex());
    }catch(SqlUnknownException &ex){
        qDebug() << QString("SqlUnknownException\nMethod = %1\nErrorText = %2\nDatabaseErrorText = %3\nDriverErrorText = %4").arg(Q_FUNC_INFO, ex.errorText(), ex.databaseErrorText(), ex.driverErrorTex());
    }catch(SqlException &ex){
        qDebug() << QString("SqlException\nMethod = %1\nErrorText = %2\nDatabaseErrorText = %3\nDriverErrorText = %4").arg(Q_FUNC_INFO, ex.errorText(), ex.databaseErrorText(), ex.driverErrorTex());
    }
    return 0;
}

QList<User *> UserDao::getAll(QObject *parent)
{
    QList<User *> list;
    QSqlDatabase *db = crud::Session::instance()->systemDatabase();
    try{
        SqlQuery query(db);
        query.prepare("select id, loginName, userName, password, datetime, email from user");
        query.exec();
        while(query.next())
        {
            list.append(new User(query.record(), PARENT(parent)));
        }
        return list;
    }catch(SqlException &ex){
        // deleteAll(list);
        return list;
    }
}

QList<User *> UserDao::getByMatch(const QString &column, QVariant value, QObject *parent)
{
    QList<User *> list;
    QSqlDatabase *db = crud::Session::instance()->systemDatabase();
    try{
        QString sqlQuery = QString("select id, loginName, userName, password, datetime, email from user where %1 = :value").arg(column);
        SqlQuery query(db);
        query.prepare(sqlQuery);
        query.bindValue(":value", value);
        query.exec();
        while(query.next())
        {
            list.append(new User(query.record(), PARENT(parent)));
        }
        return list;
    }catch(SqlException &ex){
        // deleteAll(list);
        return list;
    }
}

User * UserDao::getByLoginName(const QString &loginName, QSqlDatabase *db, QObject *parent)
{
    try{
        SqlQuery query(db);
        query.prepare("select id, loginName, userName, password, datetime, email from user where loginName = :loginName");
        query.bindValue(":loginName", loginName);
        query.exec();
        if (query.next())
        {
            User *user = new User(query.record(), PARENT(parent));
            return user;
        }
    }catch(SqlException &ex){

    }
    return 0;
}




} // namespace crud
