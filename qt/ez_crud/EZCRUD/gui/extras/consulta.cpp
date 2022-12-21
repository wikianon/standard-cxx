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

#include <QAbstractItemModel>
#include <QSortFilterProxyModel>

#include "consulta.h"
#include "ui_consulta.h"

Consulta::Consulta(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Consulta)
{
    ui->setupUi(this);
}

Consulta::Consulta(QAbstractItemModel *model, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Consulta)
{
    ui->setupUi(this);
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);
    ui->tableView->setModel(proxyModel);


    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();
}

Consulta::~Consulta()
{
    delete ui;
}

void Consulta::on_tableView_doubleClicked(const QModelIndex &index)
{
    ui->tableView->setCurrentIndex(index);
    accept();
}


qlonglong Consulta::consultar(QAbstractItemModel *model,QWidget *parent)
{
    Consulta *consulta = new Consulta(model, parent);
    if(consulta->exec() == QDialog::Accepted)
    {
        QModelIndex index = consulta->ui->tableView->model()->index(consulta->ui->tableView->currentIndex().row(), 0);
        return consulta->ui->tableView->model()->data(index).toLongLong();
    }
    else
        return -1;
}
