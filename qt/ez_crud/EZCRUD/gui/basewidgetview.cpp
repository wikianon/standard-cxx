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

#include <QDebug>

#include "basewidgetview.h"

#include "../data/entityclass.h"
#include "../core/dynamic/dynamicbinder.h"

BaseWidgetView::BaseWidgetView(QWidget *parent) :
    QWidget(parent),
    m_state(crud::KWatching),
    m_objectBase(),
    m_dynamicBinders()
{

}
BaseWidgetView::BaseWidgetView(crud::State state, QWidget *parent) :
    QWidget(parent),
    m_state(state),
    m_objectBase(),
    m_dynamicBinders()
{
}

crud::ObjectBase * BaseWidgetView::model()
{
    return m_objectBase;
}

void BaseWidgetView::setModel(crud::ObjectBase *model)
{
    m_objectBase = model;
}

void BaseWidgetView::setModelValues()
{
    foreach(DynamicBinder *dynamicBinder, m_dynamicBinders){
        dynamicBinder->updateModel();
    }
}

void BaseWidgetView::setViewValues()
{
    foreach(DynamicBinder *dynamicBinder, m_dynamicBinders){
        dynamicBinder->updateView();
    }
}


void BaseWidgetView::setState(crud::State state)
{
    setViewValues();
    QList<QWidget *> children = qFindChildren<QWidget *>(this);
    m_state = state;
    switch(m_state){
    case crud::KWatching:
    {
        this->setProperty("readOnly", true);
        foreach(QWidget *widget, children){
            widget->setProperty("readOnly", true);
        }
        break;
    }
    case crud::KInserting:
    case crud::KUpdating:
    {
        this->setProperty("readOnly", false);
        foreach(QWidget *widget, children){
            widget->setProperty("readOnly", false);
        }
        break;
    }
    }
    emit viewStateChanged(m_state);
}
