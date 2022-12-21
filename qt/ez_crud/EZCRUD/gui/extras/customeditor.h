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

#ifndef CUSTOMEDITOR_H
#define CUSTOMEDITOR_H

#include <QWidget>
#include <QVariant>

#include "global/ezcrud_global.h"

class QSqlQueryModel;
class QComboBox;
class QKeyEvent;

namespace Ui {
    class CustomEditor;
}

class EZCRUD_EXPORT CustomEditor : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QVariant value
               READ value
               WRITE setValue)
    Q_PROPERTY(bool readOnly
               READ isReadOnly
               WRITE setReadOnly)
public:
    explicit CustomEditor(QWidget *parent = 0);
    ~CustomEditor();
    QVariant value();
    QString descripcion();
    bool isReadOnly() const;
    virtual void setReadOnly(bool readOnly);
protected:
    QComboBox * descripcionComboBox();
    /*capture F10*/
    void keyPressEvent(QKeyEvent *keyEvent);
public slots:
    void setValue(QVariant value);
signals:
    void valueChanged(qlonglong);
private slots:
    void on_m_descripcion_currentIndexChanged(int index);
    void on_m_codigo_textEdited(const QString &arg1);
private:
    Ui::CustomEditor *ui;
};

#endif // CUSTOMEDITOR_H
