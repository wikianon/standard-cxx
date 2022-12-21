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

#ifndef CRUD_MODULECRUD_H
#define CRUD_MODULECRUD_H

#include <gui/basecrud.h>
#include <data/object/module.h>

namespace crud {

class EZCRUD_EXPORT ModuleCrud : public BaseCRUD
{
    Q_OBJECT
public:
    ModuleCrud(crud::EasyCrud::CRUDFlags &crudFlags, QWidget *parent = 0);
    bool insert(crud::ObjectBase *registro);
    bool save(crud::ObjectBase *registro);
    crud::ObjectBase * getRegistro(qlonglong id = 0);
    EntityView * getWidget();
    QString formName();
    QString windowTitle();

protected:
    QSqlDatabase *getDatabase();

};

} // namespace crud

#endif // CRUD_MODULECRUD_H
