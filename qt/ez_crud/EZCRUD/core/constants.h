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

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QMetaType>

namespace crud{

enum State {KWatching = 1,
            KInserting = 2,
            KUpdating = 3} ;

enum WidgetType {
    KRegistro = 1,
    KMovimiento = 2,
    KInforme = 3,
    KConfiguracion = 4
};



} // namespace crud


Q_DECLARE_METATYPE(crud::State)
Q_DECLARE_METATYPE(crud::WidgetType)

//qRegisterMetaType<crud::WidgetType>("crud::WidgetType");

#endif // CONSTANTS_H
