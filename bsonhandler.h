/****************************************************************************
**
** Druid - Is a piece of program for read text file and store as json data.
** This is part of it's code.
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

#ifndef BSONHANDLER_H
#define BSONHANDLER_H

#include <QtCore/QObject>
#include <QtCore/QRegularExpressionMatch>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>

class BsonHandler : public QObject
{
  Q_OBJECT
public:
  explicit BsonHandler(QObject *parent = 0);

  bsoncxx::document::value newEntry(QRegularExpressionMatch match);
  bsoncxx::document::value filter(QRegularExpressionMatch match);
  bsoncxx::document::value filter2(QRegularExpressionMatch match);
  bsoncxx::document::value started(QRegularExpressionMatch match);
  bsoncxx::document::value running(QRegularExpressionMatch match);
  bsoncxx::document::value check(QRegularExpressionMatch match);
  bsoncxx::document::value ended(QRegularExpressionMatch match);
signals:

public slots:
};

#endif // BSONHANDLER_H
