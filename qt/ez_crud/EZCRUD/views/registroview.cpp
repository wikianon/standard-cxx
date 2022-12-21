#include "registroview.h"
#include "ui_registroview.h"

#include "orm/ormclass.h"
#include "dyn/dynamicitem.h"
#include "dyn/dynamicwidget.h"


RegistroView::RegistroView(crud::State state, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegistroView),
    m_state(state),
    m_ormClass(),
    dynamicWidgets()
{
    ui->setupUi(this);
    ui->idValue->setReadOnly(true);
    ui->situacion->setReadOnly(true);
    ui->dateOfInsertion->setReadOnly(true);
    ui->lastUpdate->setReadOnly(true);
}

RegistroView::~RegistroView()
{
    delete ui;
}


OrmClass *RegistroView::model()
{
    return m_ormClass;
}

void RegistroView::setModel(OrmClass *model)
{
    m_ormClass = model;
}

void RegistroView::onSaveButtonClicked()
{
    switch(state())
    {
    case INSERTING:
    case UPDATING:
        if(validate())
            emit viewValidated();
        break;
    default:
        break;
    }
}

void RegistroView::setModelValues()
{
    foreach(DynamicWidget *dynamicWidget, dynamicWidgets)
    {
        m_ormClass->setProperty(dynamicWidget->widget()->objectName().toLocal8Bit(), dynamicWidget->value());
    }
}

void RegistroView::setViewValues()
{
    foreach(DynamicWidget *dynamicWidget, dynamicWidgets)
    {
        dynamicWidget->setValue(m_ormClass->property(dynamicWidget->widget()->objectName().toLocal8Bit().data()));
    }
}

void RegistroView::setMainForm(QWidget * form)
{
    ui->verticalLayout_WidgetView->addWidget(form);
    form->setParent(ui->widgetView);
}

void RegistroView::setState(crud::State state)
{
    if(m_state != state){
        m_state = state;
        switch(m_state){
        case WATCHING:
            setViewValues();
            ui->widgetView->setEnabled(false);
            break;
        case INSERTING:
        case UPDATING:
            ui->widgetView->setEnabled(true);
            break;
        }
    }
}
