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

#ifndef CRUD_SYSTEM_WIDGETHANDLER_H
#define CRUD_SYSTEM_WIDGETHANDLER_H

#include <QObject>
#include <QMap>
#include <QPair>

#include "global/ezcrud_global.h"
#include "../crudnamespace.h"
#include "plugininterface.h"

class QAction;

class MdiWidget;

namespace crud {

class EZCRUD_EXPORT WidgetHandler : public QObject
{
    Q_OBJECT
public:
    explicit WidgetHandler(const QString &nombreClase, const QString &nombreMenu, int accessMode = 0, QObject *parent = 0);
    QAction * action();
    void closeWidget();
    MdiWidget * mdiWidget();
    void setMdiWidget(MdiWidget *);
    EasyCrud::CRUDFlags flags();
private:
    QAction *m_action;
    MdiWidget *m_mdiWidget;
    int m_accesMode;
};

} // namespace crud

#endif // CRUD_SYSTEM_WIDGETHANDLER_H
