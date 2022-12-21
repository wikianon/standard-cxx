#ifndef CRUD_ORGANIZATIONVIEW_H
#define CRUD_ORGANIZATIONVIEW_H

#include <gui/entityview.h>

namespace crud {

namespace Ui {
class OrganizationView;
}

class OrganizationView : public EntityView
{
    Q_OBJECT
    
public:
    explicit OrganizationView(QWidget *parent = 0);
    ~OrganizationView();
    void setState(State state);
    
private:
    Ui::OrganizationView *ui;
};


} // namespace crud
#endif // CRUD_ORGANIZATIONVIEW_H
