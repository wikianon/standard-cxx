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

#ifndef CRUD_USERVIEW_H
#define CRUD_USERVIEW_H

#include <gui/entityview.h>

class QAbstractItemModel;

namespace crud {

namespace Ui {
class UserView;
}

class Organization;
class AccessModeModel;
class Revision;

class EZCRUD_EXPORT UserView : public EntityView
{
    Q_OBJECT

public:
    explicit UserView(QWidget *parent = 0);
    ~UserView();
    AccessModeModel *treeModel();
    void setState(State);
    void setModel(ObjectBase *model);
    QList<Revision *> getNewRevisionList();
private:
    Ui::UserView *ui;
    QList<Organization *> m_organizationList;
    QList<Revision *> m_revisionList;
    QList<AccessModeModel *> m_accessModeModelList;
private slots:
    void selectAll();
    void selectNone();
    void expandAll();
    void collapseAll();
};

} // namespace crud
#endif // CRUD_USERVIEW_H
