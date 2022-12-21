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
#include <QSharedPointer>
#include <QMessageBox>

#include "selectserver.h"
#include "ui_selectserver.h"

#include "../core/settings.h"
#include "../core/connectionpool.h"
#include "../core/encryption.h"

namespace crud {

SelectServer::SelectServer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectServer)
{
    ui->setupUi(this);
    connect(ui->buttonBox, SIGNAL(accepted()), SLOT(onAccepted()));
    connect(ui->buttonBox, SIGNAL(rejected()), SLOT(reject()));
}

void SelectServer::setTipoServer(quint32 tipoServer)
{
    if(tipoServer == Settings::MySQL)
    {
        ui->rbMySQL->setChecked(true);
        QString serverAddress = Encryption::decrypt(Settings::getValue(Settings::ColumnServerAddress).toString());
        QString serverPort = Encryption::decrypt(Settings::getValue(Settings::ColumnServerPort).toString());
        ui->m_serverAddress->setText(serverAddress);
        ui->m_serverPort->setText(serverPort);
    }
    else if(tipoServer == Settings::SQLite)
    {
        ui->rbSQLite->setChecked(true);
    }
}

void SelectServer::onAccepted()
{
    if (ui->rbMySQL->isChecked()){
        Settings::setValue(Settings::ColumnServerType, Settings::MySQL);
        QString serverAddress = Encryption::encrypt(ui->m_serverAddress->text());
        QString serverPort = Encryption::encrypt(ui->m_serverPort->text());
        Settings::setValue(Settings::ColumnServerAddress, serverAddress);
        Settings::setValue(Settings::ColumnServerPort, serverPort);
    }else{
        Settings::setValue(Settings::ColumnServerType, Settings::SQLite);
    }
    accept();
}

SelectServer::~SelectServer()
{
    delete ui;
}

QDialog::DialogCode SelectServer::selectServer(QWidget *parent)
{
    int tipoServer = Settings::getValue(Settings::ColumnServerType).toInt();
    SelectServer s(parent);
    s.setTipoServer(tipoServer);
    QDialog::DialogCode code =  QDialog::DialogCode(s.exec());
    return code;
}

} // namespace crud
