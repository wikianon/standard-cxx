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

#ifndef CRUD_SYSTEM_ACCESSMODEMODEL_H
#define CRUD_SYSTEM_ACCESSMODEMODEL_H

#include <QAbstractItemModel>

#include <global/ezcrud_global.h>

namespace crud {

class AccessModeItem;
class Revision;

const int ColumnCount = 5;

class  EZCRUD_EXPORT AccessModeModel : public QAbstractItemModel
{
    Q_OBJECT
    Q_PROPERTY(QVariant readOnly
               READ readOnly
               WRITE setReadOnly)
public:
//    enum AccessModeColumns {ModuleColumn, WidgetColumn, AccessModeColumn};
    enum AccessModeColumns {KName=0, KRead=1, KCreate=2, KUpdate=3, KDelete=4};

    explicit AccessModeModel(QObject *parent = 0);
    explicit AccessModeModel(Revision *revision, QObject *parent = 0);
    virtual ~AccessModeModel();

    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index,int role=Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role=Qt::DisplayRole) const;
    QModelIndex index(int row, int column, const QModelIndex &parent=QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent=QModelIndex()) const;
    int columnCount(const QModelIndex &parent=QModelIndex()) const;
    bool setData(const QModelIndex &index, const QVariant &value,  int role=Qt::EditRole);
    bool isChecked(const QModelIndex &index) const;

    AccessModeItem * itemForIndex(const QModelIndex &) const;

    crud::Revision * getNewRevision();

    void setRevision(Revision *revision);

    QVariant readOnly();
    void setReadOnly(QVariant);

    qlonglong idOrganization() const;
    void setIdOrganization(qlonglong idOrganization);
    qlonglong idUser() const;
    void setIdUser(qlonglong idUser);
private:
    bool m_readOnly;
    AccessModeItem *rootItem;
    crud::Revision *m_revision;
    qlonglong m_idOrganization;
    qlonglong m_idUser;
    void createTreeModel();
public slots:
    void selectAll();
    void selectNone();
};

} // namespace crud

#endif // CRUD_SYSTEM_ACCESSMODEMODEL_H
