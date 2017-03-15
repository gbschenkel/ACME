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

#include "bsonhandler.h"

#include <QtCore/QDateTime>
#include <QtCore/QDebug>

#include <bsoncxx/builder/stream/array.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/types.hpp>

using namespace bsoncxx::types;
using namespace bsoncxx::builder::stream;

BsonHandler::BsonHandler(QObject *parent) : QObject(parent)
{

}

bsoncxx::document::value BsonHandler::newEntry(QRegularExpressionMatch match)
{

//  qDebug() << match.captured(0);
  // Convert to milliseconds in Epoch time.
  int64_t mseconds = QDateTime::fromString(match.captured("entryDate")+match.captured("entryTime"), "dd/MM/yyhh:mm:ss").addYears(100).toMSecsSinceEpoch();

  auto builder = document{};

  builder << "soNumber" << b_int32{match.captured("soNumber").toInt()}
          << "open" << b_bool{true}
          << "entry" << b_date{std::chrono::milliseconds{mseconds}}
          << "finished" << b_null{}
          << "jobs" << open_array << open_document
            << "jobName" << b_utf8{match.captured("jobName").toStdString()}
            << "jobNumber" << b_int32{match.captured("jobNumber").toInt()}
            << "jobStatus" << b_utf8{"queue"}
            << "machine" << b_utf8{match.captured("machine").toStdString()}
          << close_document << close_array;// << finalize;

//  qDebug() << QString::fromStdString(bsoncxx::to_json(builder.view()));
  return builder.extract();
}

bsoncxx::document::value BsonHandler::filter(QRegularExpressionMatch match)
{
  auto filter = document{};

  filter << "soNumber" << b_int32{match.captured("soNumber").toInt()}
         << "open" << b_bool{true}
         << "jobs.jobNumber" << b_int32{match.captured("jobNumber").toInt()};

  return filter.extract();
}

bsoncxx::document::value BsonHandler::filter2(QRegularExpressionMatch match)
{
  auto filter = document{};

  filter << "soNumber" << b_int32{match.captured("soNumber").toInt()}
         << "jobs.jobNumber" << b_int32{match.captured("jobNumber").toInt()};

  return filter.extract();
}

bsoncxx::document::value BsonHandler::started(QRegularExpressionMatch match)
{
  int64_t mseconds = QDateTime::fromString(match.captured("executionDate")+match.captured("executionTime"), "dd/MM/yyhh:mm:ss").addYears(100).toMSecsSinceEpoch();

  auto builder = document{};

  builder << "$set" << open_document
            << "jobs.$.started" << b_date{std::chrono::milliseconds{mseconds}}
            << "jobs.$.jobStatus" << b_utf8{"running"}
            << "jobs.$.finished" << b_null{}
          << close_document;

  return builder.extract();
}

bsoncxx::document::value BsonHandler::running(QRegularExpressionMatch match)
{
  int64_t mseconds = QDateTime::fromString(match.captured("executionDate")+match.captured("executionTime"), "dd/MM/yyhh:mm:ss").addYears(100).toMSecsSinceEpoch();
  auto builder = document{};

  builder << "$addToSet" << open_document
            << "jobs.$.steps" << open_document
              << "stepName" << b_utf8{match.captured("stepName").toStdString()}
              << "program" << b_utf8{match.captured("program").toStdString()}
              << "finished" << b_date{std::chrono::milliseconds{mseconds}}
              << "conditionCode" << b_utf8{match.captured("conditionCode").toStdString()}
              << "elapsedTime" << b_double{match.captured("elapsedTime").toDouble()*60}
              << "cpuTime" << b_double{match.captured("cpuTime").toDouble()}
              << "srbTime" << b_double{match.captured("srbTime").toDouble()}
            << close_document
          << close_document;
  return builder.extract();
}

bsoncxx::document::value BsonHandler::check(QRegularExpressionMatch match)
{
  int64_t mseconds = QDateTime::fromString(match.captured("executionDate")+match.captured("executionTime"), "dd/MM/yyhh:mm:ss").addYears(100).toMSecsSinceEpoch();

  auto builder = document{};

  builder << "$set" << open_document
            << "open" << b_bool{false}
            << "finished" << b_date{std::chrono::milliseconds{mseconds}}
          << close_document;

  return builder.extract();
}

bsoncxx::document::value BsonHandler::ended(QRegularExpressionMatch match)
{
  int64_t mseconds = QDateTime::fromString(match.captured("executionDate")+match.captured("executionTime"), "dd/MM/yyhh:mm:ss").addYears(100).toMSecsSinceEpoch();

  auto builder = document{};

  builder << "$set" << open_document
            << "jobs.$.jobStatus" << b_utf8{"finished"}
            << "jobs.$.finished" << b_date{std::chrono::milliseconds{mseconds}}
            << "jobs.$.elapsedTime" << b_double{match.captured("elapsedTime").toDouble()*60}
            << "jobs.$.cpuTime" << b_double{match.captured("cpuTime").toDouble()}
            << "jobs.$.srbTime" << b_double{match.captured("srbTime").toDouble()}
          << close_document;

  return builder.extract();
}
