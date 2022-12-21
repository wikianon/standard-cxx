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

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDateTime>


#include "functions.h"
#include "sql/sqlquery.h"
#include "sql/exception/sqlexception.h"
#include "session.h"

qlonglong Functions::getSequence(const QString &className, QSqlDatabase *db)
{
    qlonglong result = -1;
    try {
        SqlQuery query(db);
        query.prepare("select nextvalue from sequences where name = :name");
        query.bindValue(":name", className);
        query.exec();
        if(query.next()){
            result = query.value(0).toLongLong();
            SqlQuery updateQuery(db);
            updateQuery.prepare("update sequences set nextvalue = :value where name = :name");
            qlonglong nextValue = result + 1;
            updateQuery.bindValue(":value", nextValue);
            updateQuery.bindValue(":name", className);
            updateQuery.exec();
        }else{
            result = -1;
        }
    }catch(SqlException &ex){

        return -1;
    }
    return result;
}


QDateTime Functions::getSystemDate()
{
    //QDateTime result = orm::system::database::getSystemDateTime();
    if(crud::Session::instance()->systemDatabase()->driverName() == "QMYSQL")
        return QDateTime::currentDateTime();
    else
        return QDateTime::currentDateTime();
}
