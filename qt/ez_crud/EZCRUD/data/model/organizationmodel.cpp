// ****************************************************
// * Codigo Generado por DAOBuilder                   *
// * Fecha: 23/10/2012 07:48:24                       *
// * Luis Valdes 2012 - Todos los Derechos Reservados *
// ****************************************************
#include "organizationmodel.h"
#include "../object/organization.h"

namespace crud {

OrganizationModel::OrganizationModel(QObject *parent) :
    QAbstractTableModel(parent)
{

}

OrganizationModel::OrganizationModel(QList<Organization *> *organizationList, QObject *parent) :
    QAbstractTableModel(parent)
{

    m_organizationList = organizationList;
}

QVariant OrganizationModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();
    const Organization &obj = *(m_organizationList->at(index.row()));
    if((role == Qt::DisplayRole) || (role == Qt::EditRole)){
        switch(index.column()){
        case KId:
            return obj.id();
        case KName:
            return obj.name();
        case KHostaddress:
            return obj.hostAddress();
        case KHostdatabase:
            return obj.hostDatabase();
        case KHostuser:
            return obj.hostUser();
        case KHostpassword:
            return obj.hostPassword();
        default:
            qWarning("Error: OrganizationModel::data()");
        }
    }
    return QVariant();
}

bool OrganizationModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return false;
}

QVariant OrganizationModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant();
    if(orientation == Qt::Horizontal){
        switch(section){
        case KId:
            return tr("Id");
        case KName:
            return tr("Name");
        case KHostaddress:
            return tr("Hostaddress");
        case KHostdatabase:
            return tr("Hostdatabase");
        case KHostuser:
            return tr("Hostuser");
        case KHostpassword:
            return tr("Hostpassword");
        default:
            qWarning("Error: OrganizationModel::headerData()");
        }
    }
    return QVariant();
}

Qt::ItemFlags OrganizationModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractTableModel::flags(index);
    return flags;
}

int OrganizationModel::rowCount(const QModelIndex &) const
{
    return m_organizationList->count();
}

int OrganizationModel::columnCount(const QModelIndex &) const
{
    return 6;
}

bool OrganizationModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count -1);
    endInsertRows();
    return true;
}

bool OrganizationModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count -1);
    m_organizationList->removeAt(row);
    endRemoveRows();
    return true;
}

bool OrganizationModel::addOrganization(Organization *obj)
{
    m_organizationList->append(obj);
    return insertRows(m_organizationList->count() -1, 1);
}

} // namespace crud
