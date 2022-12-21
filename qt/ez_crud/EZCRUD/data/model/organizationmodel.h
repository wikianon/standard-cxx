// ****************************************************
// * Codigo Generado por DAOBuilder                   *
// * Fecha: 23/10/2012 07:48:24                       *
// * Luis Valdes 2012 - Todos los Derechos Reservados *
// ****************************************************
#ifndef OrganizationMODEL_H
#define OrganizationMODEL_H

#include <QAbstractTableModel>

#include "../../global/ezcrud_global.h"

namespace crud {

// Object Forward Declarations
class Organization;

class EZCRUD_EXPORT OrganizationModel : public QAbstractTableModel
{
	Q_OBJECT
public:
// Enum Declaration
	enum KOrganization 
	{
		KId = 0,
		KName = 1,
		KHostaddress = 2,
		KHostdatabase = 3,
		KHostuser = 4,
		KHostpassword = 5
	};

// Constructors
	OrganizationModel(QObject *parent = 0);
	OrganizationModel(QList<Organization *> *organizationList, QObject *parent = 0);

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
	bool addOrganization(Organization *);

// Private Member
private:
	QList<Organization *>  *m_organizationList;

};

} // namespace crud

#endif // ORGANIZATIONMODEL_H_H
