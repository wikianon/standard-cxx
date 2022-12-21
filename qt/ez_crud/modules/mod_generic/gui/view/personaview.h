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

#ifndef PERSONAVIEW_H
#define PERSONAVIEW_H

#include <global/ezcrud_global.h>
#include <gui/entityview.h>

namespace Ui {
    class PersonaView;
}

namespace crud{

class CategoriaCheckModel;

class EZCRUD_EXPORT PersonaView : public EntityView
{
    Q_OBJECT
    Q_PROPERTY(bool readOnly
               READ readOnly
               WRITE setReadOnly)
    Q_PROPERTY(QVariant tipopersona
               READ tipopersona
               WRITE setTipopersona)
public:
    explicit PersonaView(QWidget *parent = 0);
    ~PersonaView();
    CategoriaCheckModel *categoriaCheckModel();
    void setState(State state);
    void setModel(ObjectBase *model);

    bool readOnly();
    void setReadOnly(bool);

    QVariant tipopersona();
    void setTipopersona(QVariant value);
private:
    Ui::PersonaView *ui;
    CategoriaCheckModel *m_categoriaCheckModel;
};

} // namespace crud

#endif // PERSONAVIEW_H
