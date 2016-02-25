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
#ifndef STRINGHANDLE_H
#define STRINGHANDLE_H

#include <QtCore/QObject>
#include <QtCore/QRegularExpressionMatch>

class StringHandle : public QObject
{
    Q_OBJECT
public:
    explicit StringHandle(QObject *parent = 0);

    enum CodeType{
        PWETRT10, PWETRT20, PWETRT30, PWETRT40, PWEUJI10, NOT_DEFINED_CODE
    };

    CodeType checkCode(QString);

private:
    QRegularExpressionMatch Teste(QString str);

signals:
    void dataValidated(QRegularExpressionMatch match);

public slots:
    void checkString(QString str);

};

#endif // STRINGHANDLE_H
