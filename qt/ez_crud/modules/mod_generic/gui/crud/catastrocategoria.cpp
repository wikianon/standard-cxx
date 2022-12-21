#include <QSqlQueryModel>
#include <QSortFilterProxyModel>
#include <QTableView>

#include "catastrocategoria.h"
#include "forms/persona/categoriaview.h"

#include "../orm/persona/categoria.h"
#include "../orm/precompiled.h"

CatastroCategoria::CatastroCategoria(crud::EasyCrud::CRUDFlags &crudFlags, QWidget *parent) :
    BaseCRUD(crudFlags, parent)
{
    setObjectName("CatastroCategoria");

    QSqlQueryModel *source = new QSqlQueryModel(this);
    source->setQuery(QString("select categoria.nidvalue,\n") +
                    QString("       categoria.descripcion,\n") +
                    QString("       categoria.dateofinsertion,\n") +
                    QString("       categoria.dateoflastupdate\n") +
                    QString("  from categoria\n") +
                    QString("order by categoria.nidvalue"), qx::QxSqlDatabase::getSingleton()->getDatabase());
    source->setHeaderData(0, Qt::Horizontal, tr("Codigo"));
    source->setHeaderData(1, Qt::Horizontal, tr("Descripcion"));
    source->setHeaderData(2, Qt::Horizontal, tr("Fecha de insercion"));
    source->setHeaderData(3, Qt::Horizontal, tr("Fecha de actualizacion"));

    QSortFilterProxyModel * model = new QSortFilterProxyModel(this);
    model->setSourceModel(source);

    setTableViewModel(model);
    tableView()->resizeColumnsToContents();
    tableView()->resizeRowsToContents();

    setWindowTitle(windowTitle());
}

bool CatastroCategoria::insert(OrmClass *registro)
{
    orm::persona::Categoria *categoria = qobject_cast<orm::persona::Categoria *>(registro);
    // TODO: Utilizar try {} catch(SQLConnectionException &ce) {}
    QSqlError sqlError = qx::dao::insert(categoria);
    return !sqlError.isValid();
}

bool CatastroCategoria::save(OrmClass *registro)
{
    orm::persona::Categoria *categoria = qobject_cast<orm::persona::Categoria *>(registro);
    // TODO: Utilizar try {} catch(SQLConnectionException &ce) {}
    QSqlError sqlError = qx::dao::save(categoria);
   return !sqlError.isValid();
}

OrmClass * CatastroCategoria::getRegistro(qlonglong codigo)
{
    if(codigo > 0)
    {
        orm::persona::Categoria *categoria = new orm::persona::Categoria(this);
        categoria->setId(codigo);
        qx::dao::fetch_by_id(categoria);
        return categoria;
    }
    return new orm::persona::Categoria(this);
}

EntityView * CatastroCategoria::getWidget()
{
    return new CategoriaView(this);
}


QString CatastroCategoria::formName()
{
    return QString("FORMCATEGORIA");
}

QString CatastroCategoria::windowTitle()
{
    return QString(tr("Catastro de Categorias"));
}
