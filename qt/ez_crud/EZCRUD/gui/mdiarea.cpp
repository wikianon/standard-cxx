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

#include "mdiarea.h"

#include "mdiwidget.h"

#include "../core/settings.h"

MdiArea::MdiArea(QWidget *parent) :
    QMdiArea(parent)
{
}

MdiWidget * MdiArea::addSubWindow(WidgetHandler *widgetHandler, QWidget *widget)
{
    QString className = widget->metaObject()->className();
    MdiWidget *mdiWidget = new MdiWidget(widgetHandler);
    mdiWidget->setObjectName(className);
    mdiWidget->setWidget(widget);
    mdiWidget->setWindowIcon(widget->windowIcon());
    mdiWidget->setAttribute(Qt::WA_DeleteOnClose);

    QMdiArea::addSubWindow(mdiWidget);

    // Read the saved settings of the widget
    if(!mdiWidget->readSettings()){
        // Calculate MdiWidget Location to make a cascade in creation order
        QList<QMdiSubWindow *>  mdiWidgetList = subWindowList();
        int index = mdiWidgetList.indexOf(static_cast<QMdiSubWindow *>(mdiWidget));
        int margin = (index + 1) * 25;
        mdiWidget->setGeometry(margin, margin, mdiWidget->width(), mdiWidget->height());
    }


    // TODO: here I have to implement the procedures needed to make the application capable of handling the maximize event of the MdiWidgets, so that when a widget maximize the mdiarea becomes a tabWidget
//    connect(mdiWidget, SIGNAL(windowStateChanged(Qt::WindowStates, Qt::WindowStates)),
//            this, SLOT(onSubWindowMaximized(Qt::WindowStates, Qt::WindowStates)));
    return mdiWidget;
}

void MdiArea::onSubWindowMaximized(Qt::WindowStates oldState, Qt::WindowStates newState)
{
    // TODO: here I have to implement the procedures needed to make the application capable of handling the maximize event of the MdiWidgets, so that when a widget maximize the mdiarea becomes a tabWidget
//    WindowNoState    = 0x00000000,
//    WindowMinimized  = 0x00000001,
//    WindowMaximized  = 0x00000002,
//    WindowFullScreen = 0x00000004,
//    WindowActive     = 0x00000008
//    if (oldState & Qt::WindowMaximized)
//    {
//        setViewMode(QMdiArea::SubWindowView);
//    }
//    else if(newState & Qt::WindowMaximized)
//    {
//        setViewMode(QMdiArea::TabbedView);
//    }
//    if (newState & Qt::WindowMaximized)
//    {
//        setViewMode(QMdiArea::TabbedView);
//    }
//    else if(newState & Qt::WindowMinimized)
//    {
//        setViewMode(QMdiArea::SubWindowView);
//    }
}
