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

#ifndef CRUD_SYSTEM_SESSION_H
#define CRUD_SYSTEM_SESSION_H

#include <QObject>
#include <QSharedPointer>

#include "../global/ezcrud_global.h"

class QSqlDatabase;

namespace crud {

class BaseModule;
class User;
class Revision;
class MainWindow;
class Module;
class Widget;
class Organization;

class  EZCRUD_EXPORT Session : public QObject
{
    Q_OBJECT
public:
    static Session * instance();

    bool sessionStarted();

    QSqlDatabase * systemDatabase();
    QSqlDatabase * organizationDatabase();

    User * user();
    Revision * revision();
    Organization * organization();

    QList<BaseModule *> & loadedModuleList();
    QList<Module *> & moduleList();
    QList<Widget *> & widgetList();
    QList<Organization *> & organizationList();
    QList<Revision *> & revisionList();

    void startSession(User *user, QSqlDatabase *systemDB);


protected:
    explicit Session(QObject *parent = 0);

private:
    bool m_sessionStarted;
    QSqlDatabase *m_systemDB;
    QSqlDatabase *m_organizationDB;

    User *m_user;
    Revision *m_revision;
    Organization *m_organization;
    QList<Revision *> m_revisionList;

    MainWindow *m_mainWindow;

    QList<BaseModule *> m_loadedModuleList;
    QList<Module *> m_moduleList;
    QList<Widget *> m_widgetList;    
    QList<Organization *> m_organizationList;

    void loadModules();
    void loadUserProfile();
    void loadRootProfile();
    void loadRevisionProfile();
    void selectOrganization();

public slots:
    void setOrganization(qlonglong idOrganization);

private slots:
    void endSession();
signals:
    void sessionEnded();

};

} // namespace crud

#endif // CRUD_SYSTEM_SESSION_H
