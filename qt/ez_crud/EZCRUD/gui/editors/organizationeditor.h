// ****************************************************
// * Codigo Generado por DAOBuilder                   *
// * Fecha: 23/10/2012 07:48:24                       *
// * Luis Valdes 2012 - Todos los Derechos Reservados *
// ****************************************************
#ifndef ORGANIZATIONEDITOR_H
#define ORGANIZATIONEDITOR_H

#include "../extras/customeditor.h"

namespace crud {

class OrganizationModel;

class EZCRUD_EXPORT OrganizationEditor : public CustomEditor
{
	Q_OBJECT
public:
// Constructors
	explicit OrganizationEditor(QWidget *parent = 0);
    explicit OrganizationEditor(OrganizationModel *model, QWidget *parent = 0);

    void setModel(OrganizationModel *model);

    void setReadOnly(bool readOnly);
    void setNotReadOnly(bool b);
private:
    bool m_notReadOnly;
};

} // namespace crud

#endif // ORGANIZATIONEDITOR_H
