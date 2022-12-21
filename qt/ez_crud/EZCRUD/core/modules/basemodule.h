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

#ifndef BASEMODULE_H
#define BASEMODULE_H

#include <QObject>
#include <QMap>
#include <QPair>


#include "global/ezcrud_global.h"
#include "../crudnamespace.h"
#include "plugininterface.h"
#include "widgethandler.h"

class QMenu;
class QAction;

namespace crud {

class MainWindow ;

class EZCRUD_EXPORT BaseModule : public QObject, public PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(crud::PluginInterface)
public:
    explicit BaseModule(QObject *parent = 0);
    virtual QString & moduleName()=0;
    virtual int moduleVersion()=0;
    virtual void prepareModule();
    virtual void loadRootModule(MainWindow  *mainWindow) ;
    virtual void loadModule(MainWindow  *mainWindow);
    virtual void loadMenu();

protected:
    crud::WidgetHandler * getWidgetHandler(const QString &, WidgetType);
    MainWindow *m_mainWindow;
    QList<WidgetHandler *> m_registros;
    QList<WidgetHandler *> m_movimientos;
    QList<WidgetHandler *> m_informes;
    QList<WidgetHandler *> m_configuraciones;
};

} // namespace crud

#endif // BASEMODULE_H
