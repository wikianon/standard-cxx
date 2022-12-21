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

#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QMdiArea>

#include "../../gui/mainwindow.h"
#include "../../gui/basecrud.h"

#include "../../data/object/revision.h"
#include "../../data/object/module_revision.h"
#include "../../data/object/widget_revision.h"
#include "../../data/object/module.h"
#include "../../data/object/widget.h"

#include "../session.h"
#include "basemodule.h"



namespace crud {

BaseModule::BaseModule(QObject *parent) :
    QObject(parent),
    m_mainWindow(0),
    m_registros(),
    m_movimientos(),
    m_informes(),
    m_configuraciones()
{

}

void BaseModule::prepareModule()
{
    m_registros.clear();
    m_movimientos.clear();
    m_informes.clear();
    m_configuraciones.clear();
}

void BaseModule::loadRootModule(crud::MainWindow *mainWindow)
{
    qDebug() << Q_FUNC_INFO;
}

void BaseModule::loadModule(crud::MainWindow  *mainWindow)
{
    // Se recorren todos los modulos activos de la revision seleccionada por el usuario
    m_mainWindow = mainWindow;
    if(Session::instance()->revision()){
        foreach(ModuleRevision *mod, Session::instance()->revision()->moduleList()){
            if(mod->module()->moduleName().toString() == moduleName()){
                foreach(WidgetRevision *wid, Session::instance()->revision()->widgetList()){
                    if(wid->widget()->idModule() == mod->idModule()){
                        WidgetHandler *widgetHd =  new WidgetHandler(wid->widget()->widgetName().toString(),
                                                                     wid->widget()->widgetMenu().toString(),
                                                                     wid->accessMode().toInt(),
                                                                     m_mainWindow);
                        switch(wid->widget()->widgetType().toInt()){
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
                // Si tiene por lo menos 1 elemento en cualquiera de los grupos
                if((m_registros.count() + m_movimientos.count() + m_informes.count() + m_configuraciones.count()) > 0){
                    QMenu *menu = new QMenu(mod->module()->moduleMenu().toString(), mainWindow);
                    if(m_registros.count() > 0){
                        QMenu *registros = menu->addMenu(tr("Registros"));
                        foreach(WidgetHandler *widgetHd, m_registros){
                            registros->addAction(widgetHd->action());
                        }
                    }
                    if(m_movimientos.count() > 0){
                        QMenu *movimientos = menu->addMenu(tr("Movimientos"));
                        foreach(WidgetHandler *widgetHd, m_movimientos){
                            movimientos->addAction(widgetHd->action());
                        }
                    }
                    if(m_informes.count() > 0){
                        QMenu *informes = menu->addMenu(tr("Informes"));
                        foreach(WidgetHandler *widgetHd, m_informes){
                            informes->addAction(widgetHd->action());
                        }
                    }
                    if(m_configuraciones.count() > 0){
                        QMenu *configuraciones = menu->addMenu(tr("Configuraciones"));
                        foreach(WidgetHandler *widgetHd, m_configuraciones){
                            configuraciones->addAction(widgetHd->action());
                        }
                    }
                    m_mainWindow->menuList().append(menu);
                }
            }
        }
    }

}

void BaseModule::loadMenu()
{
    qDebug() << Q_FUNC_INFO;
}

crud::WidgetHandler *BaseModule::getWidgetHandler(const QString &nombreClase, WidgetType widgetType)
{
    QList<crud::WidgetHandler *> *lista = 0;
    switch(widgetType){
    case KRegistro:
        lista = &m_registros;
        break;
    case KMovimiento:
        lista = &m_movimientos;
        break;
    case KInforme:
        lista = &m_informes;
        break;
    case KConfiguracion:
        lista = &m_configuraciones;
        break;
    }
    foreach(crud::WidgetHandler *wh, *lista){
        if(wh->objectName() ==  nombreClase)
            return wh;
    }
    return 0;
}

} // namespace crud
