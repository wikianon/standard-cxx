// ****************************************************
// * Codigo Generado por DAOBuilder                   *
// * Fecha: 23/10/2012 17:40:10                       *
// * Luis Valdes 2012 - Todos los Derechos Reservados *
// ****************************************************
#ifndef WIDGET_REVISION_H
#define WIDGET_REVISION_H

#include <QSqlRecord>
#include <data/objectbase.h>

namespace crud {

// Objects Forward Declarations

class Revision;
class Widget;

class EZCRUD_EXPORT WidgetRevision : public QObject
{
	Q_OBJECT
    Q_PROPERTY(QVariant idRevision
               READ idRevision
               WRITE setIdRevision)
    Q_PROPERTY(QVariant idWidget
               READ idWidget
               WRITE setIdWidget)
    Q_PROPERTY(QVariant accessMode
               READ accessMode
               WRITE setAccessMode)

public:
// Constructors
    explicit WidgetRevision(QObject *parent = 0);
    explicit WidgetRevision(const QSqlRecord &, QObject *parent = 0);

// Getters & Setters
    QVariant idRevision() const;
    void setIdRevision(QVariant);
    QVariant idWidget() const;
    void setIdWidget(QVariant);
    QVariant accessMode() const;
    void setAccessMode(QVariant);

// Objects Getters & Setters
	Revision * revision();
    void setRevision(Revision *obj);
    Widget * widget();
    void setWidget(Widget *obj);

private:
// Private Members
	qlonglong m_idrevision;
	qlonglong m_idwidget;
    quint32 m_accessmode;

private:
// Private Object Members
	Revision *m_revision;
    Widget *m_widget;
};

} // namespace crud

#endif // WIDGET_REVISION_H
