// ****************************************************
// * Codigo Generado por DAOBuilder                   *
// * Fecha: 23/10/2012 17:40:10                       *
// * Luis Valdes 2012 - Todos los Derechos Reservados *
// ****************************************************
#include "module_revision.h"
// Include Objects
#include "revision.h"
#include "module.h"
// Include DAOs
#include "../dao/revisiondao.h"
#include "../dao/moduledao.h"

namespace crud {

ModuleRevision::ModuleRevision(QObject *parent) :
    QObject(parent),
    m_idrevision(-1),
    m_idmodule(-1),
    m_revision(0),
    m_module(0)
{

}

ModuleRevision::ModuleRevision(const QSqlRecord &record, QObject *parent) :
    QObject(parent),
    m_idrevision(-1),
    m_idmodule(-1),
    m_revision(0),
    m_module(0)
{
    // idrevision, idmodule
    setIdRevision(record.value(0));
    setIdModule(record.value(1));
}

// Getters y Setters Definitions
QVariant ModuleRevision::idRevision() const
{
    return m_idrevision;
}

void ModuleRevision::setIdRevision(QVariant value)
{
    m_idrevision = value.toLongLong();
}

QVariant ModuleRevision::idModule() const
{
    return m_idmodule;
}

void ModuleRevision::setIdModule(QVariant value)
{
    m_idmodule = value.toLongLong();
}

Revision *ModuleRevision::revision()
{
    if (! m_revision && (m_idrevision != -1)){
        m_revision = RevisionDAO::instance()->getById(m_idrevision, this);
    }
    return m_revision;
}

void ModuleRevision::setRevision(Revision *obj)
{
    m_revision = obj;
}

// Objects Getters y Setters Definitions
Module * ModuleRevision::module()
{
    if (! m_module && (m_idmodule != -1)){
        m_module = ModuleDAO::instance()->getById(m_idmodule, this);
    }
    return m_module;
}

void ModuleRevision::setModule(Module *obj)
{
    m_module = obj;
}

} // namespace crud
