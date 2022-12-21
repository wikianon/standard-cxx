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

#ifndef CRUD_MODULEGENERIC_H
#define CRUD_MODULEGENERIC_H

#include <core/modules/basemodule.h>
#include <core/modules/widgethandler.h>

class QMenu;

namespace crud {

class ModuleGeneric : public BaseModule
{
    Q_OBJECT
public:
    explicit ModuleGeneric(QObject *parent = 0);
    QString & moduleName();
    int moduleVersion();
    void loadMenu();

private:
    QString m_moduleName;
//    CatastroPersona *m_catastroPersona;
    WidgetHandler *m_categoriaCrud;
    WidgetHandler *m_personaCrud;
private slots:
    void onCategoriaCrudClicked();
    void onPersonaCrudClicked();

};

} // namespace crud

#endif // CRUD_MODULEGENERIC_H
