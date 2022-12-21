// ****************************************************
// * Codigo Generado por DAOBuilder                   *
// * Fecha: 23/10/2012 17:40:10                       *
// * Luis Valdes 2012 - Todos los Derechos Reservados *
// ****************************************************
#include "widget_revision.h"
// Include Objects
#include "revision.h"
#include "widget.h"
// Include DAOs
#include "../dao/revisiondao.h"
#include "../dao/widgetdao.h"

namespace crud {

WidgetRevision::WidgetRevision(QObject *parent) :
    QObject(parent),
    m_idrevision(-1),
    m_idwidget(-1),
    m_accessmode(0),
    m_revision(0),
    m_widget(0)
{

}

WidgetRevision::WidgetRevision(const QSqlRecord &record, QObject *parent) :
    QObject(parent),
    m_idrevision(-1),
    m_idwidget(-1),
    m_accessmode(0),
    m_revision(0),
    m_widget(0)
{
	// idrevision, idwidget, accessmode
    setIdRevision(record.value(0));
    setIdWidget(record.value(1));
    setAccessMode(record.value(2));
}

// Getters y Setters Definitions
QVariant WidgetRevision::idRevision() const
{
	return m_idrevision;
}

void WidgetRevision::setIdRevision(QVariant value)
{
	m_idrevision = value.toLongLong();
}

QVariant WidgetRevision::idWidget() const
{
	return m_idwidget;
}

void WidgetRevision::setIdWidget(QVariant value)
{
	m_idwidget = value.toLongLong();
}

QVariant WidgetRevision::accessMode() const
{
	return m_accessmode;
}

void WidgetRevision::setAccessMode(QVariant value)
{
    m_accessmode = value.toUInt();
}

// Objects Getters y Setters Definitions
Revision * WidgetRevision::revision()
{
    if (! m_revision && (m_idrevision != -1)){
        m_revision = RevisionDAO::instance()->getById(m_idrevision, this);
	}
	return m_revision;
}

void WidgetRevision::setRevision(Revision *obj)
{
    m_revision = obj;
}

Widget *WidgetRevision::widget()
{
    if (! m_widget && (m_idwidget != -1)){
        m_widget = WidgetDAO::instance()->getById(m_idwidget, this);
    }
    return m_widget;
}

void WidgetRevision::setWidget(Widget *obj)
{
    m_widget = obj;
}

} // namespace crud
