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
#include <QTreeView>

#include <data/object/user.h>
#include <data/object/revision.h>
#include <data/object/organization.h>

#include <data/dao/userdao.h>
#include <data/dao/revisiondao.h>
#include <data/dao/organizationdao.h>

#include <data/model/organizationmodel.h>

#include <gui/caseutil.h>

#include "userview.h"
#include "ui_userview.h"

#include "../../data/model/accessmodemodel.h"

namespace crud {

UserView::UserView(QWidget *parent) :
    EntityView(parent),
    ui(new Ui::UserView),
    m_organizationList(),
    m_revisionList()
  //    m_accessModeModelList()
{
    ui->setupUi(widgetView());
    m_organizationList = OrganizationDAO::instance()->getAll(this);
    connect(ui->bSelectAll, SIGNAL(clicked()), SLOT(selectAll()));
    connect(ui->bSelectNone, SIGNAL(clicked()), SLOT(selectNone()));
    connect(ui->bExpandAll, SIGNAL(clicked()), SLOT(expandAll()));
    connect(ui->bCollapseAll, SIGNAL(clicked()), SLOT(collapseAll()));

    CaseUtil::instance()->addUpperCaseEdit(ui->loginName);
}

UserView::~UserView()
{
    delete ui;
    //    qDeleteAll(m_accessModeModelList);
}

AccessModeModel *UserView::treeModel()
{
    QTreeView *treeView = qobject_cast<QTreeView *>(ui->tabOrgPrivileges->currentWidget());
    AccessModeModel *model = qobject_cast<AccessModeModel *>(treeView->model());
    if(model)
        return model;
    else
        return 0;
}

void UserView::setState(State state)
{
    switch (state) {
    case KWatching:
        foreach(AccessModeModel *model, m_accessModeModelList){
            model->setReadOnly(true);
        }
        ui->bSelectAll->setEnabled(false);
        ui->bSelectNone->setEnabled(false);
        break;
    case KInserting:
    case KUpdating:
        foreach(AccessModeModel *model, m_accessModeModelList){
            model->setReadOnly(false);
        }
        ui->bSelectAll->setEnabled(true);
        ui->bSelectNone->setEnabled(true);
        break;
    default:
        Q_ASSERT(false);
        break;
    }
    EntityView::setState(state);
    ui->loginName->setReadOnly(state == KUpdating);
}

void UserView::setModel(ObjectBase *model)
{
    BaseWidgetView::setModel(model);

    User *user = qobject_cast<User *>(m_objectBase);
    m_revisionList = RevisionDAO::instance()->getLatestRevision(user->id().toLongLong());

    // Create TreeView for every organization of the session
    foreach(Organization *org, m_organizationList){
        QTreeView *treeView = new QTreeView(this);
        treeView->setObjectName("treeView_" + org->name().toString().trimmed());
        // Create AccessModeModel for treeView
        AccessModeModel *accessModel = new AccessModeModel(this);
        accessModel->setObjectName("model_" + org->name().toString().trimmed());
        accessModel->setIdOrganization(org->id().toLongLong());
        accessModel->setIdUser(user->id().toLongLong());
        m_accessModeModelList.append(accessModel);
        // Set the revision for this accessModeModel
        foreach(Revision *rev, m_revisionList){
            if(rev->idOrganization().toLongLong() == org->id().toLongLong()){
                accessModel->setRevision(rev);
                break;
            }
        }
        treeView->setModel(accessModel);
        treeView->header()->resizeSection(0, 250);
        ui->tabOrgPrivileges->addTab(treeView, org->name().toString());
    }
}

QList<Revision *> UserView::getNewRevisionList()
{
    QList<Revision *> list;
    foreach(AccessModeModel *model, m_accessModeModelList){
        list.append(model->getNewRevision());
    }
    return list;
}

void UserView::selectAll()
{
    QTreeView *treeView = qobject_cast<QTreeView *>(ui->tabOrgPrivileges->currentWidget());
    if(treeView){
        AccessModeModel *model = qobject_cast<AccessModeModel *>(treeView->model());
        model->selectAll();
    }
}

void UserView::selectNone()
{
    QTreeView *treeView = qobject_cast<QTreeView *>(ui->tabOrgPrivileges->currentWidget());
    if(treeView){
        AccessModeModel *model = qobject_cast<AccessModeModel *>(treeView->model());
        model->selectNone();
    }
}

void UserView::expandAll()
{
    QTreeView *treeView = qobject_cast<QTreeView *>(ui->tabOrgPrivileges->currentWidget());
    if(treeView){
        treeView->expandAll();
    }
}

void UserView::collapseAll()
{
    QTreeView *treeView = qobject_cast<QTreeView *>(ui->tabOrgPrivileges->currentWidget());
    if(treeView){
        treeView->collapseAll();
    }
}

} // namespace crud
