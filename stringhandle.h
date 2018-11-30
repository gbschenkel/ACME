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

#ifndef STRINGHANDLE_H
#define STRINGHANDLE_H

#include "definition.h"

#include <QtCore/QObject>
#include <QtCore/QRegularExpressionMatch>

class StringHandle : public QObject
{
    Q_OBJECT
  public:
    explicit StringHandle(QObject *parent = nullptr);

  private:
    CodeType code;
    void validate(QString str);

  signals:
    void dataValidated(QRegularExpressionMatch match);
    void codeChanged(CodeType code);

  public slots:
    void checkString(QString str);
};

#endif // STRINGHANDLE_H
