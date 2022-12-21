#include "categoriaitem.h"


namespace crud {

CategoriaItem::CategoriaItem(CategoriaItem *parent) :
    m_parent(parent),
    m_categoria(0),
    m_checked(false)
{
    if(m_parent)
        m_parent->m_children.append(this);
}

CategoriaItem::~CategoriaItem()
{
    qDeleteAll(m_children);
}

int CategoriaItem::row() const
{
    if (m_parent)
         return m_parent->m_children.indexOf(const_cast<CategoriaItem*>(this));
    return 0;
}

int CategoriaItem::rowOfChild(CategoriaItem *child) const
{
    return m_children.indexOf(child);
}

int CategoriaItem::childCount()
{
    return m_children.count();
}

CategoriaItem *CategoriaItem::parent()
{
    return m_parent;
}

CategoriaItem *CategoriaItem::childAt(int index)
{
    return m_children.value(index);
}

CategoriaItem *CategoriaItem::childAt(int index) const
{
    return m_children.value(index);
}

QList<CategoriaItem *> &CategoriaItem::children()
{
    return m_children;
}

Categoria *CategoriaItem::categoria() const
{
    return m_categoria;
}

void CategoriaItem::setCategoria(Categoria *categoria)
{
    m_categoria = categoria;
}

bool CategoriaItem::checked() const
{
    return m_checked;
}

void CategoriaItem::setChecked(bool b)
{
    m_checked = b;
}

} // namespace crud
