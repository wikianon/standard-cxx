#ifndef CRUD_ORGANIZATIONCRUD_H
#define CRUD_ORGANIZATIONCRUD_H

#include <gui/basecrud.h>
#include <data/object/module.h>

namespace crud {

class OrganizationCrud : public BaseCRUD
{
    Q_OBJECT
public:
    OrganizationCrud(crud::EasyCrud::CRUDFlags &crudFlags, QWidget *parent = 0);
    bool insert(crud::ObjectBase *registro);
    bool save(crud::ObjectBase *registro);
    crud::ObjectBase * getRegistro(qlonglong id = 0);
    EntityView * getWidget();
    QString formName();
    QString windowTitle();

protected:
    QSqlDatabase *getDatabase();
};

} // namespace crud

#endif // CRUD_ORGANIZATIONCRUD_H
