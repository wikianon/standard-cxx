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

#include <QLineEdit>
#include <QTextEdit>
#include <QDateTimeEdit>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QWidget>
#include <QMetaObject>
#include <QVariant>

#include "gui/extras/customeditor.h"

#include "widgetbinder.h"

WidgetBinder::WidgetBinder(QWidget *widget, QObject *parent) :
    Binder(widget,parent)
{
    setObjectName(widget->objectName());
}

QVariant WidgetBinder::value() const
{
    QString className(m_widget->metaObject()->className());
    QString superClass("");
    const QMetaObject *metaSuperClass = m_widget->metaObject()->superClass();
    if(metaSuperClass)
        superClass = metaSuperClass->className();
    if(className == "QLineEdit")
    {
        QLineEdit *lineEdit = qobject_cast<QLineEdit *>(m_widget);
        return QVariant(lineEdit->text());
    }
    if(className == "QTextEdit")
    {
        QTextEdit *textEdit = qobject_cast<QTextEdit *>(m_widget);
        return QVariant(textEdit->toPlainText());
    }
    else if(className == "QDateTimeEdit")
    {
        QDateTimeEdit *dateTimeEdit = qobject_cast<QDateTimeEdit *>(m_widget);
        return QVariant(dateTimeEdit->dateTime());
    }
    else if(className == "QSpinBox")
    {
        QSpinBox *spinBox = qobject_cast<QSpinBox *>(m_widget);
        return QVariant(spinBox->value());
    }
    else if(className == "QDoubleSpinBox")
    {
        QDoubleSpinBox *doubleSpinBox = qobject_cast<QDoubleSpinBox *>(m_widget);
        return QVariant(doubleSpinBox->value());
    }
    else if((superClass == "CustomEditor") || (className == "CustomEditor"))
    {
        CustomEditor *customEditor = qobject_cast<CustomEditor *>(m_widget);
        if(customEditor)
            return customEditor->value();
    }
    return QVariant();
}

void WidgetBinder::setValue(QVariant value)
{
    QString className(m_widget->metaObject()->className());
    QString superClass("");
    const QMetaObject *metaSuperClass = m_widget->metaObject()->superClass();
    if(metaSuperClass)
        superClass = metaSuperClass->className();

    if(className == "QLineEdit")
    {
        QLineEdit *lineEdit = qobject_cast<QLineEdit *>(m_widget);
        lineEdit->setText(value.toString());
    }
    if(className == "QTextEdit")
    {
        QTextEdit *textEdit = qobject_cast<QTextEdit *>(m_widget);
        textEdit->setText(value.toString());
    }
    else if(className == "QDateTimeEdit")
    {
        QDateTimeEdit *dateTimeEdit = qobject_cast<QDateTimeEdit *>(m_widget);
        dateTimeEdit->setDateTime(value.toDateTime());
    }
    else if(className == "QSpinBox")
    {
        QSpinBox *spinBox = qobject_cast<QSpinBox *>(m_widget);
        spinBox->setValue(value.toInt());
    }
    else if(className == "QDoubleSpinBox")
    {
        QDoubleSpinBox *doubleSpinBox = qobject_cast<QDoubleSpinBox *>(m_widget);
        doubleSpinBox->setValue(value.toDouble());
    }
    else if((superClass == "CustomEditor") || (className == "CustomEditor"))
    {
        CustomEditor *customEditor = qobject_cast<CustomEditor *>(m_widget);
        customEditor->setValue(value.toLongLong());
    }
}
