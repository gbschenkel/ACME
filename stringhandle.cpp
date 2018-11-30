/****************************************************************************
**
** ACME - Is a piece of program for read text file and store as json data.
** This is part of it's code.
** Copyright (C) 2017  Gustavo Brondani Schenkel
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#include "stringhandle.h"

#include <QtCore/QDebug>
#include <QtCore/QRegularExpression>
#include <QtCore/QRegularExpressionMatch>

StringHandle::StringHandle(QObject *parent) : QObject(parent) {}

void StringHandle::checkString(QString str) { validate(str); }

void StringHandle::validate(QString str)
{

    QRegularExpressionMatch match = stringCode.match(str);
    if (match.hasMatch()) {
        code = checkCode(match.captured("code"));
        emit codeChanged(code);
        switch (code) {
        case PWETRT10:
            match = TRT10.match(str);
            if (match.hasMatch())
                emit dataValidated(match);
            break;
        case PWEUJI10:
            match = UJI10.match(str);
            if (match.hasMatch())
                emit dataValidated(match);
            break;
        case PWETRT20:
            match = TRT20.match(str);
            if (match.hasMatch())
                emit dataValidated(match);
            break;
        case PWETRT40:
            match = TRT40.match(str);
            if (match.hasMatch())
                emit dataValidated(match);
            break;
        case PWETRT30:
            match = TRT30.match(str);
            if (match.hasMatch())
                emit dataValidated(match);
            break;
        case NOT_DEFINED_CODE:
            // qDebug() << "StringHandle: " + match.captured(1) + " code not defined, yet!";
            break;
        }
    } else {
        qDebug() << "String doesn't have match:\n";
        qDebug() << str + "\n";
    }
}
