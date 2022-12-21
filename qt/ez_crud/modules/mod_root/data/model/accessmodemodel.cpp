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

#include <QSize>

#include "accessmodemodel.h"

#include <data/object/module.h>
#include <data/object/widget.h>
#include <data/object/user.h>
#include <data/object/revision.h>
#include <data/object/module_revision.h>
#include <data/object/widget_revision.h>

#include <data/dao/revisiondao.h>

#include <core/session.h>

#include "accessmodeitem.h"

namespace crud {

AccessModeModel::AccessModeModel(QObject *parent) :
    QAbstractItemModel(parent),
    m_readOnly(false),
    rootItem(new AccessModeItem),
    m_revision(0),
    m_idOrganization(-1),
    m_idUser(-1)
{
    createTreeModel();
    setRevision(0);
}

AccessModeModel::AccessModeModel(Revision *revision, QObject *parent) :
    QAbstractItemModel(parent),
    m_readOnly(false),
    rootItem(new AccessModeItem),
    m_revision(revision),
    m_idOrganization(-1),
    m_idUser(-1)
{
    createTreeModel();
    setRevision(m_revision);
}

AccessModeModel::~AccessModeModel()
{
    delete rootItem;
}

Qt::ItemFlags AccessModeModel::flags(const QModelIndex &index) const
{
    //    Qt::ItemFlags flags = QAbstractItemModel::flags(index);
    Qt::ItemFlags flags (Qt::NoItemFlags);
    if(index.isValid()){
        //        flags  |= Qt::ItemFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsTristate);
        //        flags  |= Qt::ItemFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled );
        switch(index.column()){
        case KName:{
            flags  |= Qt::ItemFlags(Qt::ItemIsEnabled );
            break;
        }
        case KRead:{
            AccessModeItem *item = itemForIndex(index);
            int accessMode = item->accessMode();
            accessMode &= ~EasyCrud::Watch;
            if(m_readOnly || (!m_readOnly && accessMode))
                flags  |= Qt::ItemFlags(Qt::ItemIsUserCheckable);
            else
                flags  |= Qt::ItemFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
            break;
        }
        case KCreate:
        case KUpdate:
        case KDelete:
        {
            if(m_readOnly)
                flags  |= Qt::ItemFlags(Qt::ItemIsUserCheckable);
            else
                flags  |= Qt::ItemFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
            break;
        }
        }
    }
    return flags;
}

QVariant AccessModeModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || index.column() < 0 || index.column() >= ColumnCount)
        return QVariant();
    if(AccessModeItem *item = itemForIndex(index)){
        if(role == Qt::DisplayRole){
            switch (index.column()) {
            case KName:
                if(item->isModule())
                    return item->module()->moduleMenu();
                if(item->isWidget())
                    return item->widget()->widgetMenu();
                //                if(item->isFlag())
                //                    return ""; //boost::tuples::get<0>(item->flag());
                if(item->isGroup())
                    return item->text();
                break;
            case KRead:
            case KCreate:
            case KUpdate:
            case KDelete:
                break;
            default:
                break;
            }
        }
        if((role == Qt::CheckStateRole)
                && ((index.column() == KRead)
                    || (index.column() == KCreate)
                    || (index.column() == KUpdate)
                    || (index.column() == KDelete) )
                && (item->isWidget())){
            int tester = 0;
            int accessMode = item->accessMode();

            switch(item->widget()->widgetType().toInt()){
            case KRegistro:
            case KMovimiento:
            {
                switch( index.column() ){
                case KRead:
                    tester = EasyCrud::Watch;
                    break;
                case KCreate:
                    tester = EasyCrud::Insert;
                    tester &= ~EasyCrud::Watch;
                    break;
                case KUpdate:
                    tester = EasyCrud::Update;
                    tester &= ~EasyCrud::Watch;
                    break;
                case KDelete:
                    tester = EasyCrud::Delete;
                    tester &= ~EasyCrud::Watch;
                    break;
                }
                return  (accessMode & tester) > 0;
            }
            case KInforme:
            {
                switch( index.column() ){
                case KRead:
                    tester = EasyCrud::Watch;
                    break;
                case KCreate:
                case KUpdate:
                case KDelete:
                    return QVariant();
                    break;
                }
                return  (accessMode & tester) > 0;
            }
            case KConfiguracion:
            {
                switch( index.column() ){
                case KRead:
                    tester = EasyCrud::Watch;
                    break;
                case KCreate:
                    return QVariant();
                    break;
                case KUpdate:
                    tester = EasyCrud::Update;
                    tester &= ~EasyCrud::Watch;
                    break;
                case KDelete:
                    return QVariant();
                    break;
                }
                return  (accessMode & tester) > 0;
            }
            }
        }
    }
    return QVariant();
}

QVariant AccessModeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole){
        switch(section){
        case KName:
            return tr("Nombre");
        case KRead:
            return tr("Visualizar");
        case KCreate:
            return tr("Insertar");
        case KUpdate:
            return tr("Actualizar");
        case KDelete:
            return tr("Eliminar");
        }
    }
    return QVariant();
}

int AccessModeModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid() && parent.column() != 0)
        return 0;
    AccessModeItem *parentItem = itemForIndex(parent);
    return parentItem ? parentItem->childCount() : 0;
}

int AccessModeModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() && parent.column() != 0 ? 0  : ColumnCount;
}

QModelIndex AccessModeModel::index(int row, int column, const QModelIndex &parent) const
{
    if(row < 0 || column < 0 || column >= ColumnCount || (parent.isValid() && parent.column() != 0))
        return QModelIndex();
    AccessModeItem *parentItem = itemForIndex(parent);
    Q_ASSERT(parentItem);
    if(AccessModeItem *item = parentItem->childAt(row))
        return createIndex(row, column, item);
    return QModelIndex();
}

QModelIndex AccessModeModel::parent(const QModelIndex &index) const
{
    if(!index.isValid())
        return QModelIndex();
    if(AccessModeItem *childItem = itemForIndex(index)){
        if(AccessModeItem *parentItem = childItem->parent()){
            if(parentItem == rootItem)
                return QModelIndex();
            if(AccessModeItem *grandParentItem = parentItem->parent()){
                int row = grandParentItem->rowOfChild(parentItem);
                return createIndex(row, 0, parentItem);
            }
        }
    }
    return QModelIndex();
}

bool AccessModeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(value)
    if((role == Qt::CheckStateRole) && ((index.column() == KRead)
                                        || (index.column() == KCreate)
                                        || (index.column() == KUpdate)
                                        || (index.column() == KDelete) )){
        if(AccessModeItem *item = itemForIndex(index)){
            if(item->isWidget()){
                int accessMode = item->accessMode();
                //                switch(item->widget()->widgetType().toInt()){
                switch( index.column() ){
                case KRead:
                {
                    if(!(accessMode & EasyCrud::Watch))
                        accessMode |= EasyCrud::Watch;
                    else
                        accessMode &= ~EasyCrud::Watch;
                    break;
                }
                case KCreate:
                {
                    int bitPattern = EasyCrud::Insert;
                    bitPattern &= ~EasyCrud::Watch;
                    if(!(accessMode & bitPattern))
                        accessMode |= EasyCrud::Insert;
                    else
                        accessMode &= ~bitPattern;
                    break;
                }
                case KUpdate:
                {
                    int bitPattern = EasyCrud::Update;
                    bitPattern &= ~EasyCrud::Watch;
                    if(!(accessMode & bitPattern))
                        accessMode |= EasyCrud::Update;
                    else
                        accessMode &= ~bitPattern;
                    break;
                }
                case KDelete:
                {
                    int bitPattern = EasyCrud::Delete;
                    bitPattern &= ~EasyCrud::Watch;
                    if(!(accessMode & bitPattern))
                        accessMode |= EasyCrud::Delete;
                    else
                        accessMode &= ~bitPattern;
                    break;
                }
                }
                item->setAccessMode(accessMode);
                emit dataChanged(index, index);
                return true;
            }
            return false;
        }
    }
    return false;
}

bool AccessModeModel::isChecked(const QModelIndex &index) const
{
    if (!index.isValid())
        return false;
    return data(index, Qt::CheckStateRole).toInt() == Qt::Checked || data(index, Qt::CheckStateRole).toInt() == Qt::PartiallyChecked;
}

AccessModeItem *AccessModeModel::itemForIndex(const QModelIndex &index) const
{
    if(index.isValid()){
        if(AccessModeItem *item = static_cast<AccessModeItem *>(index.internalPointer()))
            return item;
    }
    return rootItem;
}

crud::Revision *AccessModeModel::getNewRevision()
{
    crud::Revision *revision = new crud::Revision(this);
    revision->setIdOrganization(m_idOrganization);
    revision->setIdUser(m_idUser);
    foreach(AccessModeItem *moduleItem, rootItem->children()) {// Modules
        crud::ModuleRevision *revisionModule = 0;
        foreach(AccessModeItem *groupItem, moduleItem->children()){ // Groups
            foreach(AccessModeItem *widgetItem, groupItem->children()){ // Widgets
                int accessMode = widgetItem->accessMode();
                if(accessMode){
                    if(!revisionModule){
                        revisionModule = new crud::ModuleRevision(revision);
                        revisionModule->setIdModule(moduleItem->module()->id());
                        revision->moduleList().append(revisionModule);
                    }
                    crud::WidgetRevision *revisionWidget = new crud::WidgetRevision(revisionModule);
                    revisionWidget->setIdWidget(widgetItem->widget()->id());
                    revisionWidget->setAccessMode(accessMode);
                    revision->widgetList().append(revisionWidget);
                }
            }
        }
    }
    return revision;
}

void AccessModeModel::setRevision(Revision *revision)
{
    m_revision = revision;
    if(m_revision){
        foreach(crud::ModuleRevision *rm, m_revision->moduleList()){
            foreach(AccessModeItem *moduleItem, rootItem->children()){ // Modules
                if(moduleItem->module()->id().toLongLong() == rm->idModule().toLongLong()){
                    foreach(crud::WidgetRevision *rw, m_revision->widgetList()){
                        foreach(AccessModeItem *groupItem, moduleItem->children()){ // Groups
                            foreach(AccessModeItem *widgetItem, groupItem->children()){ // Widgets
                                if(widgetItem->widget()->id().toLongLong() == rw->idWidget().toLongLong()){
                                    int accessMode = rw->accessMode().toInt();
                                    widgetItem->setAccessMode(accessMode);
                                }
                            }
                        }
                    }
                }
            }
        }
    }else{
        selectNone();
    }
}

QVariant AccessModeModel::readOnly()
{
    return m_readOnly;
}

void AccessModeModel::setReadOnly(QVariant readOnly)
{
    m_readOnly = readOnly.toBool();
}

qlonglong AccessModeModel::idOrganization() const
{
    return m_idOrganization;
}

void AccessModeModel::setIdOrganization(qlonglong idOrganization)
{
    m_idOrganization = idOrganization;
}

qlonglong AccessModeModel::idUser() const
{
    return m_idUser;
}

void AccessModeModel::setIdUser(qlonglong idUser)
{
    m_idUser = idUser;
}

void AccessModeModel::createTreeModel()
{
    foreach(crud::Module *module, Session::instance()->moduleList()) {
        AccessModeItem *moduleItem = new AccessModeItem( rootItem);
        moduleItem->setModule(module);
        AccessModeItem *registroItem = 0;
        AccessModeItem *movimientoItem = 0;
        AccessModeItem *informeItem = 0;
        AccessModeItem *configuracionItem = 0;
        foreach(crud::Widget *widget, module->widgetList()) {
            if(widget->idModule() == module->id()){
                AccessModeItem *groupItem = 0;
                switch(widget->widgetType().toInt()){
                case KRegistro:
                    if(!registroItem){
                        registroItem = new AccessModeItem(moduleItem);
                        registroItem->setGroupId(KRegistro);
                        registroItem->setText(tr("Registros"));
                    }
                    groupItem = registroItem;
                    break;
                case KMovimiento:
                    if(!movimientoItem){
                        movimientoItem = new AccessModeItem(moduleItem);
                        movimientoItem->setGroupId(KMovimiento);
                        movimientoItem->setText(tr("Movimientos"));
                    }
                    groupItem = movimientoItem;
                    break;
                case KInforme:
                    if(!informeItem){
                        informeItem = new AccessModeItem(moduleItem);
                        informeItem->setGroupId(KInforme);
                        informeItem->setText(tr("Informes"));
                    }
                    groupItem = informeItem;
                    break;
                case KConfiguracion:
                    if(!configuracionItem){
                        configuracionItem = new AccessModeItem(moduleItem);
                        configuracionItem->setGroupId(KConfiguracion);
                        configuracionItem->setText(tr("Configuraciones"));
                    }
                    groupItem = configuracionItem;
                    break;
                }
                AccessModeItem *widgetItem = new AccessModeItem(groupItem);
                widgetItem->setWidget(widget);
            }
        }
    }
}

void AccessModeModel::selectAll()
{
    foreach(AccessModeItem *moduleItem, rootItem->children()){
        foreach(AccessModeItem *groupItem, moduleItem->children()){
            foreach(AccessModeItem *widgetItem, groupItem->children()){
                switch(widgetItem->widget()->widgetType().toInt()){
                case KRegistro:
                case KMovimiento:
                    widgetItem->setAccessMode(15);
                    break;
                case KInforme:
                    widgetItem->setAccessMode(1);
                    break;
                case KConfiguracion:
                    widgetItem->setAccessMode(5);
                    break;
                }
            }
        }
    }
    emit dataChanged(QModelIndex(), QModelIndex());
}

void AccessModeModel::selectNone()
{
    foreach(AccessModeItem *moduleItem, rootItem->children()){
        foreach(AccessModeItem *groupItem, moduleItem->children()){
            foreach(AccessModeItem *widgetItem, groupItem->children()){
                widgetItem->setAccessMode(0);
            }
        }
    }
    emit dataChanged(QModelIndex(), QModelIndex());
}



} // namespace crud
