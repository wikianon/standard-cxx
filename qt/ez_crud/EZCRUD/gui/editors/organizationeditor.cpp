// ****************************************************
// * Codigo Generado por DAOBuilder                   *
// * Fecha: 23/10/2012 07:48:24                       *
// * Luis Valdes 2012 - Todos los Derechos Reservados *
// ****************************************************
#include <QComboBox>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>

#include "../../core/session.h"

#include "organizationeditor.h"
#include "../../data/model/organizationmodel.h"

namespace crud {

OrganizationEditor::OrganizationEditor(QWidget *parent) :
    CustomEditor(parent),
    m_notReadOnly(false)
{
    OrganizationModel *model = new OrganizationModel(&Session::instance()->organizationList(), this);
    descripcionComboBox()->setModel(model);
    descripcionComboBox()->setModelColumn(1);
    descripcionComboBox()->setCurrentIndex(0);
}

OrganizationEditor::OrganizationEditor(OrganizationModel *model, QWidget *parent) :
    CustomEditor(parent),
    m_notReadOnly(false)
{
    setModel(model);
}

void OrganizationEditor::setModel(OrganizationModel *model)
{
    if(model){
        descripcionComboBox()->setModel(model);
        descripcionComboBox()->setModelColumn(1);
    }
}

void OrganizationEditor::setReadOnly(bool readOnly)
{
    if(!m_notReadOnly)
        CustomEditor::setReadOnly(readOnly);
}

void OrganizationEditor::setNotReadOnly(bool b)
{
    m_notReadOnly = b;
}

} // namespace crud
