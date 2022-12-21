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
#include <QPluginLoader>
#include <QSettings>
#include <QDir>
#include <QSqlDatabase>

#include "../core/modules/basemodule.h"
#include "../core/connectionpool.h"

#include "session.h"
#include "../core/settings.h"
#include "../core/encryption.h"
#include "../data/object/user.h"
#include "../data/object/revision.h"
#include "../data/object/module_revision.h"
#include "../data/object/module.h"
#include "../data/object/organization.h"

#include "../data/dao/moduledao.h"
#include "../data/dao/widgetdao.h"
#include "../data/dao/revisiondao.h"
#include "../data/dao/organizationdao.h"

#include "../gui/logindialog.h"
#include "../gui/selectserver.h"
#include "../gui/selectorganization.h"

#include "../gui/mainwindow.h"



namespace crud {

Session::Session(QObject *parent) :
    QObject(parent),
    m_sessionStarted(false),
    m_systemDB(0),
    m_organizationDB(0),
    m_user(0),
    m_revision(0),
    m_organization(0),
    m_mainWindow(0),
    m_loadedModuleList(),
    m_moduleList(),
    m_widgetList(),
    m_organizationList()
{

}

crud::User * Session::user()
{
    return m_user;
}

crud::Revision * Session::revision()
{
    return m_revision;
}

Organization *Session::organization()
{
    return m_organization;
}

QList<BaseModule *> & Session::loadedModuleList()
{
    return m_loadedModuleList;
}

QList<Module *> &Session::moduleList()
{
    return m_moduleList;
}

QList<Widget *> &Session::widgetList()
{
    return m_widgetList;
}

QList<Organization *> &Session::organizationList()
{
    return m_organizationList;
}

QList<Revision *> &Session::revisionList()
{
    return m_revisionList;
}

void Session::loadModules()
{
    QDir pluginsDir(qApp->applicationDirPath());
    pluginsDir.cd("modules");

    foreach (QString fileName, pluginsDir.entryList(QDir::Files)){
        QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = loader.instance();

        if (plugin){
            crud::BaseModule *baseModule = qobject_cast<crud::BaseModule *>(plugin);
            if(baseModule){
                m_loadedModuleList.append(baseModule);
            }
        }
    }
}

void Session::loadUserProfile()
{
    if((m_user->id().toLongLong() == 1) && (m_user->loginName().toString() == "ROOT")){
        loadRootProfile();
    }else{
        loadRevisionProfile();
    }
}

void Session::loadRootProfile()
{
    foreach(BaseModule *baseModule, m_loadedModuleList){
        if(baseModule->moduleName() == "mod_root"){
            baseModule->prepareModule();
            baseModule->loadRootModule(m_mainWindow);
            baseModule->loadMenu();
            return;
        }
    }
}

void Session::loadRevisionProfile()
{
    if(m_revision){
        foreach(BaseModule *baseModule, m_loadedModuleList){
            foreach(ModuleRevision *modRev, m_revision->moduleList()){
                if( (modRev->module()->moduleName() == baseModule->moduleName()) && (baseModule->moduleName() != "mod_root")){
                    baseModule->prepareModule();
                    baseModule->loadModule(m_mainWindow);
                    baseModule->loadMenu();
                }
            }
        }
    }
}

void Session::selectOrganization()
{
    qlonglong idOrganization = SelectOrganization::selectOrg();
    if(idOrganization != -1)
        setOrganization(idOrganization);
}

void Session::setOrganization(qlonglong idOrganization)
{
    m_organization = 0;
    foreach(Organization *org, m_organizationList){
        if(org->id().toLongLong() == idOrganization){
            m_organization = org;
            m_organizationDB = ConnectionPool::instance()->organizationConnection(m_organization);
            break;
        }
    }
    if(m_organization){
        foreach(Revision *rev, m_revisionList){
            if(rev->idOrganization().toLongLong() == m_organization->id().toLongLong()){
                m_revision = rev;
                break;
            }
        }
    }
}

void Session::startSession(User *user, QSqlDatabase *systemDB )
{
    // Crear MainWindow
    if(!m_mainWindow){
        m_mainWindow = new MainWindow;
        m_mainWindow->readSettings();
        connect(m_mainWindow, SIGNAL(loginOut()), SLOT(endSession()));
    }

    m_sessionStarted = true;
    m_systemDB = systemDB;
    m_organizationDB = 0;
    m_moduleList = ModuleDAO::instance()->getAll();
    m_widgetList = WidgetDAO::instance()->getAll();
    m_user = user;
    //    m_organizationList = OrganizationDAO::instance()->getByUser(m_user->id().toLongLong());
    m_revisionList = RevisionDAO::instance()->getLatestRevision(m_user->id().toLongLong());

    // If the Revision has at leas one module, the session will load the Revision Organization
    foreach(Revision *rev, m_revisionList){
        if(rev->moduleList().count() > 0){
            Organization *org = OrganizationDAO::instance()->getById(rev->idOrganization().toLongLong());
            m_organizationList.append(org);
        }
    }
    // If there is more than one Organization for the User it asks the user to select one
    if(m_organizationList.count() > 1){
        selectOrganization();
    }else if(m_organizationList.count() == 1){
        setOrganization(m_organizationList.at(0)->id().toLongLong());
    }

    loadModules();
    loadUserProfile();
    m_mainWindow->show();
}

void Session::endSession()
{
    m_sessionStarted = false;
    // Cleaning up heap
    m_user->deleteLater();
    m_mainWindow->deleteLater();
    // Setting pointers to zero
    m_systemDB = 0;
    m_organizationDB = 0;
    m_user = 0;
    m_mainWindow = 0;

    // Clear Loaded Modules, Organizations, Modules and Widgets
    deleteAll(m_loadedModuleList);
    deleteAll(m_revisionList);
    deleteAll(m_organizationList);
    deleteAll(m_moduleList);
    deleteAll(m_widgetList);
    // Clear zero pointer elements of list
    m_loadedModuleList.clear();
    m_revisionList.clear();
    m_organizationList.clear();
    m_moduleList.clear();
    m_widgetList.clear();
    emit sessionEnded();
}

Session *Session::instance()
{
    static Session *instance = 0;
    if(!instance){
        instance = new Session(qApp);
    }
    return instance;
}

bool Session::sessionStarted()
{
    return m_sessionStarted;
}

QSqlDatabase *Session::systemDatabase()
{
    return m_systemDB;
}

QSqlDatabase *Session::organizationDatabase()
{
    return m_organizationDB;
}

} // namespace crud
