#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSortFilterProxyModel>
#include <QTableView>

#include <core/session.h>

#include "personacrud.h"
#include "../view/personaview.h"

#include "../../data/dao/personadao.h"
#include "../../data/dao/persona_categoriadao.h"
#include "../../data/object/persona.h"
#include "../../data/model/categoriacheckmodel.h"

namespace crud{

PersonaCrud::PersonaCrud(EasyCrud::CRUDFlags &crudFlags, QWidget *parent):
    BaseCRUD(crudFlags, parent)
{
    //    select id, nombre
    QSqlQueryModel *source = new QSqlQueryModel(this);
    source->setQuery(QString("select id, razonsocial from persona"),
                     *crud::Session::instance()->organizationDatabase());
    source->setHeaderData(0, Qt::Horizontal, tr("Código"));
    source->setHeaderData(1, Qt::Horizontal, tr("Nombre "));

    QSortFilterProxyModel * model = new QSortFilterProxyModel(this);
    model->setSourceModel(source);
    setTableViewModel(model);

    tableView()->resizeColumnsToContents();
    tableView()->resizeRowsToContents();

    if(source->query().lastError().isValid())
        qDebug() << source->query().lastError().text();
    setWindowTitle(windowTitle());
}

bool PersonaCrud::insert(crud::ObjectBase *obj)
{
    Persona *persona = qobject_cast<Persona *>(obj);
    bool bInsert = crud::PersonaDAO::instance()->insertObject(persona);
    if(bInsert){
        PersonaView *personaView = qobject_cast<PersonaView *>(tabWidget()->currentWidget());
        if(personaView){
            QList<PersonaCategoria *> personaCategoriaList = personaView->categoriaCheckModel()->getPersonaCategoriaList(persona);
            PersonaCategoriaDAO::instance()->insertObjectList(persona->id().toLongLong(), personaCategoriaList);
        }
    }
    return bInsert;
}

bool PersonaCrud::save(crud::ObjectBase *obj)
{
    Persona *persona = qobject_cast<Persona *>(obj);
    bool bUpdate = crud::PersonaDAO::instance()->updateObject(persona);
    if(bUpdate){
        PersonaView *personaView = qobject_cast<PersonaView *>(tabWidget()->currentWidget());
        if(personaView){
            QList<PersonaCategoria *> personaCategoriaList = personaView->categoriaCheckModel()->getPersonaCategoriaList(persona);
            PersonaCategoriaDAO::instance()->insertObjectList(persona->id().toLongLong(), personaCategoriaList);
        }
    }
    return bUpdate;
}

crud::ObjectBase *PersonaCrud::getRegistro(qlonglong id)
{
    if(id > 0){
        Persona *persona = crud::PersonaDAO::instance()->getById(id, this);
        return persona;
    }
    return new Persona(this);
}

EntityView *PersonaCrud::getWidget()
{
    return new PersonaView(this);
}

QString PersonaCrud::formName()
{
    return "PersonaCrud";
}

QString PersonaCrud::windowTitle()
{
    return tr("Personas");
}

QSqlDatabase *PersonaCrud::getDatabase()
{
    return crud::Session::instance()->organizationDatabase();
}

} // namespace crud
