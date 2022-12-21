#ifndef CATASTRODISTRITO_H
#define CATASTRODISTRITO_H

#include "catastrobase.h"

class CatastroDistrito : public CatastroBase
{
    Q_OBJECT
public:
    CatastroDistrito(QWidget *parent=0);
protected:
    QSqlError insert(OrmClass *registro);
    QSqlError save(OrmClass *registro);
    OrmClass * getRegistro(qlonglong codigo = 0);
    RegistroView * getWidget();
    QString formName();
    QString windowTitle();
};

#endif // CATASTRODISTRITO_H
