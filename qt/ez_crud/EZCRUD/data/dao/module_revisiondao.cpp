// ****************************************************
// * Codigo Generado por DAOBuilder                   *
// * Fecha: 23/10/2012 17:40:10                       *
// * Luis Valdes 2012 - Todos los Derechos Reservados *
// ****************************************************
#include <QApplication>
#include <QSqlDatabase>
#include <QSqlRecord>

#include <core/sql/sqlquery.h>
#include <core/session.h>
#include <core/functions.h>

#include "module_revisiondao.h"
#include "../object/module_revision.h"

namespace crud {

ModuleRevisionDAO::ModuleRevisionDAO(QObject *parent) :
    GenericDAO<ModuleRevision>(parent)
{
}

ModuleRevisionDAO * ModuleRevisionDAO::instance()
{
    static ModuleRevisionDAO *instance = 0;
    if(! instance){
        instance = new ModuleRevisionDAO(qApp);
    }
    return instance;
}

bool ModuleRevisionDAO::insertObject(ModuleRevision *module_revision)
{
    QSqlDatabase *db = Session::instance()->systemDatabase();
    db->transaction();
    try{
        SqlQuery query(db);
        query.prepare("insert into module_revision (idrevision, idmodule) "
                      " values(:idrevision, :idmodule) ");
        query.bindValue(":idrevision", module_revision->idRevision());
        query.bindValue(":idmodule", module_revision->idModule());
        query.exec();
        db->commit();
        return true;
    }catch(SqlException &ex){
        db->rollback();
        return false;
    }
}

bool ModuleRevisionDAO::insertObject(ModuleRevision *obj, QSqlDatabase *db) throw(SqlException)
{
    SqlQuery query(db);
    query.prepare("insert into module_revision (idrevision, idmodule) "
                  " values(:idrevision, :idmodule) ");
    query.bindValue(":idrevision", obj->idRevision());
    query.bindValue(":idmodule", obj->idModule());
    query.exec();
    return true;
}

bool ModuleRevisionDAO::updateObject(ModuleRevision *module_revision)
{
    QSqlDatabase *db = Session::instance()->systemDatabase();
    db->transaction();
    try{
        SqlQuery query(db);
        query.prepare("update module_revision set idrevision = :idrevision, idmodule = :idmodulewhere id = :id ");
        query.bindValue(":idrevision", module_revision->idRevision());
        query.bindValue(":idmodule", module_revision->idModule());
        query.exec();
        db->commit();
        return true;
    }catch(SqlException &ex){
        db->rollback();
        return false;
    }
}
bool ModuleRevisionDAO::deleteObject(ModuleRevision *)
{
    return false;
}
ModuleRevision * ModuleRevisionDAO::getById(qlonglong id, QObject *parent)
{
    QSqlDatabase *db = Session::instance()->systemDatabase();
    try{
        SqlQuery query(db);
        query.prepare("select idrevision, idmodule from module_revision  where id = :id ");
        query.bindValue(":id", id);
        query.exec();
        ModuleRevision *module_revision = 0;
        if(query.next()){
            module_revision = new ModuleRevision(query.record(), PARENT(parent));
        }
        return module_revision;
    }catch(SqlException &ex){
        return 0;
    }
}

QList<ModuleRevision *> ModuleRevisionDAO::getByRevisionId(qlonglong idRevision, QObject *parent)
{
    QList<ModuleRevision *> list;
    QSqlDatabase *db = Session::instance()->systemDatabase();
    try{
        SqlQuery query(db);
        query.prepare("select idrevision, idmodule from module_revision  where idrevision = :idrevision ");
        query.bindValue(":idrevision", idRevision);
        query.exec();
        while(query.next()){
            list.append(new ModuleRevision(query.record(), PARENT(parent)));
        }
        return list;
    }catch(SqlException &ex){

    }
    return list;
}

QList<ModuleRevision *> ModuleRevisionDAO::getAll(QObject *parent)
{
    QList<ModuleRevision *> list;
    QSqlDatabase *db = Session::instance()->systemDatabase();
    try{
        SqlQuery query(db);
        query.prepare("select idrevision, idmodule from module_revision ");
        query.exec();
        while(query.next()){
            list.append(new ModuleRevision(query.record(), PARENT(parent)));
        }
        return list;
    }catch(SqlException &ex){
        // deleteAll(list);
        return list;
    }
}
QList<ModuleRevision *> ModuleRevisionDAO::getByMatch(const QString &ColumnObject, QVariant value, QObject *parent)
{
    QList<ModuleRevision *> list;
    QSqlDatabase *db = Session::instance()->systemDatabase();
    try{
        SqlQuery query(db);
        QString sqlQuery = QString("select idrevision, idmodule from module_revision  where %1 = :value ").arg(ColumnObject);
        query.prepare(sqlQuery);
        query.bindValue(":value", value);
        query.exec();
        while(query.next()){
            list.append(new ModuleRevision(query.record(), PARENT(parent)));
        }
        return list;
    }catch(SqlException &ex){
        // deleteAll(list);
        return list;
    }
}
} // namespace crud
