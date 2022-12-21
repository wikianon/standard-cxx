// ****************************************************
// * Codigo Generado por DAOBuilder                   *
// * Fecha: 23/10/2012 17:40:10                       *
// * Luis Valdes 2012 - Todos los Derechos Reservados *
// ****************************************************
#ifndef MODULE_REVISIONDAO_H
#define MODULE_REVISIONDAO_H

#include "../genericdao.h"
#include "../../core/sql/exception/sqlexception.h"

class QSqlDatabase;

namespace crud {

class ModuleRevision;

class EZCRUD_EXPORT ModuleRevisionDAO : public GenericDAO<ModuleRevision>
{
	Q_OBJECT
protected:
    ModuleRevisionDAO(QObject *parent = 0);
public:
    static ModuleRevisionDAO * instance();
	bool insertObject(ModuleRevision *obj);
    bool insertObject(ModuleRevision *obj, QSqlDatabase *db) throw(SqlException);
	bool updateObject(ModuleRevision *obj);
	bool deleteObject(ModuleRevision *obj);
    ModuleRevision * getById(qlonglong id, QObject *parent=0);
    QList<ModuleRevision *> getByRevisionId(qlonglong idRevision, QObject *parent=0);
    QList<ModuleRevision *> getAll(QObject *parent = 0);
    QList<ModuleRevision *> getByMatch(const QString &columnName, QVariant value, QObject *parent=0);
};

} // namespace crud
#endif // MODULE_REVISIONDAO_H
