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
#include <QSqlError>
#include <QDebug>

#include "connectionpool.h"
#include "encryption.h"
#include "settings.h"

#include "../../data/object/organization.h"

namespace crud {

ConnectionPool::ConnectionPool(QObject *parent) :
    QObject(parent)
{
}

ConnectionPool *ConnectionPool::instance()
{
    // Singleton
    static ConnectionPool *m_instance = 0;

    if(!m_instance)
    {
        m_instance = new ConnectionPool(qApp);
    }
    return m_instance;
}

QSqlDatabase * ConnectionPool::systemConnection()
{
    int tipoServer = Settings::getValue(Settings::ColumnServerType).toInt();
    if (tipoServer == Settings::MySQL){
        if(! QSqlDatabase::contains("System") ){
            m_systemConnection = QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL", "System"));
            QString hostName = Encryption::decrypt(Settings::getValue(Settings::ColumnServerAddress).toString());
            QString hostDatabase = "sysdb";
            QString hostUserName = "sysuser";
            QString hostPassword = "123456";
            m_systemConnection.setHostName(hostName);
            m_systemConnection.setDatabaseName(hostDatabase);
            m_systemConnection.setUserName(hostUserName);
            m_systemConnection.setPassword(hostPassword);
        }
        if(!m_systemConnection.isOpen())
            m_systemConnection.open();
    }else{
        QString appDirPath(QApplication::applicationDirPath());
        QString sysdbPath(appDirPath + "/sysdb.db");
        m_systemConnection = QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", "System"));
        m_systemConnection.setDatabaseName(sysdbPath);
        m_systemConnection.open();
    }
    qDebug() << m_systemConnection.lastError().text();
    return &m_systemConnection;
}

QSqlDatabase * ConnectionPool::organizationConnection(Organization *org)
{
    int tipoServer = Settings::getValue(Settings::ColumnServerType).toInt();
    if (tipoServer == Settings::MySQL){
        if(! QSqlDatabase::contains("Organization") ){
            m_organizationConnection = QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL", "Organization"));
            QString hostName = Encryption::decrypt(org->hostAddress().toString());
            QString hostDatabase = org->hostDatabase().toString();
            QString hostUserName = org->hostUser().toString();
            QString hostPassword = Encryption::decrypt(org->hostPassword().toString());
            m_organizationConnection.setHostName(hostName);
            m_organizationConnection.setDatabaseName(hostDatabase);
            m_organizationConnection.setUserName(hostUserName);
            m_organizationConnection.setPassword(hostPassword);
        }
        if(!m_organizationConnection.isOpen())
            m_organizationConnection.open();
    }else{
        m_organizationConnection = QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", "Organization"));
        QString appDirPath(QApplication::applicationDirPath());
        QString hostDatabase(appDirPath + "/" + org->name().toString().trimmed().toLower() + ".db");
        m_organizationConnection.setDatabaseName(hostDatabase);
        m_organizationConnection.open();
    }
    qDebug() << m_systemConnection.lastError().text();
    return &m_organizationConnection;
}

} // namespace crud
