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

#ifndef CRUD_SYSTE_ACCESSMODEITEM_H
#define CRUD_SYSTE_ACCESSMODEITEM_H

#include <core/crudnamespace.h>
#include <data/object/module.h>
#include <data/object/widget.h>

namespace crud {

class EZCRUD_EXPORT AccessModeItem
{
public:
    AccessModeItem(AccessModeItem *parent =0);
    virtual ~AccessModeItem();

    crud::Module * module();
    void setModule(crud::Module *);

    crud::Widget * widget();
    void setWidget(crud::Widget *);
    int accessMode();
    void setAccessMode(int accessMode);

    int groupId() const;
    void setGroupId(int id);
    QString text() const;
    void setText(const QString &text);

    bool isModule();
    bool isWidget();
    bool isGroup();

    int row() const;
    int rowOfChild(AccessModeItem *) const;

    int childCount();

    AccessModeItem * parent();
    AccessModeItem * childAt(int index);
    QList<AccessModeItem*> & children();
private:
    AccessModeItem *m_parent;
    QList<AccessModeItem*> m_children;

    crud::Module *m_module;

    crud::Widget *m_widget;
    int m_accessMode;

    int m_groupId;
    QString m_text;

    bool m_isModule;
    bool m_isWidget;
    bool m_isGroup;
};

} // namespace crud

#endif // CRUD_SYSTE_ACCESSMODEITEM_H
