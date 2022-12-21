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

#include "dynamicbinder.h"

#include "binder.h"

DynamicBinder::DynamicBinder(crud::ObjectBase *model, Binder *binder, QObject *parent) :
    QObject(parent),
    m_model(model),
    m_binder(binder)
{

}

void DynamicBinder::updateModel()
{
    setModelValue(widgetValue());
}

void DynamicBinder::updateView()
{
    setWidgetValue(modelValue());
}

QVariant DynamicBinder::value()
{
    return modelValue();
}

QString DynamicBinder::propertyName()
{
    return m_binder->objectName();
}

QVariant DynamicBinder::modelValue()
{
    return m_model->property(propertyName().toLocal8Bit());
}

void DynamicBinder::setModelValue(QVariant value )
{
    m_model->setProperty(propertyName().toLocal8Bit(), value);
}

QVariant DynamicBinder::widgetValue()
{
    return m_binder->value();
}

void DynamicBinder::setWidgetValue(QVariant value)
{
    if((m_binder->objectName() == "id") && value.toInt() == -1){
        m_binder->setValue("");
    }else{
        m_binder->setValue(value);
    }

}
