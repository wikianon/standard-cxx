#ifndef CATASTROCATEGORIA_H
#define CATASTROCATEGORIA_H

#include <forms/basecrud.h>
#include <core/crudnamespace.h>

class CatastroCategoria : public BaseCRUD
{
    Q_OBJECT
public:
    CatastroCategoria(crud::EasyCrud::CRUDFlags &crudFlags, QWidget *parent = 0);
protected:
    bool insert(OrmClass *registro);
    bool save(OrmClass *registro);
    OrmClass * getRegistro(qlonglong codigo = 0);
    EntityView * getWidget();
    QString formName();
    QString windowTitle();
};

#endif // CATASTROCATEGORIA_H
