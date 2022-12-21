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

#ifndef BASEWIDGETVIEW_H
#define BASEWIDGETVIEW_H

#include <QWidget>

#include "../data/objectbase.h"
#include "../core/constants.h"

class DynamicBinder;

class EZCRUD_EXPORT BaseWidgetView : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(crud::State state
               READ state
               WRITE setState)
public:
    explicit BaseWidgetView(QWidget *parent = 0);
    explicit BaseWidgetView(crud::State state, QWidget *parent = 0);

    virtual void setModelValues();
    virtual void setViewValues();
    virtual void setState(crud::State state);
    virtual crud::State state(){return m_state;}
    virtual crud::ObjectBase *model();
    virtual void setModel(crud::ObjectBase *model);
protected:
    friend class ViewFactory;
    crud::State m_state;
    crud::ObjectBase *m_objectBase;
    QList<DynamicBinder *> m_dynamicBinders;

signals:
    void viewStateChanged(crud::State state);
public slots:

};

#endif // BASEWIDGETVIEW_H
