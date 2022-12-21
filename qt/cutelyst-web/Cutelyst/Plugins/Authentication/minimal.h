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
#ifndef AUTHENTICATION_STORE_MINIMAL_H
#define AUTHENTICATION_STORE_MINIMAL_H

#include <Cutelyst/cutelyst_global.h>
#include <Cutelyst/Plugins/Authentication/authenticationstore.h>
#include <QVector>

namespace Cutelyst {

class CUTELYST_PLUGIN_AUTHENTICATION_EXPORT StoreMinimal : public AuthenticationStore
{
    Q_OBJECT
public:
    /**
     * Constructs a new minimal authentication store object with the given parent.
     */
    explicit StoreMinimal(const QString &idField, QObject *parent = nullptr);
    virtual ~StoreMinimal();

    /**
     * Appends the user to internal memory storage
     */
    void addUser(const AuthenticationUser &user);

    /**
     * Reimplemented from AuthenticationStore::findUser().
     */
    virtual AuthenticationUser findUser(Context *c, const ParamsMultiMap &userInfo) final;

    /**
     * Reimplemented from AuthenticationStore::forSession().
     */
    virtual QVariant forSession(Context *c, const AuthenticationUser &user) final;

    /**
     * Reimplemented from AuthenticationStore::fromSession().
     */
    virtual AuthenticationUser fromSession(Context *c, const QVariant &frozenUser) final;

private:
    QString m_idField;
    QVector<AuthenticationUser> m_users;
};

} // namespace CutelystPlugin

#endif // AUTHENTICATION_STORE_MINIMAL_H
