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

#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QMdiArea>
#include <QMdiSubWindow>

#include <core/session.h>
#include <core/crudnamespace.h>
#include <gui/mainwindow.h>
#include <data/object/module.h>
#include <data/object/widget.h>
#include <gui/mdiarea.h>
#include <gui/mdiwidget.h>

#include "moduleroot.h"
#include "gui/crud/usercrud.h"
#include "gui/crud/modulecrud.h"
#include "gui/crud/widgetcrud.h"
#include "gui/crud/organizationcrud.h"

namespace crud {

ModuleRoot::ModuleRoot(QObject *parent) :
    BaseModule(parent),
    m_moduleName("mod_root"),
    m_userCrud(0),
    m_moduleCrud(0),
    m_widgetCrud(0),
    m_organizationCrud(0)
{
}

QString &ModuleRoot::moduleName()
{
    return m_moduleName;
}

int ModuleRoot::moduleVersion()
{
    return 000100;
}

void ModuleRoot::loadRootModule(MainWindow *mainWindow)
{
    // Se recorren todos los modulos activos del sistema
    m_mainWindow = mainWindow;
    foreach(Module *mod, Session::instance()->moduleList()){
        if(mod->moduleName().toString() == "mod_root"){
            foreach(Widget *wid, Session::instance()->widgetList()){
                if(wid->idModule() == mod->id()){
                    WidgetHandler *widgetHd =  new WidgetHandler(wid->widgetName().toString(),
                                                                 wid->widgetMenu().toString(),
                                                                 0,
                                                                 m_mainWindow);
                    switch(wid->widgetType().toInt()){
                    case KRegistro:
                        m_registros.append(widgetHd);
                        break;
                    case KMovimiento:
                        m_movimientos.append(widgetHd);
                        break;
                    case KInforme:
                        m_informes.append(widgetHd);
                        break;
                    case KConfiguracion:
                        m_configuraciones.append(widgetHd);
                        break;
                    }
                }
            }
        }
        continue;
    }

    // Si tiene por lo menos 1 elemento en cualquiera de los grupos
    if((m_registros.count() + m_movimientos.count() + m_informes.count() + m_configuraciones.count()) > 0){
        QMenu *menu = new QMenu(tr("Administrar"), mainWindow);
        if(m_registros.count() > 0){
            QMenu *registros = menu->addMenu("Registros");
            foreach(WidgetHandler *widgetHd, m_registros){
                registros->addAction(widgetHd->action());
            }
        }
        if(m_movimientos.count() > 0){
            QMenu *movimientos = menu->addMenu("Movimientos");
            foreach(WidgetHandler *widgetHd, m_movimientos){
                movimientos->addAction(widgetHd->action());
            }
        }
        if(m_informes.count() > 0){
            QMenu *informes = menu->addMenu("Informes");
            foreach(WidgetHandler *widgetHd, m_informes){
                informes->addAction(widgetHd->action());
            }
        }
        if(m_configuraciones.count() > 0){
            QMenu *configuraciones = menu->addMenu("Configuraciones");
            foreach(WidgetHandler *widgetHd, m_configuraciones){
               configuraciones->addAction(widgetHd->action());
            }
        }
        m_mainWindow->setMenuRoot(menu);
    }
}

void ModuleRoot::loadMenu()
{
    m_userCrud = getWidgetHandler("UserCrud", KRegistro);
    if(m_userCrud){
        connect(m_userCrud->action(), SIGNAL(triggered()), SLOT(onUserCrudClicked()));
    }
    m_moduleCrud = getWidgetHandler("ModuleCrud", KRegistro);
    if(m_moduleCrud){
        connect(m_moduleCrud->action(), SIGNAL(triggered()), SLOT(onModuleCrudClicked()));
    }
    m_widgetCrud = getWidgetHandler("WidgetCrud", KRegistro);
    if(m_widgetCrud){
        connect(m_widgetCrud->action(), SIGNAL(triggered()), SLOT(onWidgetCrudClicked()));
    }
    m_organizationCrud = getWidgetHandler("OrganizationCrud", KRegistro);
    if(m_organizationCrud){
        connect(m_organizationCrud->action(), SIGNAL(triggered()), SLOT(onOrganizationCrudClicked()));
    }
}

void ModuleRoot::onUserCrudClicked()
{
    if(m_userCrud->mdiWidget()){
        m_userCrud->mdiWidget()->setFocus();
    }else{
        crud::EasyCrud::CRUDFlags flags = crud::getAllFlags();
        QWidget *widget = new crud::UserCrud(flags, m_mainWindow);
        m_mainWindow->mdiArea()->addSubWindow(m_userCrud, widget);
        widget->show();
    }
}

void ModuleRoot::onModuleCrudClicked()
{
    if(m_moduleCrud->mdiWidget()){
        m_moduleCrud->mdiWidget()->setFocus();
    }else{
        crud::EasyCrud::CRUDFlags flags = crud::getAllFlags();
        QWidget *widget = new crud::ModuleCrud(flags, m_mainWindow);
        m_mainWindow->mdiArea()->addSubWindow(m_moduleCrud, widget);
        widget->show();
    }
}

void ModuleRoot::onWidgetCrudClicked()
{
    if(m_widgetCrud->mdiWidget()){
        m_widgetCrud->mdiWidget()->setFocus();
    }else{
        crud::EasyCrud::CRUDFlags flags = crud::getAllFlags();
        QWidget *widget = new crud::WidgetCrud(flags, m_mainWindow);
        m_mainWindow->mdiArea()->addSubWindow(m_widgetCrud, widget);
        widget->show();
    }
}

void ModuleRoot::onOrganizationCrudClicked()
{
    if(m_organizationCrud->mdiWidget()){
        m_organizationCrud->mdiWidget()->setFocus();
    }else{
        crud::EasyCrud::CRUDFlags flags = crud::getAllFlags();
        QWidget *widget = new crud::OrganizationCrud(flags, m_mainWindow);
        m_mainWindow->mdiArea()->addSubWindow(m_organizationCrud, widget);
        widget->show();
    }
}

} // namespace crud


Q_EXPORT_PLUGIN2(mod_root, crud::ModuleRoot)
