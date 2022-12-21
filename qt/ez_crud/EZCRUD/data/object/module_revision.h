// ****************************************************
// * Codigo Generado por DAOBuilder                   *
// * Fecha: 23/10/2012 17:40:10                       *
// * Luis Valdes 2012 - Todos los Derechos Reservados *
// ****************************************************
#ifndef MODULE_REVISION_H
#define MODULE_REVISION_H

#include <QSqlRecord>
#include <data/objectbase.h>

namespace crud {

// Objects Forward Declarations

class Revision;
class Module;

class EZCRUD_EXPORT ModuleRevision : public QObject
{
	Q_OBJECT
    Q_PROPERTY(QVariant idRevision
               READ idRevision
               WRITE setIdRevision)
    Q_PROPERTY(QVariant idModule
               READ idModule
               WRITE setIdModule)

public:
// Constructors
    explicit ModuleRevision(QObject *parent = 0);
    explicit ModuleRevision(const QSqlRecord &record, QObject *parent = 0);

// Getters & Setters
    QVariant idRevision() const;
    void setIdRevision(QVariant);
    QVariant idModule() const;
    void setIdModule(QVariant);

// Objects Getters & Setters
    Revision * revision();
    void setRevision(Revision *obj);
	Module * module();
    void setModule(Module *obj);

private:
// Private Members
	qlonglong m_idrevision;
	qlonglong m_idmodule;

private:
// Private Object Members
    Revision *m_revision;
    Module *m_module;
};

} // namespace crud

#endif // MODULE_REVISION_H
