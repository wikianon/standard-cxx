#ifndef CRUD_PERSONACRUD_H
#define CRUD_PERSONACRUD_H

#include <gui/basecrud.h>
#include <data/object/module.h>

namespace crud {

class EZCRUD_EXPORT PersonaCrud : public BaseCRUD
{
    Q_OBJECT
public:
    explicit PersonaCrud(crud::EasyCrud::CRUDFlags &crudFlags, QWidget *parent = 0);
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

#endif // CRUD_PERSONACRUD_H
