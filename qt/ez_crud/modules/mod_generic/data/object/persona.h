// ****************************************************
// * Codigo Generado por DAOBuilder                   *
// * Fecha: 25/10/2012 08:33:59                       *
// * Luis Valdes 2012 - Todos los Derechos Reservados *
// ****************************************************
#ifndef PERSONA_H
#define PERSONA_H

#include <QSqlRecord>
#include <data/objectbase.h>

namespace crud {

// Objects Forward Declarations
class Categoria;

class EZCRUD_EXPORT Persona : public ObjectBase
{
	Q_OBJECT
	Q_PROPERTY(QVariant tipopersona
		READ tipopersona
		WRITE setTipopersona)
	Q_PROPERTY(QVariant razonsocial
		READ razonsocial
		WRITE setRazonsocial)
	Q_PROPERTY(QVariant ruc
		READ ruc
		WRITE setRuc)
	Q_PROPERTY(QVariant barrio
		READ barrio
		WRITE setBarrio)
	Q_PROPERTY(QVariant calle1
		READ calle1
		WRITE setCalle1)
	Q_PROPERTY(QVariant calle2
		READ calle2
		WRITE setCalle2)
	Q_PROPERTY(QVariant telefono1
		READ telefono1
		WRITE setTelefono1)
	Q_PROPERTY(QVariant telefono2
		READ telefono2
		WRITE setTelefono2)
	Q_PROPERTY(QVariant email1
		READ email1
		WRITE setEmail1)
	Q_PROPERTY(QVariant email2
		READ email2
		WRITE setEmail2)

public:
    enum TipoPersona{KFisica = 1, KJuridica=2};
// Constructors
	explicit Persona(QObject *parent = 0);
    explicit Persona(const QSqlRecord &record, QObject *parent = 0);

// Getters & Setters
	QVariant tipopersona() const;
	void setTipopersona(QVariant);
	QVariant razonsocial() const;
	void setRazonsocial(QVariant);
	QVariant ruc() const;
	void setRuc(QVariant);
	QVariant barrio() const;
	void setBarrio(QVariant);
	QVariant calle1() const;
	void setCalle1(QVariant);
	QVariant calle2() const;
	void setCalle2(QVariant);
	QVariant telefono1() const;
	void setTelefono1(QVariant);
	QVariant telefono2() const;
	void setTelefono2(QVariant);
	QVariant email1() const;
	void setEmail1(QVariant);
	QVariant email2() const;
	void setEmail2(QVariant);

    QList<Categoria *> & categoriaList();
// Objects Getters & Setters

private:
// Private Members
    int m_tipopersona;
	QString m_razonsocial;
	QString m_ruc;
	QString m_barrio;
	QString m_calle1;
	QString m_calle2;
	QString m_telefono1;
	QString m_telefono2;
	QString m_email1;
	QString m_email2;
    QList<Categoria *> m_categoriaList;

private:
// Private Object Members
// Other Methodsprivate:

};

} // namespace crud

#endif // PERSONA_H
