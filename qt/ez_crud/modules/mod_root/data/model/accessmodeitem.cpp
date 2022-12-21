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

#include "accessmodeitem.h"

namespace crud {

AccessModeItem::AccessModeItem(AccessModeItem *parent) :
    m_parent(parent),
    m_children(),
    m_module(0),
    m_widget(0),
    m_accessMode(0),
    m_groupId(0),
    m_text(""),
    m_isModule(false),
    m_isWidget(false),
    m_isGroup(false)
{
    if(m_parent)
        m_parent->m_children.append(this);
}

AccessModeItem::~AccessModeItem()
{
    qDeleteAll(m_children);
}

crud::Module *AccessModeItem::module()
{
    return m_module;
}

void AccessModeItem::setModule(crud::Module *module)
{
    m_isModule = true;
    m_module = module;
}

crud::Widget *AccessModeItem::widget()
{

    return m_widget;
}

void AccessModeItem::setWidget(crud::Widget *widget)
{
    m_isWidget = true;
    m_widget = widget;
}

int AccessModeItem::groupId() const
{
    return m_groupId;
}

void AccessModeItem::setGroupId(int id)
{
    m_isGroup = true;
    m_groupId = id;
}

QString AccessModeItem::text() const
{
    return m_text;
}

void AccessModeItem::setText(const QString &labelText)
{
    m_isGroup = true;
    m_text = labelText;
}

int AccessModeItem::accessMode()
{
    return m_accessMode;
}

void AccessModeItem::setAccessMode(int accessMode)
{
    m_accessMode = accessMode;
}

bool AccessModeItem::isModule()
{
    return m_isModule;
}

bool AccessModeItem::isWidget()
{
    return m_isWidget;
}

bool AccessModeItem::isGroup()
{
    return m_isGroup;
}

int AccessModeItem::row() const
{
    if (m_parent)
         return m_parent->m_children.indexOf(const_cast<AccessModeItem*>(this));
    return 0;
}

int AccessModeItem::rowOfChild(AccessModeItem *child) const
{
    return m_children.indexOf(child);
}

int AccessModeItem::childCount()
{
    return m_children.count();
}

AccessModeItem *AccessModeItem::parent()
{
    return m_parent;
}

AccessModeItem *AccessModeItem::childAt(int index)
{
    return m_children.value(index);
}

QList<AccessModeItem *> &AccessModeItem::children()
{
    return m_children;
}

} // namespace crud
