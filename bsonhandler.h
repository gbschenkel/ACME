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

#ifndef BSONHANDLER_H
#define BSONHANDLER_H

#include <QtCore/QObject>
#include <QtCore/QRegularExpressionMatch>

#include <bsoncxx/builder/basic/document.hpp>

class BsonHandler : public QObject
{
    Q_OBJECT
  public:
    explicit BsonHandler(QObject *parent = nullptr);

    bsoncxx::document::value newEntry(QRegularExpressionMatch match);
    bsoncxx::document::value filter(QRegularExpressionMatch match);
    bsoncxx::document::value filterSO(QRegularExpressionMatch match);
    bsoncxx::document::value filterClosedSO(QRegularExpressionMatch match);
    bsoncxx::document::value filterWithErrors(QRegularExpressionMatch match);
    bsoncxx::document::value started(QRegularExpressionMatch match);
    bsoncxx::document::value processing(QRegularExpressionMatch match);
    bsoncxx::document::value ended(QRegularExpressionMatch match);
    bsoncxx::document::value check();
    bsoncxx::document::value closeOS(QRegularExpressionMatch match);
    bsoncxx::document::value createIndex();
};

#endif // BSONHANDLER_H
