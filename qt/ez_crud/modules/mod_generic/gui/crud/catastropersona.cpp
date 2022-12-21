#include <QSqlQueryModel>
#include <QSortFilterProxyModel>
#include <QTableView>

#include <forms/entityview.h>

#include "catastropersona.h"

#include "../forms/persona/personaview.h"

#include "../orm/persona/persona.h"
#include "../orm/precompiled.h"

CatastroPersona::CatastroPersona(crud::EasyCrud::CRUDFlags &crudFlags, QWidget *parent) :
    BaseCRUD(crudFlags, parent)
{
    setObjectName("CatastroPersona");

    QSqlQueryModel *source = new QSqlQueryModel(this);
    source->setQuery(QString("select persona.nidvalue,\n") +
                QString("       persona.nombre,\n") +
                QString("       persona.apellido,\n") +
                QString("       persona.ruc,\n") +
                QString("       persona.dateofinsertion,\n") +
                QString("       persona.dateoflastupdate\n") +
                QString("  from persona\n") +
                QString("order by persona.nidvalue"), qx::QxSqlDatabase::getSingleton()->getDatabase());
    source->setHeaderData(0, Qt::Horizontal, tr("Codigo"));
    source->setHeaderData(1, Qt::Horizontal, tr("Nombre"));
    source->setHeaderData(2, Qt::Horizontal, tr("Apellido"));
    source->setHeaderData(3, Qt::Horizontal, tr("RUC"));
    source->setHeaderData(4, Qt::Horizontal, tr("Fecha de insercion"));
    source->setHeaderData(5, Qt::Horizontal, tr("Fecha de actualizacion"));

    QSortFilterProxyModel * model = new QSortFilterProxyModel(this);
    model->setSourceModel(source);
    setTableViewModel(model);

    tableView()->resizeColumnsToContents();
    tableView()->resizeRowsToContents();

    if(source->query().lastError().isValid())
        qDebug() << source->query().lastError().text();

    setWindowTitle(windowTitle());
}

bool CatastroPersona::insert(OrmClass *registro)
{
     orm::persona::Persona *persona = qobject_cast< orm::persona::Persona *>(registro);

    QSqlError sqlError = qx::dao::insert(persona);
    return !sqlError.isValid();
}

bool CatastroPersona::save(OrmClass *registro)
{
     orm::persona::Persona *persona = qobject_cast< orm::persona::Persona *>(registro);
     QSqlError sqlError = qx::dao::save(persona);
    return !sqlError.isValid();
}

OrmClass * CatastroPersona::getRegistro(qlonglong codigo)
{
    if(codigo > 0)
    {
        orm::persona::Persona *persona = new orm::persona::Persona (this);
        persona->setId(codigo);
        qx::dao::fetch_by_id(persona);
        return persona;
    }
    return new orm::persona::Persona(this);
}

EntityView * CatastroPersona::getWidget()
{
    return new PersonaView(this);
}


QString CatastroPersona::formName()
{
    return QString("FORMPERSONA");
}

QString CatastroPersona::windowTitle()
{
    return QString(tr("Catastro de Personas"));
}
