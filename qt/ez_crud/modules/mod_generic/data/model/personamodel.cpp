// ****************************************************
// * Codigo Generado por DAOBuilder                   *
// * Fecha: 25/10/2012 08:33:59                       *
// * Luis Valdes 2012 - Todos los Derechos Reservados *
// ****************************************************
#include "personamodel.h"
#include "../object/persona.h"

namespace crud {

PersonaModel::PersonaModel(QObject *parent) :
	QAbstractTableModel(parent)
{

}

PersonaModel::PersonaModel(QList<Persona *> *personaList, QObject *parent) :
	QAbstractTableModel(parent)
{

	m_personaList = personaList;
}

QVariant PersonaModel::data(const QModelIndex &index, int role) const
{
	if(!index.isValid()) 
		return QVariant();
	const Persona &obj = *(m_personaList->at(index.row()));
	if((role == Qt::DisplayRole) || (role == Qt::EditRole))
	{
		switch(index.column())
		{
		case KId: 
			return obj.id();
		case KTipopersona: 
			return obj.tipopersona();
		case KRazonsocial: 
			return obj.razonsocial();
		case KRuc: 
			return obj.ruc();
		case KBarrio: 
			return obj.barrio();
		case KCalle1: 
			return obj.calle1();
		case KCalle2: 
			return obj.calle2();
		case KTelefono1: 
			return obj.telefono1();
		case KTelefono2: 
			return obj.telefono2();
		case KEmail1: 
			return obj.email1();
		case KEmail2: 
			return obj.email2();
		default: 
			qWarning("Error: PersonaModel::data()");
		}
	}
	return QVariant();
}

bool PersonaModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	return false;
}

QVariant PersonaModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if(role != Qt::DisplayRole) 
		return QVariant();
	if(orientation == Qt::Horizontal)
	{
		switch(section)
		{
		case KId: 
			return tr("Id");
		case KTipopersona: 
			return tr("Tipopersona");
		case KRazonsocial: 
			return tr("Razonsocial");
		case KRuc: 
			return tr("Ruc");
		case KBarrio: 
			return tr("Barrio");
		case KCalle1: 
			return tr("Calle1");
		case KCalle2: 
			return tr("Calle2");
		case KTelefono1: 
			return tr("Telefono1");
		case KTelefono2: 
			return tr("Telefono2");
		case KEmail1: 
			return tr("Email1");
		case KEmail2: 
			return tr("Email2");
		default: 
			qWarning("Error: PersonaModel::headerData()");
		}
	}
	return QVariant();
}

Qt::ItemFlags PersonaModel::flags(const QModelIndex &index) const
{
	Qt::ItemFlags flags = QAbstractTableModel::flags(index);
	return flags;
}

int PersonaModel::rowCount(const QModelIndex &) const
{
	return m_personaList->count();
}

int PersonaModel::columnCount(const QModelIndex &) const
{
	return 11;
}

bool PersonaModel::insertRows(int row, int count, const QModelIndex &parent)
{
	beginInsertRows(parent, row, row + count -1);
	endInsertRows();
	return true;
}

bool PersonaModel::removeRows(int row, int count, const QModelIndex &parent)
{
	beginRemoveRows(parent, row, row + count -1);
	m_personaList->removeAt(row);
	endRemoveRows();
	return true;
}

bool PersonaModel::addPersona(Persona *obj)
{
	m_personaList->append(obj);
	return insertRows(m_personaList->count() -1, 1);
}

} // namespace crud
