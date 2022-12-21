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

#include "navigatorbar.h"

#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QAction>
#include <QToolBar>
#include <QToolButton>
#include <QHBoxLayout>

NavigatorBar::NavigatorBar(QWidget *parent) :
    QWidget(parent)
{
    createAndConnect();
}

void NavigatorBar::setCRUDFlags(crud::EasyCrud::CRUDFlags type)
{
    if(m_crudFlags == type)
        return;
    else
        m_crudFlags = type;
}

crud::EasyCrud::CRUDFlags NavigatorBar::crudFlags() const
{
    return m_crudFlags;
}

NavigatorBar::~NavigatorBar()
{

}


void NavigatorBar::setState(crud::State state)
{
    m_state = state;

    if (isVisible()){
        QList<QToolButton *> buttonList = qFindChildren<QToolButton *>(this);
        QMap<QToolButton *, QPointF > positionList;

        foreach(QToolButton *button, buttonList){
            positionList.insert(button, button->pos());
        }

        QParallelAnimationGroup *animationGroup = new QParallelAnimationGroup;
        foreach(QToolButton *button, buttonList){
            QPropertyAnimation *animation = new QPropertyAnimation(button, "pos", animationGroup);
            animation->setEasingCurve(QEasingCurve::InOutBack);
            QPointF startValue;
            int duration(800);
            switch(m_state){
            case crud::KWatching:
                // arriba -> abajo
                startValue = QPointF(QPointF(positionList.value(button)).x(), -1 * (size().height() + button->pos().y()));
                break;
            case crud::KInserting:
                // derecha -> izquierda
                duration = (parentWidget()->size().width() / 640.0) * duration;
                startValue = QPointF(qreal(size().width())
                                     +QPointF(positionList.value(button)).x(), qreal(button->pos().y()));
                break;
            case crud::KUpdating:
                // abajo -> arriba
                startValue = QPointF(QPointF(positionList.value(button)).x(), size().height() + button->pos().y());
                break;
            }
            animation->setStartValue(startValue);
            animation->setEndValue(QPointF(positionList.value(button)));
            animation->setDuration(duration);
            animationGroup->addAnimation(animation);
        }
        animationGroup->start(QAbstractAnimation::DeleteWhenStopped);
    }


    switch(m_state){
    case crud::KWatching:
        m_newAction->setEnabled(m_crudFlags.testFlag(crud::EasyCrud::Insert) && true);
        m_editAction->setEnabled(m_crudFlags.testFlag(crud::EasyCrud::Update) && true);
        m_deleteAction->setEnabled(m_crudFlags.testFlag(crud::EasyCrud::Delete) && true);
        m_saveAction->setEnabled(false);
        m_cancelAction->setEnabled(false);
        m_printAction->setEnabled(m_crudFlags.testFlag(crud::EasyCrud::Watch) && true);
        break;
    case crud::KInserting:
        m_newAction->setEnabled(false);
        m_editAction->setEnabled(false);
        m_deleteAction->setEnabled(false);
        m_saveAction->setEnabled(true);
        m_cancelAction->setEnabled(true);
        m_printAction->setEnabled(false);
        break;
    case crud::KUpdating:
        m_newAction->setEnabled(false);
        m_editAction->setEnabled(false);
        m_deleteAction->setEnabled(false);
        m_saveAction->setEnabled(true);
        m_cancelAction->setEnabled(true);
        m_printAction->setEnabled(false);
        break;
    }
}

crud::State NavigatorBar::state()
{
    return m_state;
}

void NavigatorBar::createAndConnect()
{
    QString resourcePath(":/ezcrud/images");
    //Create
    m_newAction = new QAction(tr("&Nuevo"), this);
    m_newAction->setObjectName("NewAction");
    m_newAction->setShortcut(tr("ALT+F3"));
    m_newAction->setIcon(QIcon(resourcePath + QLatin1String("/crud/create.png")));

    //Update
    m_editAction = new QAction(tr("&Editar"), this);
    m_editAction->setObjectName("EditAction");
    m_editAction->setShortcut(tr("ALT+F4"));
    m_editAction->setIcon(QIcon(resourcePath + QLatin1String("/crud/update.png")));

    //Delete
    m_deleteAction = new QAction(tr("&Anular"), this);
    m_deleteAction->setObjectName("DeleteAction");
    m_deleteAction->setShortcut(tr("ALT+F5"));
    m_deleteAction->setIcon(QIcon(resourcePath + QLatin1String("/crud/delete.png")));

    //Save
    m_saveAction = new QAction(tr("&Guardar"), this);
    m_saveAction->setObjectName("SaveAction");
    m_saveAction->setShortcut(tr("ALT+F6"));
    m_saveAction->setIcon(QIcon(resourcePath + QLatin1String("/crud/save.png")));

    //Cancel
    m_cancelAction = new QAction(tr("&Cancelar"), this);
    m_cancelAction->setObjectName("CancelAction");
    m_cancelAction->setShortcut(tr("ALT+F7"));
    m_cancelAction->setIcon(QIcon(resourcePath + QLatin1String("/crud/cancel.png")));

    //Print
    m_printAction = new QAction(tr("&Imprimir"), this);
    m_printAction->setObjectName("PrintAction");
    m_printAction->setShortcut(tr("ALT+F8"));
    m_printAction->setIcon(QIcon(resourcePath + QLatin1String("/crud/print.png")));

    QToolBar *navigationBar = new QToolBar(tr("Navigation Bar"),this);
    navigationBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    navigationBar->setObjectName(QLatin1String("NavigationToolBar"));
    navigationBar->addAction(m_newAction);
    navigationBar->addAction(m_editAction);
    navigationBar->addAction(m_deleteAction);
    navigationBar->addAction(m_saveAction);
    navigationBar->addAction(m_cancelAction);
    navigationBar->addAction(m_printAction);

    QHBoxLayout *horizontalLayout = new QHBoxLayout(this);
    horizontalLayout->addWidget(navigationBar);
    setLayout(horizontalLayout);


    connect(m_newAction, SIGNAL(triggered()), SIGNAL(newButtonClicked()));
    connect(m_editAction, SIGNAL(triggered()), SIGNAL(editButtonClicked()));
    connect(m_deleteAction, SIGNAL(triggered()), SIGNAL(deleteButtonClicked()));
    connect(m_saveAction, SIGNAL(triggered()), SIGNAL(saveButtonClicked()));
    connect(m_cancelAction, SIGNAL(triggered()), SIGNAL(cancelButtonClicked()));
    connect(m_printAction, SIGNAL(triggered()), SIGNAL(printButtonClicked()));
}
