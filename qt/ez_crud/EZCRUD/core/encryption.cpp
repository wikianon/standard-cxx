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

#include <QApplication>
#include <QDebug>

#include <Poco/Crypto/CipherFactory.h>

#include "encryption.h"

using namespace Poco::Crypto;

namespace crud {

Encryption::Encryption(QObject *parent) :
    QObject(parent),
    m_key(16),
    m_iv(16)
{
    init();
}

void Encryption::init()
{
    try
    {
        QString password("ezcrud");
        QString salt("asdf1234fdsa4321()159753##");
        CipherFactory &factory = CipherFactory::defaultFactory();
        CipherKey cipherKey("aes128", password.toStdString(), salt.toStdString());
        m_cipher = factory.createCipher(cipherKey);
    }
    catch (Poco::NotFoundException &ex)
    {
        qDebug() << Q_FUNC_INFO << QString::fromStdString(ex.what());
    }
}

Encryption * Encryption::instance()
{
    static Encryption *instace = 0;
    if(! instace){
        instace = new Encryption(qApp);
    }
    return instace;
}

QString Encryption::encryptText(const QString &word)
{
    QString encrypted = "";
    try{
        encrypted = QString::fromStdString(m_cipher->encryptString(word.toStdString(), Cipher::ENC_BASE64));
    }catch (Poco::RuntimeException &ex){
        qDebug() << Q_FUNC_INFO <<  QString::fromStdString(ex.what());
    }
    return encrypted;
}

QString Encryption::decryptText(const QString &word)
{
    QString decrypted = "";
    try{
        decrypted = QString::fromStdString(m_cipher->decryptString(word.toStdString(), Cipher::ENC_BASE64));
    }catch (Poco::RuntimeException &ex){
        qDebug() << Q_FUNC_INFO <<  QString::fromStdString(ex.what());
    }
    return decrypted;
}

QString Encryption::encrypt(const QString &word)
{
    return instance()->encryptText(word);
}

QString Encryption::decrypt(const QString &word)
{
    return instance()->decryptText(word);
}

} // namespace crud
