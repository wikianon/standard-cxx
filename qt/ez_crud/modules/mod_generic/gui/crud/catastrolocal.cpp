#include <QSqlQueryModel>
#include <QSortFilterProxyModel>
#include <QTableView>

#include "catastrolocal.h"
#include "registrolocal.h"
#include "orm/local/local.h"

#include "orm/precompiled.h"

CatastroLocal::CatastroLocal(QWidget *parent) :
    CatastroBase(parent)
{
    QSqlQueryModel *source = new QSqlQueryModel(this);
    source->setQuery(QString("select local.nidvalue,\n") +
                    QString("       local.descripcion,\n") +
                    QString("       local.dateofinsertion,\n") +
                    QString("       local.dateoflastupdate\n") +
                    QString("  from local\n") +
                    QString("order by local.nidvalue"), qx::QxSqlDatabase::getSingleton()->getDatabase());
    source->setHeaderData(0, Qt::Horizontal, tr("Codigo"));
    source->setHeaderData(1, Qt::Horizontal, tr("Descripcion"));
    source->setHeaderData(2, Qt::Horizontal, tr("Fecha de insercion"));
    source->setHeaderData(3, Qt::Horizontal, tr("Fecha de actualizacion"));
    QSortFilterProxyModel * model = new QSortFilterProxyModel(this);
    model->setSourceModel(source);
    setTableViewModel(model);
    tableView()->resizeColumnsToContents();
    tableView()->resizeRowsToContents();

    if(source->query().lastError().isValid())
        qDebug() << source->query().lastError().text();

    setWindowTitle(windowTitle());
}

QSqlError CatastroLocal::insert(OrmClass *registro)
{
    Local *categoria = qobject_cast<Local *>(registro);
    return qx::dao::insert(categoria);
}

QSqlError CatastroLocal::save(OrmClass *registro)
{
    Local *categoria = qobject_cast<Local *>(registro);
//    return qx::dao::save(categoria);
    return qx::dao::save(categoria);
}

OrmClass * CatastroLocal::getRegistro(qlonglong codigo)
{
    if(codigo > 0)
    {
        Local *local = new Local(this);
        local->setIdValue(codigo);
        qx::dao::fetch_by_id(local);
        return local;
    }
    return new Local(this);
}

RegistroView * CatastroLocal::getWidget()
{
    return new RegistroLocal(this);
}


QString CatastroLocal::formName()
{
    return QString("FORMCATEGORIA");
}

QString CatastroLocal::windowTitle()
{
    return QString(tr("Catastro Locales"));
}
