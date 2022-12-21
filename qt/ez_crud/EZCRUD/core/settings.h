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

#ifndef CRUD_SYSTEM_SETTINGS_H
#define CRUD_SYSTEM_SETTINGS_H

#include <QObject>
#include <QVariant>
#include <QSettings>

#include "../global/ezcrud_global.h"

class QSqlDatabase;

namespace crud {

class EZCRUD_EXPORT Settings : public QObject
{
    Q_OBJECT
public:
    enum ServerType{MySQL = 1, SQLite = 2};
    // Database
    static const QString ColumnServerType;
    // System
    static const QString ColumnServerAddress;
    static const QString ColumnServerPort;
    // Organization
    static const QString ColumnHostDatabase;
    static const QString ColumnHostUser;
    static const QString ColumnHostPassword;


    static void setValue(const QString &settingName, QVariant value);
    static QVariant getValue(const QString &settingName);

    static QSettings * qSettings();

private:
    Settings(QObject *parent);
    static Settings * instance();

    QSettings *m_qSettings;

    /*!
            \fn QSqlDatabase* getDatabase()
            \brief Retorna un QSqlDatabase * utilizado dentro settings
    */
    QSqlDatabase * getDatabase();
    /*!
            \fn void createSettings()
            \brief Crea los registros para los settings basicos
    */
    void createSettings();
};

} // namespace crud

#endif // CRUD_SYSTEM_SETTINGS_H
