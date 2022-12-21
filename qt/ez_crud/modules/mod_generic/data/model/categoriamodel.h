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
#ifndef CATEGORIAMODEL_H
#define CATEGORIAMODEL_H
#include <QAbstractTableModel>

#include <global/ezcrud_global.h>

namespace crud {

// Object Forward Declarations
class Categoria;

class EZCRUD_EXPORT CategoriaModel : public QAbstractTableModel
{
	Q_OBJECT
public:
// Enum Declaration
    enum KCategoria{
		KId = 0,
		KNombre = 1
	};

// Constructors
	CategoriaModel(QObject *parent = 0);
	CategoriaModel(QList<Categoria *> *categoriaList, QObject *parent = 0);

// Model Methods
	QVariant data(const QModelIndex &index, int role) const;
	bool setData(const QModelIndex &index, const QVariant &value, int role);
	QVariant headerData(int section, Qt::Orientation orientation, int role) const;
	Qt::ItemFlags flags(const QModelIndex &index) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
	bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

// Add Method
	bool addCategoria(Categoria *);

// Private Member
private:
	QList<Categoria *>  *m_categoriaList;

};

} // namespace crud

#endif // CATEGORIAMODEL_H_H
