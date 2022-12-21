#ifndef CRUD_CATEGORIACRUD_H
#define CRUD_CATEGORIACRUD_H

#include <gui/basecrud.h>
#include <data/object/module.h>

namespace crud {

class EZCRUD_EXPORT CategoriaCrud : public BaseCRUD
{
    Q_OBJECT
public:
    CategoriaCrud(crud::EasyCrud::CRUDFlags &crudFlags, QWidget *parent = 0);
    bool insert(crud::ObjectBase *obj);
    bool save(crud::ObjectBase *obj);
    crud::ObjectBase * getRegistro(qlonglong id = 0);
    EntityView * getWidget();
    QString formName();
    QString windowTitle();

protected:
    QSqlDatabase *getDatabase();
};

} // namespace crud

#endif // CRUD_CATEGORIACRUD_H
