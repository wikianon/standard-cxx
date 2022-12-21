/****************************************************************************
**
** http://sourceforge.net/projects/easy-crud/
**
** Copyright (C) 2010, 2011, 2012  Luis Valdes (luisvaldes88@gmail.com)
**
** This file is part of the EZCRUD library
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**
****************************************************************************/

#include "revision.h"
#include "user.h"
#include "module.h"
#include "widget.h"
#include "module_revision.h"
#include "widget_revision.h"

#include "../dao/revisiondao.h"
#include "../dao/module_revisiondao.h"
#include "../dao/widget_revisiondao.h"

namespace crud
{

Revision::Revision(QObject *parent) :
    ObjectBase(parent),
    m_idUser(),
    m_idOrganization(),
    m_dateTime(),
    m_revisionModule(),
    m_revisionWidget(),
    m_modulesMap()
{

}

Revision::Revision(const QSqlRecord &record, QObject *parent):
    ObjectBase(parent),
    m_idUser(),
    m_idOrganization(),
    m_dateTime(),
    m_revisionModule(),
    m_revisionWidget(),
    m_modulesMap()
{
    //id, idusuario, idempresa, datetime
    setId(record.value(0));
    setIdUser(record.value(1));
    setIdOrganization(record.value(2));
    setDateTime(record.value(3));
}

QVariant Revision::idUser() const
{
    return m_idUser;
}

void Revision::setIdUser(QVariant idUsuario)
{
    m_idUser = idUsuario.toLongLong();
}

QVariant Revision::idOrganization() const
{
    return m_idOrganization;
}

void Revision::setIdOrganization(QVariant idEmpresa)
{
    m_idOrganization = idEmpresa.toLongLong();
}


QList< ModuleRevision * > & Revision::moduleList()
{
    if((m_id != -1) && (m_revisionModule.count() == 0)){
        m_revisionModule = ModuleRevisionDAO::instance()->getByRevisionId(m_id, this);
    }
    return m_revisionModule;
}

QList<WidgetRevision  * > & Revision::widgetList()
{
    if((m_id != -1) && (m_revisionWidget.count() == 0)){
        m_revisionWidget = WidgetRevisionDAO::instance()->getByRevisionId(m_id, this);
    }
    return m_revisionWidget;
}

QMap<QString, QMap<QString, int> > Revision::modulesMap()
{
    foreach(ModuleRevision *revisionModule, m_revisionModule){
        Module *module = revisionModule->module();
        QMap<QString, int> moduleWidgets;
        foreach(WidgetRevision *revisionWidget, m_revisionWidget){
            Widget *widget = revisionWidget->widget();
            if(widget->idModule() == module->id()){
                moduleWidgets.insert(widget->widgetName().toString(), revisionWidget->accessMode().toUInt());
            }
        }
        if(moduleWidgets.count() > 0)
            m_modulesMap.insert(module->moduleName().toString(), moduleWidgets);
    }
    return m_modulesMap;
}

QVariant Revision::dateTime() const
{
    return m_dateTime;
}

void Revision::setDateTime(QVariant dateTime)
{
    m_dateTime = dateTime.toDateTime();
}

} // namespace crud
