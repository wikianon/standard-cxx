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

#include "persona_categoriadao.h"
#include "../object/persona_categoria.h"

namespace crud {

PersonaCategoriaDAO::PersonaCategoriaDAO(QObject *parent) :
	GenericDAO<PersonaCategoria>(parent)
{
}

PersonaCategoriaDAO * PersonaCategoriaDAO::instance()
{
	static PersonaCategoriaDAO *instance = 0;
	if(! instance){
		instance = new PersonaCategoriaDAO(qApp);
	}
	return instance;
}

bool PersonaCategoriaDAO::insertObject(PersonaCategoria *persona_categoria)
{
    QSqlDatabase *db = Session::instance()->organizationDatabase();
	db->transaction();
	try{
		SqlQuery query(db);
		query.prepare("insert into persona_categoria (idpersona, idcategoria) " 
 				" values(:idpersona, :idcategoria) ");
		query.bindValue(":idpersona", persona_categoria->idpersona());
		query.bindValue(":idcategoria", persona_categoria->idcategoria());
		query.exec();
		db->commit();
		return true;
	}catch(SqlException &ex){
		db->rollback();
		return false;
    }
}

bool PersonaCategoriaDAO::insertObjectList(qlonglong idPersona, QList<PersonaCategoria *> &objectList)
{
    QSqlDatabase *db = Session::instance()->organizationDatabase();
    db->transaction();
    try{
        SqlQuery query(db);
        // Delete all PersonaCategoria for the current Persona
        query.prepare("delete from persona_categoria where idpersona = :idpersona");
        query.bindValue(":idpersona", idPersona);
        query.exec();

        // Insert new values for the current Persona
        foreach(PersonaCategoria *persona_categoria, objectList){
            query.prepare("insert into persona_categoria (idpersona, idcategoria) "
                    " values(:idpersona, :idcategoria) ");
            query.bindValue(":idpersona", persona_categoria->idpersona());
            query.bindValue(":idcategoria", persona_categoria->idcategoria());
            query.exec();
        }
        db->commit();
        return true;
    }catch(SqlException &ex){
        db->rollback();
        return false;
    }
}

bool PersonaCategoriaDAO::updateObject(PersonaCategoria *persona_categoria)
{
    QSqlDatabase *db = Session::instance()->organizationDatabase();
	db->transaction();
	try{
		SqlQuery query(db);
		query.prepare("update persona_categoria set idpersona = :idpersona, idcategoria = :idcategoriawhere id = :id ");
		query.bindValue(":idpersona", persona_categoria->idpersona());
		query.bindValue(":idcategoria", persona_categoria->idcategoria());
		query.exec();
		db->commit();
		return true;
	}catch(SqlException &ex){
		db->rollback();
		return false;
    }
}

bool PersonaCategoriaDAO::deleteObject(PersonaCategoria *obj)
{
    return false;
}

bool PersonaCategoriaDAO::deletePersonaCategoria(qlonglong idPersona)
{
    return false;
}

PersonaCategoria * PersonaCategoriaDAO::getById(qlonglong id, QObject *parent)
{
    QSqlDatabase *db = Session::instance()->organizationDatabase();
	try{
		SqlQuery query(db);
		query.prepare("select idpersona, idcategoria from persona_categoria  where id = :id ");
		query.bindValue(":id", id);
		query.exec();
		PersonaCategoria *persona_categoria = 0;
		if(query.next()){ 
            persona_categoria = new PersonaCategoria(query.record(), PARENT(parent));
		}
		return persona_categoria;
	}catch(SqlException &ex){
		return 0;
	}
}
QList<PersonaCategoria *> PersonaCategoriaDAO::getAll(QObject *parent)
{
	QList<PersonaCategoria *> list;
    QSqlDatabase *db = Session::instance()->organizationDatabase();
	try{
		SqlQuery query(db);
		query.prepare("select idpersona, idcategoria from persona_categoria ");
		query.exec();
		while(query.next()){ 
            list.append(new PersonaCategoria(query.record(), PARENT(parent)));
		} 
		return list;
	}catch(SqlException &ex){
        // deleteAll(list);
		return list;
	}
}
QList<PersonaCategoria *> PersonaCategoriaDAO::getByMatch(const QString &ColumnObject, QVariant value, QObject *parent)
{
	QList<PersonaCategoria *> list;
    QSqlDatabase *db = Session::instance()->organizationDatabase();
	try{
		SqlQuery query(db);
		QString sqlQuery = QString("select idpersona, idcategoria from persona_categoria  where %1 = :value ").arg(ColumnObject);
		query.prepare(sqlQuery);
		query.bindValue(":value", value);
		query.exec();
		while(query.next()){ 
            list.append(new PersonaCategoria(query.record(), PARENT(parent)));
		} 
		return list;
	}catch(SqlException &ex){
        // deleteAll(list);
		return list;
	}
}
} // namespace crud
