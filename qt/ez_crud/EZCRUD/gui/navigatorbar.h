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

#ifndef NAVIGATORBAR_H
#define NAVIGATORBAR_H

#include <QWidget>

#include "global/ezcrud_global.h"
#include "core/crudnamespace.h"

class QAction;

class EZCRUD_EXPORT NavigatorBar : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(crud::State state
               READ state
               WRITE setState)
public:
    explicit NavigatorBar(QWidget *parent = 0);
    void setCRUDFlags(crud::EasyCrud::CRUDFlags type);
    crud::EasyCrud::CRUDFlags crudFlags() const;
    ~NavigatorBar();
    crud::State state();
private:
    QAction *m_newAction;
    QAction *m_editAction;
    QAction *m_deleteAction;
    QAction *m_saveAction;
    QAction *m_cancelAction;
    QAction *m_printAction;
    crud::State m_state;
    crud::EasyCrud::CRUDFlags m_crudFlags;
    void createAndConnect();
signals:
    void newButtonClicked();
    void editButtonClicked();
    void deleteButtonClicked();
    void saveButtonClicked();
    void cancelButtonClicked();
    void printButtonClicked();

public slots:
    void setState(crud::State state);
};

#endif // NAVIGATORBAR_H
