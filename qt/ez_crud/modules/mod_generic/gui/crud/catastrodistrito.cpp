#include <QSqlQueryModel>
#include <QSortFilterProxyModel>
#include <QTableView>

#include "catastrodistrito.h"
#include "registrodistrito.h"
#include "orm/distrito/distrito.h"

#include "orm/precompiled.h"

CatastroDistrito::CatastroDistrito(QWidget *parent) :
    CatastroBase(parent)
{
    QSqlQueryModel *source = new QSqlQueryModel(this);
    source->setQuery(QString("select distrito.nidvalue,\n") +
                    QString("       distrito.descripcion,\n") +
                    QString("       departamento.descripcion,\n") +
                    QString("       distrito.dateofinsertion,\n") +
                    QString("       distrito.dateoflastupdate\n") +
                    QString("  from distrito, departamento\n") +
                     QString("  where departamento.nidvalue = distrito.idDepartamento\n") +
                    QString("order by distrito.nidvalue"), qx::QxSqlDatabase::getSingleton()->getDatabase());
    source->setHeaderData(0, Qt::Horizontal, tr("Codigo"));
    source->setHeaderData(1, Qt::Horizontal, tr("Descripcion"));
    source->setHeaderData(2, Qt::Horizontal, tr("Departamento"));
    source->setHeaderData(3, Qt::Horizontal, tr("Fecha de insercion"));
    source->setHeaderData(4, Qt::Horizontal, tr("Fecha de actualizacion"));
    QSortFilterProxyModel * model = new QSortFilterProxyModel(this);
    model->setSourceModel(source);
    setTableViewModel(model);
    tableView()->resizeColumnsToContents();
    tableView()->resizeRowsToContents();

    if(source->query().lastError().isValid())
        qDebug() << source->query().lastError().text();

    setWindowTitle(windowTitle());
}

QSqlError CatastroDistrito::insert(OrmClass *registro)
{
    Distrito *distrito = qobject_cast<Distrito *>(registro);
    return qx::dao::insert(distrito);
}

QSqlError CatastroDistrito::save(OrmClass *registro)
{
    Distrito *distrito = qobject_cast<Distrito *>(registro);
    return qx::dao::save(distrito);
}

OrmClass * CatastroDistrito::getRegistro(qlonglong codigo)
{
    if(codigo > 0)
    {
        Distrito *distrito = new Distrito(this);
        distrito->setIdValue(codigo);
        qx::dao::fetch_by_id(distrito);
        return distrito;
    }
    return new Distrito(this);
}

RegistroView * CatastroDistrito::getWidget()
{
    return new RegistroDistrito(this);
}


QString CatastroDistrito::formName()
{
    return QString("FORMCATEGORIA");
}

QString CatastroDistrito::windowTitle()
{
    return QString(tr("Catastro Distritos"));
}
