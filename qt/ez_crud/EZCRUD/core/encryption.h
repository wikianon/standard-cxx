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

#ifndef CRUD_SYSTEM_ENCRYPTION_H
#define CRUD_SYSTEM_ENCRYPTION_H

#include <QObject>
#include <Poco/Crypto/CipherKey.h>
#include <Poco/Crypto/Cipher.h>

#include "../global/ezcrud_global.h"

namespace crud {

class EZCRUD_EXPORT Encryption : private QObject
{
    Q_OBJECT
public:
    static QString encrypt(const QString &);
    static QString decrypt(const QString &);

private:
    static Encryption * instance();
    QString encryptText(const QString &);
    QString decryptText(const QString &);

private:
    explicit Encryption(QObject *parent);
    Poco::Crypto::CipherKey::ByteVec m_key;
    Poco::Crypto::CipherKey::ByteVec m_iv;
    Poco::Crypto::Cipher *m_cipher;

    void init();

};

} // namespace crud

#endif // CRUD_SYSTEM_ENCRYPTION_H
