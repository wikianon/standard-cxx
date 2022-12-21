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

#include <QSqlDatabase>
#include <QMessageBox>
#include <QDesktopWidget>

#include "logindialog.h"
#include "ui_logindialog.h"

#include "../data/dao/userdao.h"
#include "../data/object/user.h"

#include "../core/connectionpool.h"
#include "../core/settings.h"
#include "../core/encryption.h"
#include "../core/sql/sqlquery.h"
#include "../core/sql/exception/sqlexception.h"

#include "caseutil.h"

#include "selectserver.h"

namespace crud {

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new ::Ui::LoginDialog)
{
    ui->setupUi(this);
    connect(ui->buttonBox, SIGNAL(accepted()), SLOT(onAccepted()));
    connect(ui->m_selectServer, SIGNAL(clicked()), SLOT(onSelectServerClicked()));
    connect(ui->buttonBox, SIGNAL(rejected()), SIGNAL(loginCanceled()));

    CaseUtil::instance()->addUpperCaseEdit(ui->m_loginName);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::clearLoginInformation()
{
    ui->m_loginName->clear();
    ui->m_password->clear();
    ui->m_loginName->setFocus();
}

void LoginDialog::show()
{
    QRect desktopRect = QApplication::desktop()->availableGeometry(QApplication::desktop()->primaryScreen());
    QPoint center(desktopRect.center().x(), desktopRect.center().y());
    move(center.x() - width() * 0.5, center.y() - height() * 0.5);
    QDialog::show();
}

bool LoginDialog::checkUser(const QString &loginName, const QString &password)
{
    int tipoServer = Settings::getValue(Settings::ColumnServerType).toInt();
    QSqlDatabase db;
    if (tipoServer == Settings::MySQL){
        db = QSqlDatabase::addDatabase("QMYSQL","LoginConnection");
        QString hostName = Settings::getValue(Settings::ColumnServerAddress).toString();
        QString hostPort = Settings::getValue(Settings::ColumnServerPort).toString();
        QString hostDatabase = "sysdb";
        QString hostLoginName = "sysuser";
        QString hostPassword = "123456";
        // Decrypted Values
        hostName = Encryption::decrypt(hostName);
        hostPort = Encryption::decrypt(hostPort);
        db.setHostName(hostName);
        db.setPort(hostPort.toInt());
        db.setDatabaseName(hostDatabase);
        db.setUserName(hostLoginName);
        db.setPassword(hostPassword);
        db.open();
    }else{
        QString appDirPath(QApplication::applicationDirPath());
        QString sysdbPath(appDirPath+"/sysdb.db");
        db = QSqlDatabase::addDatabase("QSQLITE","LoginConnection");
        db.setDatabaseName(sysdbPath);
        db.open();
    }
    try{
        SqlQuery query(&db);
        query.prepare("select loginname, password from user where loginname = :loginname");
        query.bindValue(":loginname", loginName);
        query.exec();
        if (query.next()){
            QString sysloginName = query.value(0).toString();
            QString sysPassword = query.value(1).toString();
            return (loginName == sysloginName) && (Encryption::encrypt(password) == sysPassword);
        }
    }catch(SqlException &ex){
        qDebug() << ex.errorText() << Q_FUNC_INFO;
    }
    return false;
}

void LoginDialog::onAccepted()
{
    if(( ! ui->m_loginName->text().isEmpty()) && ( ! ui->m_password->text().isEmpty())){
        if(checkUser(ui->m_loginName->text(), ui->m_password->text())){
            QSqlDatabase *db = ConnectionPool::instance()->systemConnection();
            crud::User *user = crud::UserDao::instance()->getByLoginName(ui->m_loginName->text(), db);
            emit loginSuccessFull(user, db);
            hide();
            return;
        }
    }
    // User doesn't exists or password is invalid
    QMessageBox::critical(this, tr("Error"), tr("Usuario o contrasena invalidos."), QMessageBox::Ok, QMessageBox::Ok);
}

void LoginDialog::onSelectServerClicked()
{
    SelectServer::selectServer();
}


} //namespace crud
