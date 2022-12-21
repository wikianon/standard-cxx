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

#include <gui/mainwindow.h>
#include <gui/mdiarea.h>
#include <gui/mdiwidget.h>

#include "modulegeneric.h"
#include "gui/crud/categoriacrud.h"
#include "gui/crud/personacrud.h"

namespace crud {

ModuleGeneric::ModuleGeneric(QObject *parent) :
    BaseModule(parent),
    m_moduleName("mod_generic"),
    m_categoriaCrud(0),
    m_personaCrud(0)
{
    setObjectName(m_moduleName);
}


QString &ModuleGeneric::moduleName()
{
    return m_moduleName;
}

int ModuleGeneric::moduleVersion()
{
    return 000100;
}

void ModuleGeneric::loadMenu()
{
    m_categoriaCrud = getWidgetHandler("CategoriaCrud", KRegistro);
    if(m_categoriaCrud){
        connect(m_categoriaCrud->action(), SIGNAL(triggered()), SLOT(onCategoriaCrudClicked()));
    }
    m_personaCrud = getWidgetHandler("PersonaCrud", KRegistro);
    if(m_personaCrud){
        connect(m_personaCrud->action(), SIGNAL(triggered()), SLOT(onPersonaCrudClicked()));
    }
}

void ModuleGeneric::onCategoriaCrudClicked()
{
    if(m_categoriaCrud->mdiWidget()){
        m_categoriaCrud->mdiWidget()->setFocus();
    }else{
        EasyCrud::CRUDFlags flags = m_categoriaCrud->flags();
        QWidget *widget = new crud::CategoriaCrud(flags, m_mainWindow);
        m_mainWindow->mdiArea()->addSubWindow(m_categoriaCrud, widget);
        widget->show();
    }
}

void ModuleGeneric::onPersonaCrudClicked()
{
    if(m_personaCrud->mdiWidget()){
        m_personaCrud->mdiWidget()->setFocus();
    }else{
        EasyCrud::CRUDFlags flags = m_personaCrud->flags();
        QWidget *widget = new PersonaCrud(flags, m_mainWindow);
        m_mainWindow->mdiArea()->addSubWindow(m_personaCrud, widget);
        widget->show();
    }
}

} // namespace crud

Q_EXPORT_PLUGIN2(mod_generic, crud::ModuleGeneric)
