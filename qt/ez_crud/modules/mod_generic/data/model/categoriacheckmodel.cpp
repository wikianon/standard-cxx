#include "categoriacheckmodel.h"
#include "categoriaitem.h"
#include "../object/categoria.h"
#include "../object/persona.h"
#include "../object/persona_categoria.h"
#include "../dao/categoriadao.h"

namespace crud {

CategoriaCheckModel::CategoriaCheckModel(QObject *parent) :
    QAbstractTableModel(parent),
    m_rootItem(new CategoriaItem),
    m_persona(0)
{

}

CategoriaCheckModel::~CategoriaCheckModel()
{
    delete m_rootItem;
}

QVariant CategoriaCheckModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();
    CategoriaItem *obj = m_rootItem->childAt(index.row());
    if((role == Qt::DisplayRole) || (role == Qt::EditRole)){
        switch(index.column()){
        case KId:
            return obj->categoria()->id();
        case KNombre:
            return obj->categoria()->nombre();
        default:
            qWarning("Error: CategoriaCheckModel::data()");
        }
    }else if((role == Qt::CheckStateRole) && (index.column() == KId)){
        return obj->checked();
    }
    return QVariant();
}

bool CategoriaCheckModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if((role == Qt::CheckStateRole) && (index.column() == KId)){
        if(CategoriaItem *item = itemForIndex(index)){
            item->setChecked(!item->checked());
            emit dataChanged(index, index);
        }
    }
    return false;
}

QVariant CategoriaCheckModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant();
    if(orientation == Qt::Horizontal){
        switch(section){
        case KId:
            return tr("Id");
        case KNombre:
            return tr("Nombre");
        default:
            qWarning("Error: CategoriaCheckModel::headerData()");
        }
    }
    return QVariant();
}

Qt::ItemFlags CategoriaCheckModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags (Qt::NoItemFlags);
    if(index.isValid()){
        switch(index.column()){
        case KId:{
            if(!m_readOnly)
                flags  |= Qt::ItemFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
            else
                flags  |= Qt::ItemFlags(Qt::ItemIsEnabled);
            break;
        }
        case KNombre:{
            flags  |= Qt::ItemFlags(Qt::ItemIsEnabled);
            break;
        }
        }
    }
    return flags;
}

int CategoriaCheckModel::rowCount(const QModelIndex &) const
{
    return m_rootItem->childCount();
}

int CategoriaCheckModel::columnCount(const QModelIndex &) const
{
    return 2;
}

QModelIndex CategoriaCheckModel::index(int row, int column, const QModelIndex &parent) const
{
    if(row < 0 || column < 0 || column >= ColumnCount || (parent.isValid() && parent.column() != 0))
        return QModelIndex();
    CategoriaItem *parentItem = itemForIndex(parent);
    Q_ASSERT(parentItem);
    if(CategoriaItem *item = parentItem->childAt(row))
        return createIndex(row, column, item);
    return QModelIndex();
}

QModelIndex CategoriaCheckModel::parent(const QModelIndex &index) const
{
    if(!index.isValid())
        return QModelIndex();
    if(CategoriaItem *childItem = itemForIndex(index)){
        if(CategoriaItem *parentItem = childItem->parent()){
            if(parentItem == m_rootItem)
                return QModelIndex();
            if(CategoriaItem *grandParentItem = parentItem->parent()){
                int row = grandParentItem->rowOfChild(parentItem);
                return createIndex(row, 0, parentItem);
            }
        }
    }
    return QModelIndex();
}

CategoriaItem *CategoriaCheckModel::itemForIndex(const QModelIndex &index) const
{
    if(index.isValid()){
        if(CategoriaItem *item = static_cast<CategoriaItem *>(index.internalPointer()))
            return item;
    }
    return m_rootItem;
}

QVariant CategoriaCheckModel::readOnly()
{
    return m_readOnly;
}

void CategoriaCheckModel::setReadOnly(QVariant readOnly)
{
    m_readOnly = readOnly.toBool();
}

QList<PersonaCategoria *> CategoriaCheckModel::getPersonaCategoriaList(QObject *parent)
{
    QList<PersonaCategoria *> list;
    foreach(CategoriaItem *item, m_rootItem->children()){
        if(item->checked()){
            PersonaCategoria *perCat = new PersonaCategoria(PARENT(parent));
            perCat->setIdpersona(m_persona->id());
            perCat->setIdcategoria(item->categoria()->id());
            list.append(perCat);
        }
    }
    return list;
}

void CategoriaCheckModel::setPersona(Persona *persona)
{
    m_persona = persona;
    QList<Categoria *> categoriaList = CategoriaDAO::instance()->getAll(this);
    foreach(Categoria *cat, categoriaList){
        CategoriaItem *item = new CategoriaItem(m_rootItem);
        item->setCategoria(cat);
        foreach(Categoria *catPersona, m_persona->categoriaList()){
            if(catPersona->id().toLongLong() == cat->id().toLongLong()){
                item->setChecked(true);
                break;
            }
        }
    }
}

void CategoriaCheckModel::selectAll()
{
    foreach(CategoriaItem *item, m_rootItem->children()){
        item->setChecked(true);
    }
    emit dataChanged(QModelIndex(), QModelIndex());
}

void CategoriaCheckModel::selectNone()
{
    foreach(CategoriaItem *item, m_rootItem->children()){
        item->setChecked(false);
    }
    emit dataChanged(QModelIndex(), QModelIndex());
}

} // namespace crud
