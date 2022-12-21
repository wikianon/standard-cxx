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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "../global/ezcrud_global.h"

#include "../data/object/module.h"
#include "../data/object/widget.h"

namespace Ui {
class MainWindow;
}

class QCloseEvent;
class QMenuBar;
class QMenu;
class QStatusBar;
class MdiArea;

namespace crud {

class  EZCRUD_EXPORT MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    MdiArea * mdiArea();
    QMenuBar * menuBar();
    QStatusBar * statusBar();

    QList<QMenu *> & menuList();
    QMenu * menuRoot();
    void setMenuRoot(QMenu *menu);
    QMenu * menuAyuda();
    void setMenuAyuda(QMenu *menu);

    bool confirmLogout();
    void logout();
    void closeEvent(QCloseEvent *);
    void show();

    void readSettings();
private:
    ::Ui::MainWindow *ui;
    QList<QMenu *> m_menuList;
    QMenu *m_menuRoot;
    QMenu *m_menuAyuda;

private:
    void loadMenus();

signals:
    void loginOut();

private slots:
    void aboutEZCRUD();

};

} // namespace crud

#endif // MAINWINDOW_H
