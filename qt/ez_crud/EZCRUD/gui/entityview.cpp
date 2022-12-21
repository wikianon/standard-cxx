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

#include <QTabWidget>
#include <QMessageBox>

#include "entityview.h"
#include "ui_entityview.h"

#include "../data/entityclass.h"
#include "core/dynamic/dynamicbinder.h"

EntityView::EntityView(QWidget *parent) :
    BaseWidgetView(parent),
    ui(new Ui::EntityView),
    m_widgetView()
{
    ui->setupUi(this);
    ui->id->setReadOnly(true);
    ui->dateTime->setReadOnly(true);

    ui->stateText->setVisible(false);
    ui->labelSituacion->setVisible(false);
}

EntityView::EntityView(crud::State state, QWidget *parent) :
    BaseWidgetView(state, parent),
    ui(new Ui::EntityView),
    m_widgetView()
{
    ui->setupUi(this);
    ui->id->setReadOnly(true);
    ui->dateTime->setReadOnly(true);
}

EntityView::~EntityView()
{
    delete ui;
}

void EntityView::onSaveButtonClicked()
{
    switch(state()){
    case crud::KInserting:
    case crud::KUpdating:
        if(validate())
            emit viewValidated();
        break;
    default:
        break;
    }
}



QWidget * EntityView::widgetView()
{
    return ui->widgetView;
}

void EntityView::setState(crud::State state)
{
    QWidget *qt_tabwidget_stakedwidget = qobject_cast<QWidget *>(parent());
    QTabWidget *parentTab = 0;
    if(qt_tabwidget_stakedwidget)
        parentTab = qobject_cast<QTabWidget *>(qt_tabwidget_stakedwidget->parent());

    setViewValues();
    QList<QWidget *> children = qFindChildren<QWidget *>(ui->widgetView);

    m_state = state;

    switch(m_state){
    case crud::KWatching:
    {
        this->setProperty("readOnly", true);
        foreach(QObject *widget, children)
        {
            widget->setProperty("readOnly", true);
        }
        if(parentTab)
        {
            QString texto(tr("Codigo %1"));
            qlonglong id = model()->id().toLongLong();
            parentTab->setTabText(parentTab->indexOf(this), texto.arg(id));
        }
        break;
    }
    case crud::KInserting:
    {
        this->setProperty("readOnly", false);
        foreach(QObject *widget, children)
        {
            widget->setProperty("readOnly", false);
        }
        if(parentTab)
        {
            QString texto(tr("Nuevo"));
            parentTab->setTabText(parentTab->indexOf(this), texto);
        }
        break;
    }
    case crud::KUpdating:
    {
        this->setProperty("readOnly", false);
        foreach(QObject *widget, children)
        {
            widget->setProperty("readOnly", false);
        }
        if(parentTab)
        {
            QString texto(tr("Actualizando %1"));
            qlonglong id = model()->id().toLongLong();
            parentTab->setTabText(parentTab->indexOf(this), texto.arg(id));
        }
        break;
    }
    }
    emit viewStateChanged(m_state);
}


void EntityView::setModelValues()
{
    foreach(DynamicBinder *dynamicBinder, m_dynamicBinders){
        if(dynamicBinder->propertyName() != "stateText")
            dynamicBinder->updateModel();
    }
}

void EntityView::setViewValues()
{
    foreach(DynamicBinder *dynamicBinder, m_dynamicBinders){
        if((dynamicBinder->propertyName() == "stateText") && (dynamicBinder->value().toInt() > 0)){
            ui->stateText->setVisible(true);
            ui->labelSituacion->setVisible(true);
        }
        dynamicBinder->updateView();
    }
}
