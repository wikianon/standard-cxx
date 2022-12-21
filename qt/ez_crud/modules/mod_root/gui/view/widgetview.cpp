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

#include "widgetview.h"
#include "ui_widgetview.h"

#include <core/constants.h>

namespace crud {

WidgetView::WidgetView(QWidget *parent) :
    EntityView(parent),
    ui(new Ui::WidgetView)
{
    ui->setupUi(widgetView());

    ui->buttonGroup->setId(ui->rbRegistros, KRegistro);
    ui->buttonGroup->setId(ui->rbMovimientos, KMovimiento);
    ui->buttonGroup->setId(ui->rbInformes, KInforme);
    ui->buttonGroup->setId(ui->rbConfiguraciones, KConfiguracion);

}

WidgetView::~WidgetView()
{
    delete ui;
}

bool WidgetView::readOnly()
{
    return ! ui->groupBox->isEnabled();
}

void WidgetView::setReadOnly(bool value)
{
    ui->groupBox->setEnabled(! value);
}

QVariant WidgetView::widgetType()
{
    return ui->buttonGroup->checkedId();
}

void WidgetView::setWidgetType(QVariant value)
{
    switch(value.toInt())
    {
    case KRegistro:
        ui->rbRegistros->setChecked(true);
        break;
    case KMovimiento:
        ui->rbMovimientos->setChecked(true);
        break;
    case KInforme:
        ui->rbInformes->setChecked(true);
        break;
    case KConfiguracion:
        ui->rbConfiguraciones->setChecked(true);
        break;
    }
}

} // namespace crud
