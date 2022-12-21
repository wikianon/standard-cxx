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

#ifndef CRUD_EZAPPLICATION_H
#define CRUD_EZAPPLICATION_H

#include <QObject>

#include "../global/ezcrud_global.h"

class QSqlDatabase;

namespace crud {

class LoginDialog;
class Session;
class User;

class EZCRUD_EXPORT EZApplication : public QObject
{
    Q_OBJECT
public:
    static EZApplication * instance();
    const QString & applicationName() const;
    void setApplicationName(const QString &appName);
protected:
    explicit EZApplication(QObject *parent = 0);
    ~EZApplication();
private:
    LoginDialog *m_loginDialog;
    QString m_applicationName;
    // Signals & Slots
public slots:
    void start();

private slots:
    void onLoginSuccessFull(User *user, QSqlDatabase *db);
    void onLoginCancelled();
};

} // namespace crud

#endif // CRUD_EZAPPLICATION_H
