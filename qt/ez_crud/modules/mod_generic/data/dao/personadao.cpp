// ****************************************************
// * Codigo Generado por DAOBuilder                   *
// * Fecha: 25/10/2012 08:33:59                       *
// * Luis Valdes 2012 - Todos los Derechos Reservados *
// ****************************************************
#include <QApplication>
#include <QSqlDatabase>
#include <QSqlRecord>

#include <core/sql/sqlquery.h>
#include <core/session.h>
#include <core/functions.h>

#include "personadao.h"
#include "../object/persona.h"

namespace crud {

PersonaDAO::PersonaDAO(QObject *parent) :
	GenericDAO<Persona>(parent)
{
}

PersonaDAO * PersonaDAO::instance()
{
	static PersonaDAO *instance = 0;
	if(! instance){
		instance = new PersonaDAO(qApp);
	}
	return instance;
}

bool PersonaDAO::insertObject(Persona *persona)
{
    QSqlDatabase *db = Session::instance()->organizationDatabase();
	db->transaction();
	try{
        qlonglong id = Functions::getSequence("seq_persona", db);
		persona->setId(id);
		SqlQuery query(db);
		query.prepare("insert into persona (id, tipopersona, razonsocial, ruc, barrio, calle1, calle2, telefono1, telefono2, email1, email2) " 
 				" values(:id, :tipopersona, :razonsocial, :ruc, :barrio, :calle1, :calle2, :telefono1, :telefono2, :email1, :email2) ");
		query.bindValue(":id", persona->id());
		query.bindValue(":tipopersona", persona->tipopersona());
		query.bindValue(":razonsocial", persona->razonsocial());
		query.bindValue(":ruc", persona->ruc());
		query.bindValue(":barrio", persona->barrio());
		query.bindValue(":calle1", persona->calle1());
		query.bindValue(":calle2", persona->calle2());
		query.bindValue(":telefono1", persona->telefono1());
		query.bindValue(":telefono2", persona->telefono2());
		query.bindValue(":email1", persona->email1());
		query.bindValue(":email2", persona->email2());
		query.exec();
		db->commit();
		return true;
	}catch(SqlException &ex){
		db->rollback();
		return false;
	}
}
bool PersonaDAO::updateObject(Persona *persona)
{
    QSqlDatabase *db = Session::instance()->organizationDatabase();
	db->transaction();
	try{
		SqlQuery query(db);
        query.prepare("update persona set tipopersona = :tipopersona, razonsocial = :razonsocial, ruc = :ruc, barrio = :barrio, calle1 = :calle1, calle2 = :calle2, telefono1 = :telefono1, telefono2 = :telefono2, email1 = :email1, email2 = :email2 where id = :id ");
        query.bindValue(":id", persona->id());
		query.bindValue(":tipopersona", persona->tipopersona());
		query.bindValue(":razonsocial", persona->razonsocial());
		query.bindValue(":ruc", persona->ruc());
		query.bindValue(":barrio", persona->barrio());
		query.bindValue(":calle1", persona->calle1());
		query.bindValue(":calle2", persona->calle2());
		query.bindValue(":telefono1", persona->telefono1());
		query.bindValue(":telefono2", persona->telefono2());
		query.bindValue(":email1", persona->email1());
		query.bindValue(":email2", persona->email2());
		query.exec();
		db->commit();
		return true;
	}catch(SqlException &ex){
		db->rollback();
		return false;
	}
}
bool PersonaDAO::deleteObject(Persona *)
{
	return false;
}
Persona * PersonaDAO::getById(qlonglong id, QObject *parent)
{
    QSqlDatabase *db = Session::instance()->organizationDatabase();
	try{
		SqlQuery query(db);
		query.prepare("select id, tipopersona, razonsocial, ruc, barrio, calle1, calle2, telefono1, telefono2, email1, email2 from persona  where id = :id ");
		query.bindValue(":id", id);
		query.exec();
		Persona *persona = 0;
		if(query.next()){ 
            persona = new Persona(query.record(), PARENT(parent));
		}
		return persona;
	}catch(SqlException &ex){
		return 0;
	}
}
QList<Persona *> PersonaDAO::getAll(QObject *parent)
{
	QList<Persona *> list;
    QSqlDatabase *db = Session::instance()->organizationDatabase();
	try{
		SqlQuery query(db);
		query.prepare("select id, tipopersona, razonsocial, ruc, barrio, calle1, calle2, telefono1, telefono2, email1, email2 from persona ");
		query.exec();
		while(query.next()){ 
            list.append(new Persona(query.record(), PARENT(parent)));
		} 
		return list;
	}catch(SqlException &ex){
		qDeleteAll(list);
		return list;
	}
}
QList<Persona *> PersonaDAO::getByMatch(const QString &ColumnObject, QVariant value, QObject *parent)
{
	QList<Persona *> list;
    QSqlDatabase *db = Session::instance()->organizationDatabase();
	try{
		SqlQuery query(db);
		QString sqlQuery = QString("select id, tipopersona, razonsocial, ruc, barrio, calle1, calle2, telefono1, telefono2, email1, email2 from persona  where %1 = :value ").arg(ColumnObject);
		query.prepare(sqlQuery);
		query.bindValue(":value", value);
		query.exec();
		while(query.next()){ 
            list.append(new Persona(query.record(), PARENT(parent)));
		} 
		return list;
	}catch(SqlException &ex){
		qDeleteAll(list);
		return list;
	}
}
} // namespace crud
