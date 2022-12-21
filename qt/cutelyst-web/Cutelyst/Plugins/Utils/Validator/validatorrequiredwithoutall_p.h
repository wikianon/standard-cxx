/*
 * Copyright (C) 2017-2018 Matthias Fehring <kontakt@buschmann23.de>
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
#ifndef CUTELYSTVALIDATORREQUIREDWITHOUTALL_P_H
#define CUTELYSTVALIDATORREQUIREDWITHOUTALL_P_H

#include "validatorrequiredwithoutall.h"
#include "validatorrule_p.h"

namespace Cutelyst {
    
class ValidatorRequiredWithoutAllPrivate : public ValidatorRulePrivate
{
public:
    ValidatorRequiredWithoutAllPrivate(const QString &f, const QStringList &o, const ValidatorMessages &m) :
        ValidatorRulePrivate(f, m, QString()),
        otherFields(o)
    {}

    QStringList otherFields;
};
    
}

#endif //CUTELYSTVALIDATORREQUIREDWITHOUTALL_P_H

