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

#include <QObject>
#include <QMetaProperty>

#include "viewfactory.h"
#include "../../gui/basewidgetview.h"
#include "../../gui/entityview.h"

#include "core/dynamic/dynamicbinder.h"
#include "core/dynamic/propertybinder.h"
#include "core/dynamic/widgetbinder.h"

EntityView * ViewFactory::createEntityView(EntityView *view, crud::ObjectBase *model)
{
    EntityView *registroView = view;
    registroView->setModel(model);

    const QMetaObject *modelMetaObject = model->metaObject();

    // i = 1 porque la propiedad en el index '0' es 'objectName'
    for(int i = 1; i < modelMetaObject->propertyCount(); i++)
    {
        QMetaProperty property = modelMetaObject->property(i);
        QString modelPropertyName(property.name());

        QWidget *child = registroView->findChild<QWidget *>(modelPropertyName);
        // Filtra el Widget Correspondiente a la Propiedad
        if(child)
        {
            // El WidgetBinder es el encargado de hacer proveer la interfaz para alterar los
            // datos de los distintos tipos de widgets que soporta la libreria
            WidgetBinder *widgetBind = new WidgetBinder(child, registroView);
            DynamicBinder *binder = new DynamicBinder(model, widgetBind, registroView);
            binder->setObjectName(modelPropertyName);
            registroView->m_dynamicBinders.append(binder);
        }
        else
        {
            // Cuando no existe un widget correspondiente a la propiedad, se recurre a buscar si existe alguna propiedad
            // dentro del registro que corresponda a la propiedad del model
            const QMetaObject *viewMetaObject = registroView->metaObject();
            for(int i = 1; i < viewMetaObject->propertyCount(); i++)
            {
                QMetaProperty viewProperty = modelMetaObject->property(i);
                if(modelPropertyName == viewProperty.name())
                {
                    // El PropertyBinder se encarga de guardar el nombre de la propiedad para poder alterar
                    // los datos del widget
                    PropertyBinder *propertyBind = new PropertyBinder(registroView, modelPropertyName, registroView);
                    DynamicBinder *binder = new DynamicBinder(model, propertyBind, registroView);
                    binder->setObjectName(modelPropertyName);
                    registroView->m_dynamicBinders.append(binder);
                    break;
                }
            }
        }
    }
    return registroView;
}


BaseWidgetView * ViewFactory::createBindedWidget(BaseWidgetView *view, crud::ObjectBase *model)
{
    BaseWidgetView *widgetView = view;
    widgetView->setModel(model);

    const QMetaObject *modelMetaObject = model->metaObject();

    // i = 1 porque la propiedad en el index '0' es 'objectName'
    for(int i = 1; i < modelMetaObject->propertyCount(); i++)
    {
        QMetaProperty property = modelMetaObject->property(i);
        QString propertyName(property.name());

        QWidget *child = widgetView->findChild<QWidget *>(propertyName);
        // Filtra el Widget Correspondiente a la Propiedad
        if(child)
        {
            // El WidgetBinder es el encargado de hacer proveer la interfaz para alterar los
            // datos de los distintos tipos de widgets que soporta la libreria
            WidgetBinder *widgetBind = new WidgetBinder(child, widgetView);
            DynamicBinder *binder = new DynamicBinder(model, widgetBind, widgetView);
            binder->setObjectName(propertyName);
            widgetView->m_dynamicBinders.append(binder);
        }
        else
        {
            // Cuando no existe un widget correspondiente a la propiedad, se recurre a buscar si existe alguna propiedad
            // dentro del registro que corresponda a la propiedad del model
            const QMetaObject *viewMetaObject = widgetView->metaObject();
            if (viewMetaObject->indexOfProperty(propertyName.toLocal8Bit()) != -1)
            {
                // El PropertyBinder se encarga de guardar el nombre de la propiedad para poder alterar
                // los datos del widget
                PropertyBinder *propertyBind = new PropertyBinder(widgetView, propertyName, widgetView);
                DynamicBinder *binder = new DynamicBinder(model, propertyBind, widgetView);
                binder->setObjectName(propertyName);
                widgetView->m_dynamicBinders.append(binder);
            }
        }
    }
    return widgetView;
}
