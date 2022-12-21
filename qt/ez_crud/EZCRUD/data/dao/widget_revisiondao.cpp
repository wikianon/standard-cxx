// ****************************************************
// * Codigo Generado por DAOBuilder                   *
// * Fecha: 23/10/2012 17:40:10                       *
// * Luis Valdes 2012 - Todos los Derechos Reservados *
// ****************************************************
#include <QApplication>
#include <QSqlDatabase>
#include <QSqlRecord>


#include "../../core/sql/exception/sqlexception.h"

#include "../../core/sql/sqlquery.h"
#include "../../core/session.h"
#include "../../core/functions.h"

#include "widget_revisiondao.h"
#include "../object/widget_revision.h"

namespace crud {

WidgetRevisionDAO::WidgetRevisionDAO(QObject *parent) :
    GenericDAO<WidgetRevision>(parent)
{
}

WidgetRevisionDAO * WidgetRevisionDAO::instance()
{
    static WidgetRevisionDAO *instance = 0;
    if(! instance){
        instance = new WidgetRevisionDAO(qApp);
    }
    return instance;
}

bool WidgetRevisionDAO::insertObject(WidgetRevision *widget_revision)
{
    QSqlDatabase *db = Session::instance()->systemDatabase();
    db->transaction();
    try{
        SqlQuery query(db);
        query.prepare("insert into widget_revision (idrevision, idwidget, accessmode) "
                      " values(:idrevision, :idwidget, :accessmode) ");
        query.bindValue(":idrevision", widget_revision->idRevision());
        query.bindValue(":idwidget", widget_revision->idWidget());
        query.bindValue(":accessmode", widget_revision->accessMode());
        query.exec();
        db->commit();
        return true;
    }catch(SqlException &ex){
        db->rollback();
        return false;
    }
}

bool WidgetRevisionDAO::insertObject(WidgetRevision *obj, QSqlDatabase *db) throw(SqlException)
{
    SqlQuery query(db);
    query.prepare("insert into widget_revision (idrevision, idwidget, accessmode) "
                  " values(:idrevision, :idwidget, :accessmode) ");
    query.bindValue(":idrevision", obj->idRevision());
    query.bindValue(":idwidget", obj->idWidget());
    query.bindValue(":accessmode", obj->accessMode());
    query.exec();
    return true;
}

bool WidgetRevisionDAO::updateObject(WidgetRevision *widget_revision)
{
    QSqlDatabase *db = Session::instance()->systemDatabase();
    db->transaction();
    try{
        SqlQuery query(db);
        query.prepare("update widget_revision set idrevision = :idrevision, idwidget = :idwidget, accessmode = :accessmodewhere id = :id ");
        query.bindValue(":idrevision", widget_revision->idRevision());
        query.bindValue(":idwidget", widget_revision->idWidget());
        query.bindValue(":accessmode", widget_revision->accessMode());
        query.exec();
        db->commit();
        return true;
    }catch(SqlException &ex){
        db->rollback();
        return false;
    }
}

bool WidgetRevisionDAO::deleteObject(WidgetRevision *)
{
    return false;
}

WidgetRevision * WidgetRevisionDAO::getById(qlonglong id, QObject *parent)
{
    QSqlDatabase *db = Session::instance()->systemDatabase();
    try{
        SqlQuery query(db);
        query.prepare("select idrevision, idwidget, accessmode from widget_revision  where id = :id ");
        query.bindValue(":id", id);
        query.exec();
        WidgetRevision *widget_revision = 0;
        if(query.next()){
            widget_revision = new WidgetRevision(query.record(), PARENT(parent));
        }
        return widget_revision;
    }catch(SqlException &ex){
        return 0;
    }
}

QList<WidgetRevision *> WidgetRevisionDAO::getByRevisionId(qlonglong idRevision, QObject *parent)
{
    QList<WidgetRevision *> list;
    QSqlDatabase *db = Session::instance()->systemDatabase();
    try{
        SqlQuery query(db);
        query.prepare("select idrevision, idwidget, accessmode from widget_revision  where idrevision = :idrevision ");
        query.bindValue(":idrevision", idRevision);
        query.exec();
        while(query.next()){
            list.append(new WidgetRevision(query.record(), PARENT(parent)));
        }
        return list;
    }catch(SqlException &ex){

    }
    return list;
}

QList<WidgetRevision *> WidgetRevisionDAO::getAll(QObject *parent)
{
    QList<WidgetRevision *> list;
    QSqlDatabase *db = Session::instance()->systemDatabase();
    try{
        SqlQuery query(db);
        query.prepare("select idrevision, idwidget, accessmode from widget_revision ");
        query.exec();
        while(query.next()){
            list.append(new WidgetRevision(query.record(), PARENT(parent)));
        }
        return list;
    }catch(SqlException &ex){
        // deleteAll(list);
        return list;
    }
}
QList<WidgetRevision *> WidgetRevisionDAO::getByMatch(const QString &ColumnObject, QVariant value, QObject *parent)
{
    QList<WidgetRevision *> list;
    QSqlDatabase *db = Session::instance()->systemDatabase();
    try{
        SqlQuery query(db);
        QString sqlQuery = QString("select idrevision, idwidget, accessmode from widget_revision  where %1 = :value ").arg(ColumnObject);
        query.prepare(sqlQuery);
        query.bindValue(":value", value);
        query.exec();
        while(query.next()){
            list.append(new WidgetRevision(query.record(), PARENT(parent)));
        }
        return list;
    }catch(SqlException &ex){
        // deleteAll(list);
        return list;
    }
}
} // namespace crud
