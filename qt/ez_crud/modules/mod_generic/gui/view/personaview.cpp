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

#include <QButtonGroup>

#include "personaview.h"
#include "ui_personaview.h"

#include "../../data/object/persona.h"
#include "../../data/object/categoria.h"

#include "../../data/model/categoriacheckmodel.h"

namespace crud{

PersonaView::PersonaView(QWidget *parent) :
    EntityView(parent),
    ui(new Ui::PersonaView)
{
    ui->setupUi(widgetView());
    ui->buttonGroup->setId(ui->rbFisica, Persona::KFisica);
    ui->buttonGroup->setId(ui->rbJuridica, Persona::KJuridica);
}

PersonaView::~PersonaView()
{
    delete ui;
}

CategoriaCheckModel *PersonaView::categoriaCheckModel()
{
    return m_categoriaCheckModel;
}

void PersonaView::setState(State state)
{
    switch (state) {
    case KWatching:
        m_categoriaCheckModel->setReadOnly(true);
        ui->bSelectAll->setEnabled(false);
        ui->bSelectNone->setEnabled(false);
        break;
    case KInserting:
    case KUpdating:
        m_categoriaCheckModel->setReadOnly(false);
        ui->bSelectAll->setEnabled(true);
        ui->bSelectNone->setEnabled(true);
        break;
    default:
        Q_ASSERT(false);
        break;
    }
    EntityView::setState(state);
}

void PersonaView::setModel(ObjectBase *model)
{
    BaseWidgetView::setModel(model);
    Persona *persona = qobject_cast<Persona *>(m_objectBase);
    m_categoriaCheckModel = new CategoriaCheckModel(this);
    m_categoriaCheckModel->setPersona(persona);
    connect(ui->bSelectAll, SIGNAL(clicked()), m_categoriaCheckModel, SLOT(selectAll()));
    connect(ui->bSelectNone, SIGNAL(clicked()), m_categoriaCheckModel, SLOT(selectNone()));
    ui->tableView->setModel(m_categoriaCheckModel);
}

bool PersonaView::readOnly()
{
    return ! ui->groupBox->isEnabled();
}

void PersonaView::setReadOnly(bool value)
{
    ui->groupBox->setEnabled(!value);
}

QVariant PersonaView::tipopersona()
{
    return ui->buttonGroup->checkedId();
}

void PersonaView::setTipopersona(QVariant value)
{
    switch(value.toInt()){
    case Persona::KFisica:
        ui->rbFisica->setChecked(true);
        break;
    case Persona::KJuridica:
        ui->rbJuridica->setChecked(true);
        break;
    }
}

} // namespace crud
