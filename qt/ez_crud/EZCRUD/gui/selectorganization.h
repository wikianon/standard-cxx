#ifndef SELECTORGANIZATION_H
#define SELECTORGANIZATION_H

#include <QDialog>

#include "../global/ezcrud_global.h"

namespace Ui {
class SelectOrganization;
}

class EZCRUD_EXPORT SelectOrganization : public QDialog
{
    Q_OBJECT


public:
    static qlonglong selectOrg();
    ~SelectOrganization();

protected:
    explicit SelectOrganization(QWidget *parent = 0);

private:
    Ui::SelectOrganization *ui;
    qlonglong m_result;
private slots:
    void onAccepted();
};

#endif // SELECTORGANIZATION_H
