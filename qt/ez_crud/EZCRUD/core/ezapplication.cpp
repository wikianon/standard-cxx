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

#include "ezapplication.h"
#include "session.h"
#include "connectionpool.h"

#include "../gui/logindialog.h"

namespace crud {

EZApplication::EZApplication(QObject *parent) :
    QObject(parent),
    m_loginDialog(0),
    m_applicationName("Easy Crud Application")
{
    connect(Session::instance(), SIGNAL(sessionEnded()), SLOT(start()));
}

EZApplication::~EZApplication()
{
    m_loginDialog->deleteLater();
}

EZApplication *EZApplication::instance()
{
    static EZApplication *instance = 0;
    if(!instance){
        qApp->setQuitOnLastWindowClosed(false);
        instance = new EZApplication(qApp);
    }
    return instance;
}

const QString &EZApplication::applicationName() const
{
    return m_applicationName;
}

void EZApplication::setApplicationName(const QString &appName)
{
    m_applicationName = appName;
}

void EZApplication::onLoginSuccessFull(User *user, QSqlDatabase *db)
{
    Session::instance()->startSession(user, db);
}

void EZApplication::onLoginCancelled()
{
    QApplication::quit();
}

void EZApplication::start()
{
    if(m_loginDialog){
        m_loginDialog->clearLoginInformation();
        m_loginDialog->show();
    }else{
        m_loginDialog = new LoginDialog;
        connect(m_loginDialog, SIGNAL(loginSuccessFull(User *, QSqlDatabase *)), SLOT(onLoginSuccessFull(User *, QSqlDatabase *)));
        connect(m_loginDialog, SIGNAL(loginCanceled()), SLOT(onLoginCancelled()));
        m_loginDialog->show();
    }
}

} // namespace crud
