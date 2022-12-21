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

#include "crudnamespace.h"

namespace crud {

EasyCrud::EasyCrud()
{
}

QMap<QString, EasyCrud::CRUDFlags> userPriviledges;

QSharedPointer<crud_sqlite::Revision> profile;

void loadProfile(const QString &loginName)
{
    //    profile =QSharedPointer<crud_sqlite::Revision>(crud_sqlite::getLocalProfile(loginName));
}


EasyCrud::CRUDFlags getFlags(int accessMode)
{
    EasyCrud::CRUDFlags flags( EasyCrud::None);
    // Watch
    if(EasyCrud::Watch & accessMode)
        flags = (flags | EasyCrud::CRUDFlags(EasyCrud::Watch));
    // Primero se debe desactivar el bit 0x1 correspondiente a EasyCrud::Watch
    accessMode = accessMode & ~EasyCrud::Watch;
    // Insert
    if(EasyCrud::Insert & accessMode)
        flags = (flags | EasyCrud::Insert);
    // Update
    if(EasyCrud::Update & accessMode)
        flags = (flags | EasyCrud::Update);
    // Delete
    if(EasyCrud::Delete & accessMode)
        flags = (flags | EasyCrud::Delete);
    return flags;
}

EasyCrud::CRUDFlags getAllFlags()
{
    EasyCrud::CRUDFlags flags( EasyCrud::None);
    flags = (flags | EasyCrud::CRUDFlags(EasyCrud::Watch));
    flags = (flags | EasyCrud::Insert);
    flags = (flags | EasyCrud::Update);
    flags = (flags | EasyCrud::Delete);
    return flags;
}

QList< FlagPair > & flagInfoList()
{
    static QList< QPair<QString, int > > flagList;
    if(flagList.count() == 0)
    {
        flagList.append(QPair<QString, int>(QObject::tr("Visualizar"), EasyCrud::Watch));
        flagList.append(QPair<QString, int>(QObject::tr("Insertar"), EasyCrud::Insert));
        flagList.append(QPair<QString, int>(QObject::tr("Actualizar"), EasyCrud::Update));
        flagList.append(QPair<QString, int>(QObject::tr("Eliminar"), EasyCrud::Delete));
    }
    return flagList;
}

} // namespace crud
