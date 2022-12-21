#ifndef CATASTROLOCAL_H
#define CATASTROLOCAL_H

#include "catastrobase.h"

class CatastroLocal : public CatastroBase
{
    Q_OBJECT
public:
    CatastroLocal(QWidget *parent=0);
protected:
    QSqlError insert(OrmClass *registro);
    QSqlError save(OrmClass *registro);
    OrmClass * getRegistro(qlonglong codigo = 0);
    RegistroView * getWidget();
    QString formName();
    QString windowTitle();
};

#endif // CATASTROLOCAL_H
