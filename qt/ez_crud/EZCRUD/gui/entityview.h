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

#ifndef REGISTROVIEW_H
#define REGISTROVIEW_H

#include "basewidgetview.h"

namespace Ui {
    class EntityView;
}

class EZCRUD_EXPORT EntityView : public BaseWidgetView
{
    Q_OBJECT
public:
    explicit EntityView(QWidget *parent = 0);
    explicit EntityView(crud::State state, QWidget *parent = 0);
    ~EntityView();
    virtual void setState(crud::State state);

    QWidget *widgetView();
    void setWidgetView(QWidget * widgetView);

    void setModelValues();
    void setViewValues();

private:
    Ui::EntityView *ui;
protected:
    friend class Factory;
    //widget validation rutine
    virtual bool validate(){return true;}

    QWidget *m_widgetView;
public slots:
    void onSaveButtonClicked();
signals:
    void viewValidated();
};

#endif // REGISTROVIEW_H
