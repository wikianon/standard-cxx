// ****************************************************
// * Codigo Generado por DAOBuilder                   *
// * Fecha: 25/10/2012 08:33:59                       *
// * Luis Valdes 2012 - Todos los Derechos Reservados *
// ****************************************************
#include "persona.h"

#include "../dao/categoriadao.h"

namespace crud {

Persona::Persona(QObject *parent) :
	ObjectBase(parent)
{

}

Persona::Persona(const QSqlRecord &record, QObject *parent) :
	ObjectBase(parent)
{
	// id, tipopersona, razonsocial, ruc, barrio, calle1, calle2, telefono1, telefono2, email1, email2
    setId(record.value(0));
    setTipopersona(record.value(1));
    setRazonsocial(record.value(2));
    setRuc(record.value(3));
    setBarrio(record.value(4));
    setCalle1(record.value(5));
    setCalle2(record.value(6));
    setTelefono1(record.value(7));
    setTelefono2(record.value(8));
    setEmail1(record.value(9));
    setEmail2(record.value(10));
}

// Getters y Setters Definitions
QVariant Persona::tipopersona() const
{
	return m_tipopersona;
}

void Persona::setTipopersona(QVariant value)
{
    m_tipopersona = value.toInt();
}

QVariant Persona::razonsocial() const
{
	return m_razonsocial;
}

void Persona::setRazonsocial(QVariant value)
{
	m_razonsocial = value.toString();
}

QVariant Persona::ruc() const
{
	return m_ruc;
}

void Persona::setRuc(QVariant value)
{
	m_ruc = value.toString();
}

QVariant Persona::barrio() const
{
	return m_barrio;
}

void Persona::setBarrio(QVariant value)
{
	m_barrio = value.toString();
}

QVariant Persona::calle1() const
{
	return m_calle1;
}

void Persona::setCalle1(QVariant value)
{
	m_calle1 = value.toString();
}

QVariant Persona::calle2() const
{
	return m_calle2;
}

void Persona::setCalle2(QVariant value)
{
	m_calle2 = value.toString();
}

QVariant Persona::telefono1() const
{
	return m_telefono1;
}

void Persona::setTelefono1(QVariant value)
{
	m_telefono1 = value.toString();
}

QVariant Persona::telefono2() const
{
	return m_telefono2;
}

void Persona::setTelefono2(QVariant value)
{
	m_telefono2 = value.toString();
}

QVariant Persona::email1() const
{
	return m_email1;
}

void Persona::setEmail1(QVariant value)
{
	m_email1 = value.toString();
}

QVariant Persona::email2() const
{
	return m_email2;
}

void Persona::setEmail2(QVariant value)
{
    m_email2 = value.toString();
}

QList<Categoria *> &Persona::categoriaList()
{
    if((m_id != -1) && m_categoriaList.count() == 0){
        m_categoriaList = CategoriaDAO::instance()->getByIdPersona(m_id, this);
    }
    return m_categoriaList;
}

} // namespace crud
