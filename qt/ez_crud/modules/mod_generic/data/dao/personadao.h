// ****************************************************
// * Codigo Generado por DAOBuilder                   *
// * Fecha: 25/10/2012 08:33:59                       *
// * Luis Valdes 2012 - Todos los Derechos Reservados *
// ****************************************************
#ifndef PERSONADAO_H
#define PERSONADAO_H

#include <data/genericdao.h>

namespace crud {

class Persona;

class EZCRUD_EXPORT PersonaDAO : public GenericDAO<Persona>
{
	Q_OBJECT
protected:
	PersonaDAO(QObject *parent = 0);
public:
	static PersonaDAO * instance();
	bool insertObject(Persona *obj);
	bool updateObject(Persona *obj);
	bool deleteObject(Persona *obj);
    Persona * getById(qlonglong id, QObject *parent=0);
    QList<Persona *> getAll(QObject *parent=0);
    QList<Persona *> getByMatch(const QString &columnName, QVariant value, QObject *parent=0);
};

} // namespace crud
#endif // PERSONADAO_H
