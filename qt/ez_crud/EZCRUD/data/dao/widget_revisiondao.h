// ****************************************************
// * Codigo Generado por DAOBuilder                   *
// * Fecha: 23/10/2012 17:40:10                       *
// * Luis Valdes 2012 - Todos los Derechos Reservados *
// ****************************************************
#ifndef WIDGET_REVISIONDAO_H
#define WIDGET_REVISIONDAO_H

#include "../genericdao.h"
#include "../../core/sql/exception/sqlexception.h"

class QSqlDatabase;

namespace crud {

class WidgetRevision;

class EZCRUD_EXPORT WidgetRevisionDAO : public GenericDAO<WidgetRevision>
{
	Q_OBJECT
protected:
    WidgetRevisionDAO(QObject *parent = 0);
public:
    static WidgetRevisionDAO * instance();
	bool insertObject(WidgetRevision *obj);
    bool insertObject(WidgetRevision *obj, QSqlDatabase *db) throw(SqlException);
	bool updateObject(WidgetRevision *obj);
	bool deleteObject(WidgetRevision *obj);
    WidgetRevision * getById(qlonglong id, QObject *parent=0);
    QList<WidgetRevision *> getByRevisionId(qlonglong idRevision, QObject *parent=0);
    QList<WidgetRevision *> getAll(QObject *parent=0);
    QList<WidgetRevision *> getByMatch(const QString &columnName, QVariant value, QObject *parent=0);
};

} // namespace crud
#endif // WIDGET_REVISIONDAO_H
