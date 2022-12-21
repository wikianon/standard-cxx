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

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSortFilterProxyModel>
#include <QSqlError>
#include <QToolButton>
#include <QMessageBox>
#include <QCloseEvent>
#include <QIcon>

#include "basecrud.h"
#include "ui_basecrud.h"

#include "entityview.h"
#include "core/dynamic/viewfactory.h"
#include "../data/objectbase.h"
#include "core/functions.h"



BaseCRUD::BaseCRUD(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BaseCRUD)
{
    ui->setupUi(this);
    createAndConnect();    
}

BaseCRUD::BaseCRUD(crud::EasyCrud::CRUDFlags &crudFlags, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BaseCRUD)
{
    ui->setupUi(this);
    createAndConnect();
    ui->navigatorBar->setCRUDFlags(crudFlags);
    ui->navigatorBar->setState(crud::KWatching);
}

BaseCRUD::~BaseCRUD()
{
    delete ui;
}

void BaseCRUD::on_tableView_doubleClicked(const QModelIndex &index)
{
    long codigo(ui->tableView->model()->data(ui->tableView->model()->index(index.row(),0)).toInt());

    int registroIndex = indexOfRegistro(codigo);
    if(registroIndex != -1){
        ui->tabWidget->setCurrentIndex(registroIndex);
    }else{

        crud::ObjectBase *registro = getRegistro(codigo);
        EntityView *widget = getWidget();

        EntityView *view = ViewFactory::createEntityView(widget, registro);
        //connect signals and slots
        connect(this, SIGNAL(saveRequest()), view, SLOT(onSaveButtonClicked()));
        connect(view, SIGNAL(viewValidated()), SLOT(onViewValidated()));
        connect(view, SIGNAL(viewStateChanged(crud::State)), SLOT(setCurrentState(crud::State)));
        ui->tabWidget->addTab(view, "");

        view->setViewValues();

        view->setState(crud::KWatching);

        int newIndex = ui->tabWidget->count() - 1;
        ui->tabWidget->setCurrentIndex(newIndex);
    }
}


void BaseCRUD::onNewActionClicked()
{
    if( ui->navigatorBar->state() == crud::KWatching ){
        crud::ObjectBase *model = getRegistro();
        EntityView *widget = getWidget();

        EntityView *view = ViewFactory::createEntityView(widget, model);
        //connect signals and slots
        connect(this, SIGNAL(saveRequest()), view, SLOT(onSaveButtonClicked()));
        connect(view, SIGNAL(viewValidated()), SLOT(onViewValidated()));
        connect(view, SIGNAL(viewStateChanged(crud::State)), SLOT(setCurrentState(crud::State)));
        ui->tabWidget->addTab(view, "");
        view->setViewValues();
        view->setState(crud::KInserting);

        int newIndex = ui->tabWidget->count() - 1;
        ui->tabWidget->setCurrentIndex(newIndex);
    }
}


void BaseCRUD::onPrintActionClicked()
{

}


void BaseCRUD::onEditActionClicked()
{
    EntityView *currentView = qobject_cast<EntityView *>(ui->tabWidget->currentWidget());
    if(currentView)
    {
        if(currentView->state() == crud::KWatching)
        {
            //registro ya esta siendo visualizado en el catastro
            crud::ObjectBase *model = currentView->model();
            // TODO: buscar manera de mejorar este codigo
            currentView->setViewValues();
            currentView->setState(crud::KUpdating);
        }
    }
    else // tabWidgetIndex = LISTA
    {
        QModelIndex index = ui->tableView->currentIndex();
        long id(ui->tableView->model()->data(ui->tableView->model()->index(index.row(),0)).toInt());

        if(id > 0){
            int registroIndex = indexOfRegistro(id);
            if(registroIndex != -1){
                ui->tabWidget->setCurrentIndex(registroIndex);
                currentView = qobject_cast<EntityView *>(ui->tabWidget->currentWidget());
                if(currentView)
                    currentView->setState(crud::KUpdating);
            }else{

                crud::ObjectBase *model = getRegistro(id);
                EntityView *widget = getWidget();

                EntityView *view = ViewFactory::createEntityView(widget, model);
                //connect signals and slots
                connect(this, SIGNAL(saveRequest()), view, SLOT(onSaveButtonClicked()));
                connect(view, SIGNAL(viewValidated()), SLOT(onViewValidated()));
                connect(view, SIGNAL(viewStateChanged(crud::State)), SLOT(setCurrentState(crud::State)));
                //agrega la view al tabWidget
                ui->tabWidget->addTab(view, ""); /*TODO_IMPL*/ /*puede ser implementado en registro view, qobject_cast<QTabWidget *>(parentWidget())*/
                //set view's values from the model
                view->setViewValues();

                //updates view's state
                view->setState(crud::KUpdating);

                //cambia el index, agrega la view a la lista de registros
                int newIndex = ui->tabWidget->indexOf(view);
                ui->tabWidget->setCurrentIndex(newIndex);
            }
        }


    }
}


void BaseCRUD::onDeleteActionClicked()
{

}


void BaseCRUD::onCancelActionClicked()
{
    EntityView *currentView = qobject_cast<EntityView *>(ui->tabWidget->currentWidget());
    if(currentView)
    {
        if(currentView->state() != crud::KWatching)
        {
            int option = QMessageBox::question(this, tr("Advertencia"), tr("Al cancelar se perderan todos los datos.\nDesea continuar?"), QMessageBox::Yes, QMessageBox::No);
            if(option == QMessageBox::Yes)
            {
                switch(currentView->state())
                {
                case crud::KInserting:
                    ui->tabWidget->removeTab(ui->tabWidget->indexOf(currentView));
                    currentView->deleteLater();
                    setCurrentState(crud::KWatching);
                    break;
                case crud::KUpdating:
                {
                    currentView->setViewValues();
                    currentView->setState(crud::KWatching);
                    break;
                }
                default:
                    break;
                }
            }
        }
    }
}


void BaseCRUD::onViewValidated()
{
    EntityView *view = qobject_cast<EntityView *>(sender());
    crud::ObjectBase *model = view->model();
    switch(view->state())
    {
    case crud::KInserting:
    {
        //pasa los valores de la view al model
        // view -- >> model
        view->setModelValues();
        //hace el insert de la nueva entidad
        bool  bCommit = insert(model);
        if (bCommit)
        {
            view->setState(crud::KWatching);
            view->setViewValues();
            updateTableViewModel();
        }
        break;
    }
    case crud::KUpdating:
    {
        //pasa los valores de la view al model
        // view -- >> model
        view->setModelValues();
        //hace el update de la entidad
        bool bCommit = save(model);
        if (bCommit)
        {
            view->setState(crud::KWatching);
            view->setViewValues();
            updateTableViewModel();
        }
        break;
    }
    default :
        break;
    }
}

bool BaseCRUD::canClose()
{
    int countModifying = 0;

    QList<EntityView *> entityList = qFindChildren<EntityView *>(ui->tabWidget);
    foreach(EntityView *e, entityList){
        if(e->state() != crud::KWatching)
            countModifying++;
    }if(countModifying){
        int option = QMessageBox::question(this, tr("Advertencia"), tr("Al cancelar se perderan todos los datos.\nDesea continuar?"), QMessageBox::Yes, QMessageBox::No);
        if(option == QMessageBox::Yes){
            return true;
        }else{
            return false;
        }
    }else{
        return true;
    }
}


void BaseCRUD::setTableViewModel(QAbstractItemModel *model)
{
    ui->tableView->setModel(model);
}


QTableView * BaseCRUD::tableView()
{
    return ui->tableView;
}

QTabWidget * BaseCRUD::tabWidget()
{
    return ui->tabWidget;
}

NavigatorBar *BaseCRUD::actionPanel()
{
    return ui->navigatorBar;
}

QSqlDatabase *BaseCRUD::getDatabase()
{
}

void BaseCRUD::closeTab()
{
    EntityView *currentView = qobject_cast<EntityView *>(ui->tabWidget->currentWidget());
    if(currentView){
        if(currentView->state() == crud::KWatching){
            ui->tabWidget->removeTab(ui->tabWidget->indexOf(currentView));
            currentView->deleteLater();
            on_tabWidget_currentChanged(ui->tabWidget->currentWidget());
        }
    }
}


void BaseCRUD::on_tabWidget_currentChanged(QWidget *arg1)
{
    //la unica funcion de este slot es actualizar el estado el CloseButton del TabWidget
    EntityView *currentView = qobject_cast<EntityView *>(arg1);
    if(currentView){
        QWidget *widget = ui->tabWidget->cornerWidget(Qt::TopRightCorner);
        widget->setEnabled( (currentView->state() == crud::KWatching) );
        //setCurrentState(currentView->state());
    }else{
        QWidget *widget = ui->tabWidget->cornerWidget(Qt::TopRightCorner);
        widget->setEnabled( false );
    }
}


void BaseCRUD::setCurrentState(crud::State state)
{
    EntityView *currentView = qobject_cast<EntityView *>(ui->tabWidget->currentWidget());
    if(currentView){
        QWidget *widget = ui->tabWidget->cornerWidget(Qt::TopRightCorner);
        widget->setEnabled( (currentView->state() == crud::KWatching) );
        //changes the state of NavigatorBar
        if(ui->navigatorBar->state() != state)
            ui->navigatorBar->setState(state);
    }else{
        if(ui->navigatorBar->state() != state)
            ui->navigatorBar->setState(state);
        QWidget *widget = ui->tabWidget->cornerWidget(Qt::TopRightCorner);
        widget->setEnabled( false );
    }

}

int BaseCRUD::indexOfRegistro(qlonglong codigo)
{
    int registroIndex = -1;
    QList<EntityView *> viewList = qFindChildren<EntityView *>(ui->tabWidget);
    foreach(EntityView * r, viewList){
        if(r->model()->id().toLongLong() == codigo){
            registroIndex = ui->tabWidget->indexOf(r);
            break;
        }
    }
    return registroIndex;
}

void BaseCRUD::updateTableViewModel()
{
    QSortFilterProxyModel *proxyModel = qobject_cast<QSortFilterProxyModel *>(ui->tableView->model());
    if(proxyModel){
        QSqlQueryModel *queryModel = qobject_cast<QSqlQueryModel *>(proxyModel->sourceModel());
        if(queryModel){
            queryModel->setQuery(queryModel->query().lastQuery(), *getDatabase());
            tableView()->resizeColumnsToContents();
            tableView()->resizeRowsToContents();
        }
    }
}


void BaseCRUD::createAndConnect()
{
    setWindowIcon(QIcon("NO ICON"));
    QString resourcePath(":/ezcrud/images");

    connect(ui->navigatorBar, SIGNAL(saveButtonClicked()), SIGNAL(saveRequest()));
    connect(ui->navigatorBar, SIGNAL(cancelButtonClicked()), SIGNAL(cancelRequest()));

    connect(ui->navigatorBar, SIGNAL(newButtonClicked()), SLOT(onNewActionClicked()));
    connect(ui->navigatorBar, SIGNAL(editButtonClicked()), SLOT(onEditActionClicked()));
    connect(ui->navigatorBar, SIGNAL(cancelButtonClicked()), SLOT(onCancelActionClicked()));
    connect(ui->navigatorBar, SIGNAL(printButtonClicked()), SLOT(onPrintActionClicked()));
    connect(ui->navigatorBar, SIGNAL(deleteButtonClicked()), SLOT(onDeleteActionClicked()));


    QToolButton *closeButton = new QToolButton(this);
    closeButton->setEnabled(false);
    closeButton->setAutoRaise(true);
    closeButton->setToolTip(tr("Cierra la pestana actual"));
    closeButton->setIcon(QIcon(resourcePath + QLatin1String("/closetab.png")));

    ui->tabWidget->setCornerWidget(closeButton, Qt::TopRightCorner);
    connect(closeButton, SIGNAL(clicked()), this, SLOT(closeTab()));


    //filters is not visible by the moment
    setWindowTitle(windowTitle());
    ui->groupBoxFiltros->setVisible(false);
}
