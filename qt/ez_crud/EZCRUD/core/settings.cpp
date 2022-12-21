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
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QSharedPointer>
#include <QWidget>

#include "settings.h"

namespace crud {

Settings::Settings(QObject *parent) :
    QObject(parent)
{
    m_qSettings = new QSettings("EZCRUD", "ezApplication", this);
}
// Database
const QString Settings::ColumnServerType = "servertype";
// System
const QString Settings::ColumnServerAddress = "serveraddress";
const QString Settings::ColumnServerPort = "serverport";
// Organization
const QString Settings::ColumnHostDatabase = "hostdatabase";
const QString Settings::ColumnHostUser = "hostuser";
const QString Settings::ColumnHostPassword = "hostpassword";

Settings *Settings::instance()
{
    static Settings *instance = 0;
    if (! instance){
        instance = new Settings(qApp);
    }
    return instance;
}

void Settings::setValue(const QString &settingName, QVariant value)
{
    QSqlDatabase *db = Settings::instance()->getDatabase();
    db->transaction();
    QSqlQuery query(*db);
    query.prepare("update settings set value = :newvalue where settingname = :settingname");
    query.bindValue(":newvalue", value);
    query.bindValue(":settingname", settingName);
    query.exec();
    db->commit();
}

QVariant Settings::getValue(const QString &settingName)
{
    QSqlDatabase *db = Settings::instance()->getDatabase();
    QSqlQuery query(*db);
    query.prepare("select value from settings where settingname = :settingname");
    query.bindValue(":settingname", settingName);
    query.exec();
    if(query.next()){
        return query.value(0);
    }
    return QVariant();
}

QSettings *Settings::qSettings()
{
    return instance()->m_qSettings;
}

/*!
        \fn QSqlDatabase* getDatabase()
        \brief Retorna un QSqlDatabase * utilizado dentro del modulo
*/
QSqlDatabase * Settings::getDatabase()
{
    static QSharedPointer<QSqlDatabase> settingsDB;
    static QString dbName = QApplication::applicationDirPath() + "/setttins.db";
    if (! settingsDB.data()){
        settingsDB = QSharedPointer<QSqlDatabase>( new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE")) );
        if(QFile::exists(dbName)){
            settingsDB->setDatabaseName(dbName);
            settingsDB->open();
        }else{
            settingsDB->setDatabaseName(dbName);
            settingsDB->open();
            settingsDB->transaction();
            QString createTableSettings(QString("CREATE TABLE settings ( \n")+
                                      QString("settingName TEXT NOT NULL, \n")+
                                      QString("value TEXT, \n")+
                                      QString("PRIMARY KEY(settingName) \n")+
                                      QString(") "));
            QSqlQuery query(*settingsDB);
            query.exec(createTableSettings);
            settingsDB->commit();
            createSettings();
        }
    }
    return settingsDB.data();
}

void Settings::createSettings()
{
    QSqlDatabase *db = getDatabase();
    db->transaction();
    QSqlQuery query(*db);
    query.exec("insert into settings(settingname, value) values('servertype', 1)");
    query.exec("insert into settings(settingname, value) values('serveraddress', 'nVoh9Z5vrxj/u3CYL71pmQ==')");
    query.exec("insert into settings(settingname, value) values('serverport', 'iFW+iDshVRYty7LKken3mA==')");
    query.exec("insert into settings(settingname, value) values('hostdatabase', '')");
    query.exec("insert into settings(settingname, value) values('hostuser', '')");
    query.exec("insert into settings(settingname, value) values('hostpassword', '')");
    db->commit();
}

} // namespace crud
