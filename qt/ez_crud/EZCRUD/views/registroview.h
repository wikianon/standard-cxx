#ifndef REGISTROVIEW_H
#define REGISTROVIEW_H

#include <QWidget>
#include "global/knutcomp_global.h"
#include "controller/actionpanel.h"
class OrmClass;
class DynamicWidget;

namespace Ui {
    class RegistroView;
}

class KNUTCOMP_EXPORT RegistroView : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(crud::State state
               READ state
               WRITE setState)
public:
    explicit RegistroView(crud::State state = WATCHING, QWidget *parent = 0);
    ~RegistroView();

    void setModelValues();
    void setViewValues();
    void setMainForm(QWidget * form);

    void setState(crud::State state);
    crud::State state(){return m_state;}
    OrmClass *model();
    void setModel(OrmClass *model);

private:
    Ui::RegistroView *ui;
protected:
    //widget validation rutine
    virtual bool validate(){return true;}

    friend class Factory;
    crud::State m_state;
    OrmClass *m_ormClass;
    QList<DynamicWidget *> dynamicWidgets;
public slots:
    void onSaveButtonClicked();
signals:
    void viewValidated();
};

#endif // REGISTROVIEW_H
