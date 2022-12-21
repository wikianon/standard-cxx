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

#ifndef CRUD_CRUDNAMESPACE_H
#define CRUD_CRUDNAMESPACE_H

#include <QObject>
#include <QtCore>

#include "../global/ezcrud_global.h"

namespace crud_sqlite
{
    class Revision;
}

namespace crud {

class EZCRUD_EXPORT EasyCrud
{
public:
    EasyCrud();
    enum CRUDFlag
    {
        None   = 0x0,
        Watch  = 0x1,
        Insert = Watch | 0x2,
        Update = Watch | 0x4,
        Delete = Watch | 0x8
    };

    Q_DECLARE_FLAGS(CRUDFlags, CRUDFlag)
    Q_ENUMS(crud::State)
    Q_ENUMS(crud::WidgetType)
};
Q_DECLARE_OPERATORS_FOR_FLAGS(EasyCrud::CRUDFlags)


extern QMap<QString, EasyCrud::CRUDFlags> userPriviledges;

extern QSharedPointer<crud_sqlite::Revision> profile;

EZCRUD_EXPORT void loadProfile(const QString &);

EZCRUD_EXPORT EasyCrud::CRUDFlags getFlags(int);
EZCRUD_EXPORT EasyCrud::CRUDFlags getAllFlags();


typedef QPair<QString, int > FlagPair;
EZCRUD_EXPORT QList< FlagPair > &flagInfoList();


} // namespace crud

#endif // CRUD_CRUDNAMESPACE_H
