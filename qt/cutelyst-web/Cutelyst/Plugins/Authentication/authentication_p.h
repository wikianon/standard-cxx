/*
 * Copyright (C) 2013-2017 Daniel Nicoletti <dantti12@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */
#ifndef AUTHENTICATION_P_H
#define AUTHENTICATION_P_H

#include "authentication.h"

#include <QStringList>

namespace Cutelyst {

class AuthenticationRealm;
class AuthenticationPrivate
{
public:
    AuthenticationRealm* realm(const QString &realmName) const;

    static inline AuthenticationUser restoreUser(Context *c, const QVariant &frozenUser, const QString &realmName);
    static inline AuthenticationRealm *findRealmForPersistedUser(Cutelyst::Context *c, const QMap<QString, AuthenticationRealm *> &realms, const QStringList &realmsOrder);
    static inline void setAuthenticated(Context *c, const AuthenticationUser &user, const QString &realmName, AuthenticationRealm *realm);
    static inline void setUser(Context *c, const AuthenticationUser &user, const QString &realmName = QString());
    static inline void persistUser(Context *c, const AuthenticationUser &user, const QString &realmName, AuthenticationRealm *realm);
    static void _q_postFork(Application*app);

    QString defaultRealm;
    QMap<QString, AuthenticationRealm *> realms;
    QStringList realmsOrder;
};

}

#endif // AUTHENTICATION_P_H
