#ifndef CATEGORIAITEM_H
#define CATEGORIAITEM_H

#include <global/ezcrud_global.h>

namespace crud {

class Categoria;

class EZCRUD_EXPORT CategoriaItem
{
public:
    CategoriaItem(CategoriaItem *parent=0);
    virtual ~CategoriaItem();

    int row() const;
    int rowOfChild(CategoriaItem *child) const;
    int childCount();

    CategoriaItem * parent();
    CategoriaItem * childAt(int index);
    CategoriaItem * childAt(int index) const;
    QList<CategoriaItem*> & children();

    Categoria * categoria() const;
    void setCategoria(Categoria *categoria);

    bool checked() const;
    void setChecked(bool b);
private:
    CategoriaItem *m_parent;
    QList<CategoriaItem*> m_children;
    Categoria *m_categoria;
    bool m_checked;
};

} // namespace crud

#endif // CATEGORIAITEM_H
