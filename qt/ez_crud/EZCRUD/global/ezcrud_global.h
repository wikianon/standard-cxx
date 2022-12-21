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

#ifndef EZCRUD_GLOBAL_H
#define EZCRUD_GLOBAL_H

#include <QtCore/qglobal.h>
#include "../core/constants.h"
#include "../core/algorithms.h"

#if defined(EZCRUD_LIBRARY)
#  define EZCRUD_EXPORT Q_DECL_EXPORT
#else
#  define EZCRUD_EXPORT Q_DECL_IMPORT
#endif

#endif // EZCRUD_GLOBAL_H
