// ****************************************************
// * Codigo Generado por DAOBuilder                   *
// * Fecha: 25/10/2012 08:33:59                       *
// * Luis Valdes 2012 - Todos los Derechos Reservados *
// ****************************************************
#ifndef PERSONAMODEL_H
#define PERSONAMODEL_H
#include <QAbstractTableModel>

#include <global/ezcrud_global.h>

namespace crud {

// Object Forward Declarations
class Persona;

class EZCRUD_EXPORT PersonaModel : public QAbstractTableModel
{
	Q_OBJECT
public:
// Enum Declaration
	enum KPersona 
	{
		KId = 0,
		KTipopersona = 1,
		KRazonsocial = 2,
		KRuc = 3,
		KBarrio = 4,
		KCalle1 = 5,
		KCalle2 = 6,
		KTelefono1 = 7,
		KTelefono2 = 8,
		KEmail1 = 9,
		KEmail2 = 10
	};

// Constructors
	PersonaModel(QObject *parent = 0);
	PersonaModel(QList<Persona *> *personaList, QObject *parent = 0);

// Model Methods
	QVariant data(const QModelIndex &index, int role) const;
	bool setData(const QModelIndex &index, const QVariant &value, int role);
	QVariant headerData(int section, Qt::Orientation orientation, int role) const;
	Qt::ItemFlags flags(const QModelIndex &index) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
	bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

// Add Method
	bool addPersona(Persona *);

// Private Member
private:
	QList<Persona *>  *m_personaList;

};

} // namespace crud

#endif // PERSONAMODEL_H_H
