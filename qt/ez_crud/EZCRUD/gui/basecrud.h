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

#ifndef BASECRUD_H
#define BASECRUD_H

#include <QObject>
#include <QWidget>

#include "../data/objectbase.h"
#include "../core/crudnamespace.h"

namespace Ui {
    class BaseCRUD;
}

class QCloseEvent;
class QSqlDatabase;
class QSqlError;
class QModelIndex;
class QAbstractItemModel;
class QTableView;
class QTabWidget;

class EntityView;
class NavigatorBar;

class EZCRUD_EXPORT BaseCRUD : public QWidget
{
    Q_OBJECT

public:
    explicit BaseCRUD(QWidget *parent = 0);
    explicit BaseCRUD(crud::EasyCrud::CRUDFlags &crudFlags, QWidget *parent = 0);
    ~BaseCRUD();

    bool canClose();

protected:
    Ui::BaseCRUD *ui;
    void setTableViewModel(QAbstractItemModel *model);
    QTableView *tableView();
    QTabWidget *tabWidget();
    NavigatorBar *actionPanel();

    virtual bool insert(crud::ObjectBase *registro) = 0;
    virtual bool save(crud::ObjectBase *registro) = 0;

    virtual crud::ObjectBase * getRegistro(qlonglong codigo = 0) = 0;
    virtual EntityView * getWidget() = 0;
    virtual QString formName() = 0;
    virtual QString windowTitle() { return QString("BaseCRUD");}

    virtual QSqlDatabase * getDatabase();



protected slots:
    virtual void on_tableView_doubleClicked(const QModelIndex &index);

    virtual void onNewActionClicked();
    virtual void onEditActionClicked();
    virtual void onDeleteActionClicked();
    virtual void onCancelActionClicked();
    virtual void onPrintActionClicked();

    void onViewValidated();


    void closeTab();

    void on_tabWidget_currentChanged(QWidget *arg1);

    void setCurrentState(crud::State state);

signals:
    void saveRequest();
    void cancelRequest();
    void currentStateChanged(crud::State state); // alterar o crear header constants.h
    void closingWidget();

protected:
    int indexOfRegistro(qlonglong codigo);
    void updateTableViewModel();
    void createAndConnect();
};

#endif // BASECRUD_H
