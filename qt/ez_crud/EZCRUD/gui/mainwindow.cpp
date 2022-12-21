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

#include <QCloseEvent>
#include <QMenu>
#include <QAction>
#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "../core/settings.h"
#include "../core/ezapplication.h"

#include "../gui/mdiarea.h"

#include "../data/dao/moduledao.h"
#include "../data/dao/widgetdao.h"

namespace crud {

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_menuList(),
    m_menuRoot(0),
    m_menuAyuda(0)
{
    ui->setupUi(this);
    ui->mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setAttribute(Qt::WA_DeleteOnClose, true);
    setWindowTitle(EZApplication::instance()->applicationName());
}

MainWindow::~MainWindow()
{
    delete ui;
}

MdiArea *MainWindow::mdiArea()
{
    return ui->mdiArea;
}

QMenuBar *MainWindow::menuBar()
{
    return ui->menubar;
}

QMenu *MainWindow::menuAyuda()
{
    return m_menuAyuda;
}

void MainWindow::setMenuAyuda(QMenu *menu)
{
    m_menuAyuda = menu;
}

QStatusBar *MainWindow::statusBar()
{
    return ui->statusbar;
}

QList<QMenu *> & MainWindow::menuList()
{
    return m_menuList;
}

QMenu *MainWindow::menuRoot()
{
    return m_menuRoot;
}

bool MainWindow::confirmLogout()
{
    return true;
}

void MainWindow::logout()
{
    //deleteAll(ui->mdiArea->children());
    // TODO: salvar las ventanas, preguntar las que no estan guardadas, etc.
}


void MainWindow::closeEvent(QCloseEvent *event)
 {
     if (confirmLogout()) {
         Settings::qSettings()->setValue("MainWindow/geometry", saveGeometry());
         Settings::qSettings()->setValue("MainWindow/windowState", saveState());
         logout();
         emit loginOut();
         event->accept();
     } else {
         event->ignore();
     }
}

void MainWindow::show()
{
    loadMenus();
    QWidget::show();
}

void MainWindow::readSettings()
{
    restoreGeometry(Settings::qSettings()->value("MainWindow/geometry").toByteArray());
    restoreState(Settings::qSettings()->value("MainWindow/windowState").toByteArray());
}

void MainWindow::loadMenus()
{
    foreach(QMenu *menu, m_menuList)
    {
        ui->menubar->addMenu(menu);
    }
    if(m_menuRoot)
        ui->menubar->addMenu(m_menuRoot);
    // create Menu Ayuda
    m_menuAyuda = new QMenu(tr("Ayuda"), this);
    QAction *aboutQt = new QAction(tr("About Qt"), m_menuAyuda);
    QAction *aboutEZCRUD = new QAction(tr("About Easy CRUD"), m_menuAyuda);
    connect(aboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(aboutEZCRUD, SIGNAL(triggered()), SLOT(aboutEZCRUD()));
    m_menuAyuda->addAction(aboutQt);
    m_menuAyuda->addAction(aboutEZCRUD);
    ui->menubar->addMenu(m_menuAyuda);
}

void MainWindow::setMenuRoot(QMenu *menu)
{
    m_menuRoot = menu;
}

void MainWindow::aboutEZCRUD()
{
    QString header(QString("<h3>About Easy CRUD</h3>"
                              "<p>This program uses Qt version %1.</p>").arg(QLatin1String(QT_VERSION_STR)));
    QString text(QString(
                "<p>Easy CRUD is an application arquitecture that can be extended through external modules (plugins) using Qt. "
                "This arquitecture is designed to provide the basic tools to create CRUD forms, Configuration forms and Report forms. "
                "Also it includes the root module (mod_root), its function is to allow the ROOT user to control the access privileges of the "
                "system's user, the ROOT user can limit the normal users privileges, this is useful for systems that requires security levels.</p>"
                "<p>Project home site: <a href=\"http://sourceforge.net/projects/easy-crud/\">sourceforge.net/projects/easy-crud</a></p>"
                "<p>Copyright (C) 2010, 2011, 2012 <br> Luis Valdes (<a href=\"luisvaldes88@gmail.com/\">luisvaldes88@gmail.com</a>)</p>"
                ));
    QMessageBox *messageBox = new QMessageBox(this);
    messageBox->setAttribute(Qt::WA_DeleteOnClose);
    messageBox->setWindowTitle("About Easy CRUD");
    messageBox->setText(header);
    messageBox->setInformativeText(text);
    messageBox->setDefaultButton(messageBox->addButton(QMessageBox::Ok));
    messageBox->show();
}

} // namespace crud
