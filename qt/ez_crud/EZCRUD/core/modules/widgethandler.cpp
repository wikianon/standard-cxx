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

#include <QAction>

#include "widgethandler.h"
#include "../../gui/mdiwidget.h"

#include "../crudnamespace.h"

namespace crud {

WidgetHandler::WidgetHandler(const QString &nombreClase, const QString &nombreMenu, int accessMode,  QObject *parent) :
    QObject(parent),
    m_accesMode(accessMode)
{
    setObjectName(nombreClase);
    m_action = new QAction(nombreMenu, this);
    m_mdiWidget = 0;
}

QAction *WidgetHandler::action()
{
    return m_action;
}

void WidgetHandler::closeWidget()
{
    m_mdiWidget->setVisible(false);
    m_mdiWidget->deleteLater();
    m_mdiWidget = 0;
}

MdiWidget *WidgetHandler::mdiWidget()
{
    return m_mdiWidget;
}

void WidgetHandler::setMdiWidget(MdiWidget *mdiWidget)
{
    m_mdiWidget = mdiWidget;
}

EasyCrud::CRUDFlags WidgetHandler::flags()
{
    return getFlags(m_accesMode);
}

} // namespace crud
