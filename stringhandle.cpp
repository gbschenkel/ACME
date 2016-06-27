/****************************************************************************
**
** <one line to give the program's name and a brief idea of what it does.>
** Copyright (C) 2016  Gustavo Brondani Schenkel
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

#include <QtCore/QRegularExpression>
#include <QtCore/QRegularExpressionMatch>
#include <QtCore/QDebug>

StringHandle::StringHandle(QObject *parent) : QObject(parent)
{

}

void StringHandle::validate(QString str){

    QRegularExpressionMatch match = stringCode.match(str);
    if (match.hasMatch()) {
        CodeType code = checkCode(match.captured("code"));
        emit codeChanged(&code);
        switch (code){
        case PWETRT10:
            match = entry.match(str);
            if (match.hasMatch())
                emit dataValidated(match);
            break;
        case PWEUJI10:
            match = start.match(str);
            if (match.hasMatch())
                emit dataValidated(match);
            break;
        case PWETRT20:
            match = processing.match(str);
            if (match.hasMatch())
                emit dataValidated(match);
            break;
        case PWETRT40:
            match = checking.match(str);
            if (match.hasMatch())
                emit dataValidated(match);
            break;
        case PWETRT30:
            match = end.match(str);
            if (match.hasMatch())
                emit dataValidated(match);
            break;
        default:
            qDebug() << match.captured(1) + " code not defined, yet!";
            break;
        }
    }
    else qDebug() << "String doesn't have match";
}

void StringHandle::checkString(QString str)
{
    QRegularExpressionMatch match;
    validate(str);
}
