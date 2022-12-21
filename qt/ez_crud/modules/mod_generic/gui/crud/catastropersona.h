#ifndef CATASTROPERSONA_H
#define CATASTROPERSONA_H

#include <forms/basecrud.h>
#include <core/crudnamespace.h>

class EZCRUD_EXPORT CatastroPersona : public BaseCRUD
{
    Q_OBJECT
public:
    CatastroPersona(crud::EasyCrud::CRUDFlags &crudFlags, QWidget *parent = 0);
protected:
    bool insert(OrmClass *registro);
    bool save(OrmClass *registro);
    OrmClass * getRegistro(qlonglong codigo = 0);
    EntityView * getWidget();
    QString formName();
    QString windowTitle();
};

#endif // CATASTROPERSONA_H
