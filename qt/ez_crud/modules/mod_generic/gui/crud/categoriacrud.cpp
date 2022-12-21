

#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSortFilterProxyModel>
#include <QTableView>

#include <core/session.h>

#include "categoriacrud.h"
#include "../view/categoriaview.h"

#include "../../data/dao/categoriadao.h"
#include "../../data/object/categoria.h"

namespace crud {

CategoriaCrud::CategoriaCrud(EasyCrud::CRUDFlags &crudFlags, QWidget *parent) :
    BaseCRUD(crudFlags, parent)
{
//    select id, nombre
    QSqlQueryModel *source = new QSqlQueryModel(this);
    source->setQuery(QString("select id, nombre from categoria"),
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

bool CategoriaCrud::insert(crud::ObjectBase *obj)
{
    Categoria *categoria = qobject_cast<Categoria *>(obj);
    bool bInsert = crud::CategoriaDAO::instance()->insertObject(categoria);
    return bInsert;
}

bool CategoriaCrud::save(crud::ObjectBase *obj)
{
    Categoria *categoria = qobject_cast<Categoria *>(obj);
    bool bUpdate = crud::CategoriaDAO::instance()->updateObject(categoria);
    return bUpdate;
}

crud::ObjectBase *CategoriaCrud::getRegistro(qlonglong id)
{
    if(id > 0){
        Categoria *categoria = crud::CategoriaDAO::instance()->getById(id, this);
        return categoria;
    }
    return new Categoria(this);
}

EntityView *CategoriaCrud::getWidget()
{
    return new CategoriaView(this);
}

QString CategoriaCrud::formName()
{
    return "CategoriaCrud";
}

QString CategoriaCrud::windowTitle()
{
    return tr("Categorias");
}

QSqlDatabase *CategoriaCrud::getDatabase()
{
    return crud::Session::instance()->organizationDatabase();
}


} // namespace crud
