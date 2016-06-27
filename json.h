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
#ifndef JSON_H
#define JSON_H

#include "definition.h"

#include <QtCore/QRegularExpressionMatch>
#include <QtCore/QObject>

class JSON : public QObject
{
    Q_OBJECT
public:
    explicit JSON(QObject *parent = 0);

private:
    CodeType *code;
    void newDocument(QRegularExpressionMatch match);
    void jobStep(QRegularExpressionMatch match);
    void documentToJSON(QJsonDocument jsonDocument);
    void updateCode(CodeType *code);

signals:
    void documentCreated(QByteArray);

public slots:
    void inputData(QRegularExpressionMatch match);
    void inputCode(CodeType *code);

};

#endif // JSON_H
