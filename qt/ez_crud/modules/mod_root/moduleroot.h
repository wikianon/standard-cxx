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

#ifndef CRUD_MODULEROOT_H
#define CRUD_MODULEROOT_H

#include <core/modules/basemodule.h>
#include <core/modules/widgethandler.h>

namespace crud{

class EZCRUD_EXPORT ModuleRoot : public BaseModule
{
    Q_OBJECT
public:
    ModuleRoot(QObject *parent=0);
    QString & moduleName();
    int moduleVersion();
    void loadRootModule(crud::MainWindow  *mainWindow);
    void loadMenu();
private:
    QString m_moduleName;
    crud::WidgetHandler *m_userCrud;
    crud::WidgetHandler *m_moduleCrud;
    crud::WidgetHandler *m_widgetCrud;
    crud::WidgetHandler *m_organizationCrud;

private slots:
    void onUserCrudClicked();
    void onModuleCrudClicked();
    void onWidgetCrudClicked();
    void onOrganizationCrudClicked();
};

} // namespace crud

#endif // CRUD_MODULEROOT_H
