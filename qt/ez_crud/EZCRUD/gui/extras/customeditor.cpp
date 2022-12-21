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

#include <QAbstractItemModel>
#include <QKeyEvent>
#include <QDebug>

#include "customeditor.h"
#include "ui_customeditor.h"
#include "consulta.h"

CustomEditor::CustomEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CustomEditor)
{
    ui->setupUi(this);
}

CustomEditor::~CustomEditor()
{
    delete ui;
}

QVariant CustomEditor::value()
{
    QModelIndex index = ui->m_descripcion->model()->index(ui->m_descripcion->currentIndex(), 0);
    return ui->m_descripcion->model()->data(index);
}

QString CustomEditor::descripcion()
{
    qDebug() << ui->m_descripcion->currentText();
    return ui->m_descripcion->currentText();
}

void CustomEditor::setValue(QVariant value)
{
    QModelIndex index = ui->m_descripcion->model()->index(0,0);
    QModelIndexList matchList = ui->m_descripcion->model()->match(index, Qt::DisplayRole, value, 1, Qt::MatchExactly);
    if(matchList.count() > 0){
        ui->m_codigo->setText(value.toString());
        ui->m_descripcion->setCurrentIndex(matchList.at(0).row());
        emit valueChanged(value.toLongLong());
    }else{
        ui->m_codigo->clear();
        ui->m_descripcion->setCurrentIndex(-1);
        emit valueChanged(-1);
    }
}

void CustomEditor::on_m_descripcion_currentIndexChanged(int index)
{
    if(index != -1)
        ui->m_codigo->setText(ui->m_descripcion->model()->data(ui->m_descripcion->model()->index(index, 0)).toString());
}

void CustomEditor::on_m_codigo_textEdited(const QString &arg1)
{
    setValue(arg1);
}

QComboBox * CustomEditor::descripcionComboBox()
{
    return ui->m_descripcion;
}

/*capturar F10*/
void CustomEditor::keyPressEvent(QKeyEvent *keyEvent)
{
    if((keyEvent->key() == Qt::Key_F10) && ui->m_codigo->isActiveWindow()  && !ui->m_codigo->isReadOnly())
    {
        qlonglong codigo = Consulta::consultar(ui->m_descripcion->model(), parentWidget());
        if(codigo > 0)
            setValue(codigo);
    }
}


bool CustomEditor::isReadOnly() const
{
    return ui->m_codigo->isReadOnly() && ui->m_descripcion->isEnabled();
}

void CustomEditor::setReadOnly(bool readOnly)
{
    ui->m_codigo->setReadOnly(readOnly);
    ui->m_descripcion->setEnabled(!readOnly);
}

