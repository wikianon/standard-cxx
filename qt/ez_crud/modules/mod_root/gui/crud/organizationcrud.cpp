#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSortFilterProxyModel>
#include <QTableView>

#include <core/session.h>
#include <core/encryption.h>
#include <data/object/organization.h>
#include <data/dao/organizationdao.h>

#include "organizationcrud.h"
#include "../view/organizationview.h"

namespace crud {

OrganizationCrud::OrganizationCrud(EasyCrud::CRUDFlags &crudFlags, QWidget *parent) :
    BaseCRUD(crudFlags, parent)
{
    //    select id, name, hostaddress, hostdatabase from organization
    QSqlQueryModel *source = new QSqlQueryModel(this);
    source->setQuery(QString("select id, name, hostdatabase from organization"),
                     *crud::Session::instance()->systemDatabase());
    source->setHeaderData(0, Qt::Horizontal, tr("Código"));
    source->setHeaderData(1, Qt::Horizontal, tr("Nombre de la Organizacion"));
    source->setHeaderData(2, Qt::Horizontal, tr("Banco de Datos"));

    QSortFilterProxyModel * model = new QSortFilterProxyModel(this);
    model->setSourceModel(source);
    setTableViewModel(model);

    tableView()->resizeColumnsToContents();
    tableView()->resizeRowsToContents();

    if(source->query().lastError().isValid())
        qDebug() << source->query().lastError().text();
    setWindowTitle(windowTitle());
}

bool OrganizationCrud::insert(ObjectBase *registro)
{
    crud::Organization *organization = qobject_cast<crud::Organization *>(registro);
    if(Session::instance()->systemDatabase()->driverName() == "QSQLITE"){
        organization->setHostDatabase(organization->name().toString().trimmed().toLower());
        organization->setHostAddress("localhost");
        organization->setHostUser("sqliteuser");
        organization->setHostPassword("sqlitepassword");
    }
    bool bInsert = crud::OrganizationDAO::instance()->insertObject(organization);
    return bInsert;
}

bool OrganizationCrud::save(ObjectBase *registro)
{
    crud::Organization *organization = qobject_cast<crud::Organization *>(registro);
    if(Session::instance()->systemDatabase()->driverName() == "QSQLITE"){
        organization->setHostDatabase(organization->name().toString().replace(QString(" "), QString("")).trimmed().toLower());
        organization->setHostAddress("localhost");
        organization->setHostUser("sqliteuser");
        organization->setHostPassword("sqlitepassword");
    }
    bool bUpdate = crud::OrganizationDAO::instance()->updateObject(organization);
    return bUpdate;
}

ObjectBase *OrganizationCrud::getRegistro(qlonglong id)
{
    if(id > 0){
        crud::Organization *organization = crud::OrganizationDAO::instance()->getById(id);
        organization->setParent(this);
        organization->setHostAddress(Encryption::decrypt(organization->hostAddress().toString()));
        organization->setHostPassword(Encryption::decrypt(organization->hostPassword().toString()));
        return organization;
    }
    return new crud::Organization(this);
}

EntityView *OrganizationCrud::getWidget()
{
    return new OrganizationView(this);
}

QString OrganizationCrud::formName()
{
    return "OrganizationCrud";
}

QString OrganizationCrud::windowTitle()
{
    return tr("Organización");
}

QSqlDatabase *OrganizationCrud::getDatabase()
{
    return crud::Session::instance()->systemDatabase();
}

} // namespace crud
