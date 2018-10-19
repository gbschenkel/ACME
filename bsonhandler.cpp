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

#include "bsonhandler.h"

#include <QtCore/QDateTime>
#include <QtCore/QDebug>

#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/types.hpp>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_array;
using bsoncxx::builder::basic::make_document;

using namespace bsoncxx::types;

BsonHandler::BsonHandler(QObject *parent) : QObject(parent) {}

bsoncxx::document::value BsonHandler::newEntry(QRegularExpressionMatch match)
{

    // qDebug() << match.captured(0);
    // Convert to milliseconds in Epoch time.
    int64_t mseconds = QDateTime::fromString(
                match.captured("entryDate") +
                match.captured("entryTime"), "dd/MM/yyhh:mm:ss")
            .addYears(100)
            .toMSecsSinceEpoch();

    bsoncxx::document::value document = make_document(
                kvp("$setOnInsert", make_document(
                        kvp("soNumber", b_int32{match.captured("soNumber").toInt()}),
                        kvp("open", b_bool{true}),
                        kvp("entry", b_date{std::chrono::milliseconds{mseconds}}),
                        kvp("finished", b_null{}))),
                kvp("$addToSet", make_document(
                        kvp("jobs", make_document(
                                kvp("jobName", b_utf8{match.captured("jobName").toStdString()}),
                                kvp("jobNumber", b_int32{match.captured("jobNumber").toInt()}),
                                kvp("jobStatus", b_utf8{"queue"}),
                                kvp("server", b_utf8{match.captured("server").toStdString()}))))));

    //  qDebug() << QString::fromStdString(bsoncxx::to_json(builder.view()));
    return document;
}

bsoncxx::document::value BsonHandler::filter(QRegularExpressionMatch match)
{
    bsoncxx::document::value filter = make_document(
                kvp("soNumber", b_int32{match.captured("soNumber").toInt()}),
                kvp("open", b_bool{true}),
                kvp("jobs.jobNumber", b_int32{match.captured("jobNumber").toInt()}));
    return filter;
}

bsoncxx::document::value BsonHandler::filterSO(QRegularExpressionMatch match)
{
    bsoncxx::document::value filter = make_document(
                kvp("soNumber", b_int32{match.captured("soNumber").toInt()}),
                kvp("open", b_bool{true}));
    return filter;
}

bsoncxx::document::value BsonHandler::filterClosedSO(QRegularExpressionMatch match)
{
    bsoncxx::document::value filter = make_document(
                kvp("soNumber", b_int32{match.captured("soNumber").toInt()}),
                kvp("jobs.jobNumber", b_int32{match.captured("jobNumber").toInt()}));

    return filter;
}

bsoncxx::document::value BsonHandler::filterWithErrors(QRegularExpressionMatch match)
{
    bsoncxx::document::value filter = make_document(
                kvp("soNumber", b_int32{match.captured("soNumber").toInt()}),
                kvp("jobs", make_document(
                        kvp("$elemMatch", make_document(
                                kvp("jobNumber", b_int32{match.captured("jobNumber").toInt()}),
                                kvp("steps", make_document(
                                        kvp("$elemMatch", make_document(
                                                kvp("program", "PWEWTO2"),
                                                kvp("conditionCode", "FLUSH"))))))))));
    //    qDebug().noquote() << QString::fromStdString(bsoncxx::to_json(filter.view()));
    return filter;
}

bsoncxx::document::value BsonHandler::started(QRegularExpressionMatch match)
{
    int64_t mseconds =
            QDateTime::fromString(match.captured("executionDate") +
                                  match.captured("executionTime"), "dd/MM/yyhh:mm:ss")
            .addYears(100)
            .toMSecsSinceEpoch();

    bsoncxx::document::value document = make_document(
                kvp("$set", make_document(
                        kvp("jobs.$.started", b_date{std::chrono::milliseconds{mseconds}}),
                        kvp("jobs.$.jobStatus", b_utf8{"running"}),
                        kvp("jobs.$.finished", b_null{}))));

    return document;
}

bsoncxx::document::value BsonHandler::processing(QRegularExpressionMatch match)
{
    int64_t mseconds = QDateTime::fromString(
                match.captured("executionDate") +
                match.captured("executionTime"), "dd/MM/yyhh:mm:ss")
            .addYears(100)
            .toMSecsSinceEpoch();
    bsoncxx::document::value document = make_document(
                kvp("$addToSet", make_document(
                        kvp("jobs.$.steps", make_document(
                                kvp("stepName", b_utf8{match.captured("stepName").toStdString()}),
                                kvp("program", b_utf8{match.captured("program").toStdString()}),
                                kvp("finished", b_date{std::chrono::milliseconds{mseconds}}),
                                kvp("conditionCode", b_utf8{match.captured("conditionCode").toStdString()}),
                                kvp("elapsedTime", b_double{match.captured("elapsedTime").toDouble() * 60}),
                                kvp("cpuTime", b_double{match.captured("cpuTime").toDouble()}),
                                kvp("srbTime", b_double{match.captured("srbTime").toDouble()}))))));

    return document;
}

bsoncxx::document::value BsonHandler::ended(QRegularExpressionMatch match)
{
    int64_t mseconds = QDateTime::fromString(
                match.captured("executionDate") +
                match.captured("executionTime"), "dd/MM/yyhh:mm:ss")
            .addYears(100)
            .toMSecsSinceEpoch();

    bsoncxx::document::value document = make_document(
                kvp("$set", make_document(
                        kvp("jobs.$.jobStatus", b_utf8{"finished"}),
                        kvp("jobs.$.finished", b_date{std::chrono::milliseconds{mseconds}}),
                        kvp("jobs.$.elapsedTime", b_double{match.captured("elapsedTime").toDouble() * 60}),
                        kvp("jobs.$.cpuTime", b_double{match.captured("cpuTime").toDouble()}),
                        kvp("jobs.$.srbTime", b_double{match.captured("srbTime").toDouble()}))));

    return document;
}

bsoncxx::document::value BsonHandler::check()
{
    bsoncxx::document::value document = make_document(
                kvp("$set", make_document(
                        kvp("jobs.$.jobStatus", b_utf8{"error"}))));

    return document;
}

bsoncxx::document::value BsonHandler::closeOS(QRegularExpressionMatch match)
{
    int64_t mseconds =
            QDateTime::fromString(match.captured("executionDate") +
                                  match.captured("executionTime"), "dd/MM/yyhh:mm:ss")
            .addYears(100)
            .toMSecsSinceEpoch();

    bsoncxx::document::value document = make_document(
                kvp("$set", make_document(
                        kvp("open", b_bool{false}),
                        kvp("finished", b_date{std::chrono::milliseconds{mseconds}}))));

    return document;
}
