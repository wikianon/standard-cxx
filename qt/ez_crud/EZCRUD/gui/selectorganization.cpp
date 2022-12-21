#include "selectorganization.h"
#include "ui_selectorganization.h"

#include  "../../core/session.h"

#include "editors/organizationeditor.h"

SelectOrganization::SelectOrganization(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectOrganization),
    m_result(-1)
{
    ui->setupUi(this);
    connect(ui->buttonBox, SIGNAL(accepted()), SLOT(onAccepted()));
    connect(ui->buttonBox, SIGNAL(rejected()), SLOT(close()));
}

SelectOrganization::~SelectOrganization()
{
    delete ui;
}

void SelectOrganization::onAccepted()
{
    m_result = ui->organizationEditor->value().toLongLong();
    accept();
}


qlonglong SelectOrganization::selectOrg()
{
    SelectOrganization s;
    s.exec();
    return s.m_result;
}
