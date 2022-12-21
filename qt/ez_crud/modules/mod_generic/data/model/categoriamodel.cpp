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

// ****************************************************
// * Codigo Generado por DAOBuilder                   *
// * Fecha: 26/09/2012 22:40:48                       *
// * Luis Valdes 2012 - Todos los Derechos Reservados *
// ****************************************************
#include "categoriamodel.h"
#include "../object/categoria.h"

namespace crud {

CategoriaModel::CategoriaModel(QObject *parent) :
    QAbstractTableModel(parent)
{

}

CategoriaModel::CategoriaModel(QList<Categoria *> *categoriaList, QObject *parent) :
    QAbstractTableModel(parent)
{

    m_categoriaList = categoriaList;
}

QVariant CategoriaModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();
    const Categoria &obj = *(m_categoriaList->at(index.row()));
    if((role == Qt::DisplayRole) || (role == Qt::EditRole)){
        switch(index.column()){
        case KId:
            return obj.id();
        case KNombre:
            return obj.nombre();
        default:
            qWarning("Error: CategoriaModel::data()");
        }
    }
    return QVariant();
}

bool CategoriaModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return false;
}

QVariant CategoriaModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant();
    if(orientation == Qt::Horizontal){
        switch(section)
        {
        case KId:
            return tr("Id");
        case KNombre:
            return tr("Nombre");
        default:
            qWarning("Error: CategoriaModel::headerData()");
        }
    }
    return QVariant();
}

Qt::ItemFlags CategoriaModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractTableModel::flags(index);
    return flags;
}

int CategoriaModel::rowCount(const QModelIndex &) const
{
    return m_categoriaList->count();
}

int CategoriaModel::columnCount(const QModelIndex &) const
{
    return 2;
}

bool CategoriaModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count -1);
    endInsertRows();
    return true;
}

bool CategoriaModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count -1);
    m_categoriaList->removeAt(row);
    endRemoveRows();
    return true;
}

bool CategoriaModel::addCategoria(Categoria *obj)
{
    m_categoriaList->append(obj);
    return insertRows(m_categoriaList->count() -1, 1);
}

} // namespace crud
