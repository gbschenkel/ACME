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

#include "database.h"
#include "bsonhandler.h"

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonArray>
#include <QtCore/QProcess>
#include <QtCore/QDebug>
#include <QtCore/QDate>
#include <QtCore/QTime>
#include <QtCore/QTimer>

#include <mongocxx/exception/exception.hpp>
#include <bsoncxx/json.hpp>

// TODO: change code for use native mongocxx api

Database::Database(QObject *parent) : QObject(parent)
{
  conn = mongocxx::uri{"mongodb://192.168.56.101:27017"};
  collection = conn["ACME"]["serviceOrders"];
//  auto cursor = collection.list_indexes();

  try {
    createIndex(collection);
  } catch (mongocxx::exception e) {
    qDebug() << "An error occurred: " << &e.code();
  }

}

void Database::createIndex(mongocxx::collection collection){
  using bsoncxx::builder::stream::document;

  auto index1 = document{};
  index1 << "soNumber" << 1
         << "jobs.jobName" << 1
         << "jobs.jobNumber" << 1;

  mongocxx::options::index index1_options;
  index1_options.name("noDuplicates");
  index1_options.unique(true);

  auto index2 = document{};
  index2 << "entry" << -1
         << "finished" << -1;

  mongocxx::options::index index2_options;
  index2_options.name("jobsView");
  index2_options.background(true);

  auto index3 = document{};
  index3 << "jobs.elapsedTime" << -1;

  mongocxx::options::index index3_options;
  index3_options.name("cpuTime");
  index3_options.background(true);

  collection.create_index(index1.view(), index1_options);
  collection.create_index(index2.view(), index2_options);
  collection.create_index(index3.view(), index3_options);

}

/*
void Database::updateJobCheck(QJsonObject jsonData)
{
    QByteArray dbData;

    QString soNumber = "\"soNumber\": NumberInt(" + QString::number(jsonData.value("soNumber").toDouble()) + ")";
    QString jobName = "\"jobName\": \"" + jsonData["jobs"].toArray().first().toObject().value("jobName").toString() + "\"";
    QString jobNumber = "\"jobNumber\": NumberInt(" + QString::number(jsonData["jobs"].toArray().first().toObject().value("jobNumber").toDouble()) + ")";
    QString open = "\"open\": " + QString(jsonData.value("open").toBool() ? "true": "false");
    QString finished = "\"finished\": ISODate(\"" + jsonData.value("finished").toString() + "\")";

    dbData.append("db.getCollection('serviceOrders').update({"
                  + soNumber + ","
                  "\"open\": true},"
                  "{$set:{"
                  + open + ","
                  + finished + "}})");
    dbData.append("\n");

//    qDebug().noquote() << '\n' << dbData << '\n';
//    client(dbData);
}

void Database::updateJobEnded(QJsonObject jsonData)
{
    QByteArray dbData;

    QString soNumber = "\"soNumber\": NumberInt(" + QString::number(jsonData.value("soNumber").toDouble()) + ")";
    QString jobName = "\"jobName\":\"" + jsonData["jobs"].toArray().first().toObject().value("jobName").toString() + "\"";
    QString jobNumber = "\"jobNumber\":NumberInt(" + QString::number(jsonData["jobs"].toArray().first().toObject().value("jobNumber").toDouble()) + ")";
    QString jobStatus = "\"jobs.$.jobStatus\":\"" + jsonData["jobs"].toArray().first().toObject().value("jobStatus").toString() + "\"";
    QString finished = "\"jobs.$.finished\":ISODate(\"" + jsonData["jobs"].toArray().first().toObject().value("finished").toString() + "\")";
    QString elapsedTime = "\"jobs.$.elapsedTime\":" + QString::number(jsonData["jobs"].toArray().first().toObject().value("elapsedTime").toDouble());
    QString cpuTime = "\"jobs.$.cpuTime\":" + QString::number(jsonData["jobs"].toArray().first().toObject().value("cpuTime").toDouble());
    QString srbTime = "\"jobs.$.srbTime\":" + QString::number(jsonData["jobs"].toArray().first().toObject().value("srbTime").toDouble());

    dbData.append("db.getCollection('serviceOrders').update({"
                  + soNumber + ","
                  "\"jobs\":{$elemMatch:{"
                  + jobName + ","
                  + jobNumber + "}}},"
                  "{$set:{"
                  + jobStatus + ","
                  + finished + ","
                  + elapsedTime + ","
                  + cpuTime + ","
                  + srbTime + "}})");
    dbData.append("\n");

//    qDebug().noquote() << '\n' << dbData << '\n';
//    client(dbData);
}

void Database::updateJobStep(QJsonObject jsonData)
{
    QByteArray dbData;

    QString soNumber = "\"soNumber\": NumberInt(" + QString::number(jsonData.value("soNumber").toDouble()) + ")";
    QString entry = "\"entry\": ISODate(\"" + jsonData.value("entry").toString() + "\")";
    QString open = "\"open\": " + QString(jsonData.value("open").toBool() ? "true": "false");
    QString jobName = "\"jobName\": \"" + jsonData["jobs"].toArray().first().toObject().value("jobName").toString() + "\"";
    QString jobNumber = "\"jobNumber\": NumberInt(" + QString::number(jsonData["jobs"].toArray().first().toObject().value("jobNumber").toDouble()) + ")";
    QString jobStatus = "\"jobStatus\": \"" + jsonData["jobs"].toArray().first().toObject().value("jobStatus").toString() + "\"";
    QString machine = "\"machine\": \"" + jsonData["jobs"].toArray().first().toObject().value("machine").toString() + "\"";
    QString stepName = "\"stepName\": \"" + jsonData["jobs"].toArray().first().toObject().value("steps").toArray().first().toObject().value("stepName").toString() + "\"";
    QString program = "\"program\": \"" + jsonData["jobs"].toArray().first().toObject().value("steps").toArray().first().toObject().value("program").toString() + "\"";
    QString finished = "\"finished\": ISODate(\"" + jsonData["jobs"].toArray().first().toObject().value("steps").toArray().first().toObject().value("finished").toString() + "\")";
    QString conditionCode = "\"conditionCode\": \"" + jsonData["jobs"].toArray().first().toObject().value("steps").toArray().first().toObject().value("conditionCode").toString() + "\"";
    QString elapsedTime = "\"elapsedTime\": " + QString::number(jsonData["jobs"].toArray().first().toObject().value("steps").toArray().first().toObject().value("elapsedTime").toDouble());
    QString cpuTime = "\"cpuTime\": " + QString::number(jsonData["jobs"].toArray().first().toObject().value("steps").toArray().first().toObject().value("cpuTime").toDouble());
    QString srbTime = "\"srbTime\": " + QString::number(jsonData["jobs"].toArray().first().toObject().value("steps").toArray().first().toObject().value("srbTime").toDouble());


    dbData.append("db.getCollection('serviceOrders').update({"
                  + soNumber + ","
                  "\"jobs\":{$elemMatch:{"
                  + jobName + ","
                  + jobNumber + "}}},"
                  "{$addToSet:{\"jobs.$.steps\":{"
                  + stepName + ","
                  + program + ","
                  + finished + ","
                  + conditionCode + ","
                  + elapsedTime + ","
                  + cpuTime + ","
                  + srbTime + "}}})");
    dbData.append("\n");
//    qDebug().noquote() << dbData;
//    client(dbData);
}
*/

void Database::inputCode(CodeType code)
{
    this->code = code;
}

void Database::receiveData(QRegularExpressionMatch match)
{
  BsonHandler bsonHandler;
  switch(code){
    case PWETRT10:
      try {
        collection.insert_one(bsonHandler.newDocument(match).view());
      } catch (mongocxx::exception e) {
        qDebug() << "An error occurred: " << e.what();
      }
      break;
    case PWEUJI10:
      try {
        collection.update_one(bsonHandler.filter(match).view(), bsonHandler.started(match).view());
      } catch (mongocxx::exception e) {
        qDebug() << "An error occurred: " << e.what();
      }
      break;
    case PWETRT20:
      try {
        collection.update_one(bsonHandler.filter(match).view(), bsonHandler.running(match).view());
      } catch (mongocxx::exception e) {
        qDebug() << "An error occurred: " << e.what();
      }
      break;
    case PWETRT40:
      try {
        collection.update_one(bsonHandler.filter(match).view(), bsonHandler.check(match).view());
      } catch (mongocxx::exception e) {
        qDebug() << "An error occourred:" << e.what();
      }
      break;
    case PWETRT30:
      try {
        collection.update_one(bsonHandler.filter2(match).view(), bsonHandler.ended(match).view());
      } catch (mongocxx::exception e) {
        qDebug() << "An error occurred: " << e.what();
      }
      break;
    case NOT_DEFINED_CODE:
      qDebug() << "Database: code not defined, yet!";
      break;
    }
}
