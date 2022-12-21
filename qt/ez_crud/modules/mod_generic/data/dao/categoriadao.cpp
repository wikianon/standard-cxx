/****************************************************************************
**
** http://sourceforge.net/projects/easy-crud/
**
** Copyright (C) 2010, 2011, 2012  Luis Valdes (luisvaldes88@gmail.com)
**
** This file is part of the EZCRUD library
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**
****************************************************************************/

// ****************************************************
// * Codigo Generado por DAOBuilder                   *
// * Fecha: 28/09/2012 15:26:19                       *
// * Luis Valdes 2012 - Todos los Derechos Reservados *
// ****************************************************
#include <QApplication>
#include <QSqlDatabase>
#include <QSqlRecord>

#include <core/sql/sqlquery.h>
#include <core/session.h>
#include <core/functions.h>

#include "categoriadao.h"
#include "../object/categoria.h"

namespace crud {

CategoriaDAO::CategoriaDAO(QObject *parent) :
    GenericDAO<Categoria>(parent)
{
}

CategoriaDAO * CategoriaDAO::instance()
{
    static CategoriaDAO *instance = 0;
    if(! instance){
        instance = new CategoriaDAO(qApp);
    }
    return instance;
}

bool CategoriaDAO::insertObject(Categoria *categoria)
{
    QSqlDatabase *db = Session::instance()->organizationDatabase();
    db->transaction();
    try{
        qlonglong id = Functions::getSequence("seq_categoria", db);
        categoria->setId(id);
        SqlQuery query(db);
        query.prepare("insert into categoria (id, nombre) "
                      " values(:id, :nombre) ");
        query.bindValue(":id", categoria->id());
        query.bindValue(":nombre", categoria->nombre());
        query.exec();
        db->commit();
        return true;
    }catch(SqlException &ex){
        db->rollback();
        return false;
    }
}
bool CategoriaDAO::updateObject(Categoria *categoria)
{
    QSqlDatabase *db = Session::instance()->organizationDatabase();
    db->transaction();
    try{
        SqlQuery query(db);
        query.prepare("update categoria set nombre = :nombre where id = :id ");
        query.bindValue(":id", categoria->id());
        query.bindValue(":nombre", categoria->nombre());
        query.exec();
        db->commit();
        return true;
    }catch(SqlException &ex){
        db->rollback();
        return false;
    }
}
bool CategoriaDAO::deleteObject(Categoria *)
{
    return false;
}
Categoria * CategoriaDAO::getById(qlonglong id, QObject *parent)
{
    QSqlDatabase *db = Session::instance()->organizationDatabase();
    try{
        SqlQuery query(db);
        query.prepare("select id, nombre from categoria  where id = :id ");
        query.bindValue(":id", id);
        query.exec();
        Categoria *categoria = 0;
        if(query.next()){
            categoria = new Categoria(query.record(), PARENT(parent));
        }
        return categoria;
    }catch(SqlException &ex){
        return 0;
    }
}

QList<Categoria *> CategoriaDAO::getByIdPersona(qlonglong idPersona, QObject *parent)
{
    QList<Categoria *> list;
    QSqlDatabase *db = Session::instance()->organizationDatabase();
    try{
        SqlQuery query(db);
        query.prepare("select c.id, c.nombre from categoria c, persona_categoria pc "
                      " where c.id  = pc.idcategoria and pc.idpersona = :idpersona ");
        query.bindValue(":idpersona", idPersona);
        query.exec();
        while(query.next()){
            list.append(new Categoria(query.record(), PARENT(parent)));
        }
        return list;
    }catch(SqlException &ex){
        return list;
    }
}

QList<Categoria *> CategoriaDAO::getAll(QObject *parent)
{
    QList<Categoria *> list;
    QSqlDatabase *db = Session::instance()->organizationDatabase();
    try{
        SqlQuery query(db);
        query.prepare("select id, nombre from categoria ");
        query.exec();
        while(query.next()){
            list.append(new Categoria(query.record(), PARENT(parent)));
        }
        return list;
    }catch(SqlException &ex){
        // deleteAll(list);
        return list;
    }
}

QList<Categoria *> CategoriaDAO::getByMatch(const QString &ColumnObject, QVariant value, QObject *parent)
{
    QList<Categoria *> list;
    QSqlDatabase *db = Session::instance()->organizationDatabase();
    try{
        SqlQuery query(db);
        QString sqlQuery = QString("select id, nombre from categoria  where %1 = :value ").arg(ColumnObject);
        query.prepare(sqlQuery);
        query.bindValue(":value", value);
        query.exec();
        while(query.next()){
            list.append(new Categoria(query.record(), PARENT(parent)));
        }
        return list;
    }catch(SqlException &ex){
        // deleteAll(list);
        return list;
    }
}

} // namespace crud
