#include <QSqlQueryModel>
#include <QSortFilterProxyModel>
#include <QTableView>

#include "catastrodepartamento.h"
#include "registrodepartamento.h"
#include "orm/departamento/departamento.h"

#include "orm/precompiled.h"

CatastroDepartamento::CatastroDepartamento(QWidget *parent) :
    CatastroBase(parent)
{
    QSqlQueryModel *source = new QSqlQueryModel(this);
    source->setQuery(QString("select departamento.nidvalue,\n") +
                    QString("       departamento.descripcion,\n") +
                    QString("       departamento.dateofinsertion,\n") +
                    QString("       departamento.dateoflastupdate\n") +
                    QString("  from departamento\n") +
                    QString("order by departamento.nidvalue"), qx::QxSqlDatabase::getSingleton()->getDatabase());
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

QSqlError CatastroDepartamento::insert(OrmClass *registro)
{
    Departamento *departamento = qobject_cast<Departamento *>(registro);
    return qx::dao::insert(departamento);
}

QSqlError CatastroDepartamento::save(OrmClass *registro)
{
    Departamento *departamento = qobject_cast<Departamento *>(registro);
    return qx::dao::save(departamento);
}

OrmClass * CatastroDepartamento::getRegistro(qlonglong codigo)
{
    if(codigo > 0)
    {
        Departamento *departamento = new Departamento(this);
        departamento->setIdValue(codigo);
        qx::dao::fetch_by_id(departamento);
        return departamento;
    }
    return new Departamento(this);
}

RegistroView * CatastroDepartamento::getWidget()
{
    return new RegistroDepartamento(this);
}


QString CatastroDepartamento::formName()
{
    return QString("FORMCATEGORIA");
}

QString CatastroDepartamento::windowTitle()
{
    return QString(tr("Catastro Departamentos"));
}
