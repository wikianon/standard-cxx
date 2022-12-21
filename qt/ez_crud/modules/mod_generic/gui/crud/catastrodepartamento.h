#ifndef CATASTRODEPARTAMENTO_H
#define CATASTRODEPARTAMENTO_H

#include "catastrobase.h"

class CatastroDepartamento : public CatastroBase
{
    Q_OBJECT
public:
    CatastroDepartamento(QWidget *parent=0);
protected:
    QSqlError insert(OrmClass *registro);
    QSqlError save(OrmClass *registro);
    OrmClass * getRegistro(qlonglong codigo = 0);
    RegistroView * getWidget();
    QString formName();
    QString windowTitle();
};

#endif // CATASTRODEPARTAMENTO_H
