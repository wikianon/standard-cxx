#include <QSqlDatabase>

#include <core/session.h>

#include "organizationview.h"
#include "ui_organizationview.h"

namespace crud {

OrganizationView::OrganizationView(QWidget *parent) :
    EntityView(parent),
    ui(new Ui::OrganizationView)
{
    ui->setupUi(widgetView());
}

OrganizationView::~OrganizationView()
{
    delete ui;
}

void OrganizationView::setState(State state)
{
    EntityView::setState(state);
    if(Session::instance()->systemDatabase()->driverName() == "QSQLITE"){
        ui->hostAddress->setEnabled(false);
        ui->hostDatabase->setEnabled(false);
        ui->hostPassword->setEnabled(false);
        ui->hostUser->setEnabled(false);
    }
}

} // namespace crud
