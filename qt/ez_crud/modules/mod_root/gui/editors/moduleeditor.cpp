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

// ****************************************************
// * Codigo Generado por DAOBuilder                   *
// * Fecha: 21/08/2012 15:08:45                       *
// * Luis Valdes 2012 - Todos los Derechos Reservados *
// ****************************************************

#include <QComboBox>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>

#include <core/session.h>

#include "moduleeditor.h"

namespace crud {

ModuleEditor::ModuleEditor(QWidget *parent) :
    CustomEditor(parent)
{
    QSqlDatabase *db = crud::Session::instance()->systemDatabase();
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery("select id, moduleName, moduleMenu from module ", *db);

    model->setHeaderData(0, Qt::Horizontal, tr("Id"));
    model->setHeaderData(1, Qt::Horizontal, tr("Nombre del Module"));
    model->setHeaderData(2, Qt::Horizontal, tr("Nombre del Men�"));

    descripcionComboBox()->setModel(model);
    descripcionComboBox()->setModelColumn(1);

    if(model->query().lastError().isValid())
        qDebug() << model->query().lastError().text();
}

} // namespace crud

