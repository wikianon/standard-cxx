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

#include <QCloseEvent>

#include "../core/settings.h"

#include "mdiwidget.h"
#include "basecrud.h"

MdiWidget::MdiWidget(WidgetHandler *widgetHandler, QWidget *parent, Qt::WindowFlags flags) :
    QMdiSubWindow(parent, flags),
    m_widgetHandler(widgetHandler)
{
    widgetHandler->setMdiWidget(this);
}

bool MdiWidget::readSettings()
{
    return restoreGeometry(Settings::qSettings()->value(objectName()+"/geometry").toByteArray());
}

void MdiWidget::closeEvent(QCloseEvent *closeEvent)
{
    BaseCRUD *wid = qobject_cast<BaseCRUD *>(widget());
    if(wid){
        if(wid->canClose()){
            Settings::qSettings()->setValue(objectName()+"/geometry", saveGeometry());
            m_widgetHandler->closeWidget();
//            closeEvent->accept();
        }else{
            closeEvent->ignore();
        }
    }else{
//        closeEvent->accept();
        m_widgetHandler->closeWidget();
    }
}
