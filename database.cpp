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
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonArray>
#include <QtCore/QProcess>
#include <QtCore/QDebug>
#include <QtCore/QDate>
#include <QtCore/QTime>
#include <QtCore/QTimer>

Database::Database(QObject *parent) : QObject(parent)
{
    //startServer();
    //checkCollection();
    openMongoConn();
    client("db.serviceOrders.createIndex({\"soNumber\": NumberInt(1),\"jobs.jobName\": NumberInt(1),\"jobs.jobNumber\": NumberInt(1)},{unique: true,drop: true,name: \"noDuplicates\"})\n");
    client("db.serviceOrders.createIndex({\"entry\": NumberInt(-1),\"finished\": NumberInt(-1)},{background: true,name: \"jobsView\"})\n");
    closeMongoConn();
}

void Database::startServer()
{
    QString program = "C:/mongodb/bin/mongod.exe";
    QStringList arguments;
    arguments << "-f" << "/mongodb/etc/mongod.conf";

    qDebug() << arguments;
    QProcess *server = new QProcess();
    server->start(program, arguments, QIODevice::WriteOnly);
}


void Database::client(QByteArray database)
{
    mongoClient.write(database);
    mongoClient.waitForBytesWritten();
}

void Database::insertOrUpdateJob(QJsonObject jsonData)
{
    QByteArray dbData;

    QString soNumber = "\"soNumber\": NumberInt(" + QString::number(jsonData.value("soNumber").toDouble()) + ")";
    QString jobName = "\"jobName\": \"" + jsonData["jobs"].toArray().first().toObject().value("jobName").toString() + "\"";
    QString jobNumber = "\"jobNumber\": NumberInt(" + QString::number(jsonData["jobs"].toArray().first().toObject().value("jobNumber").toDouble()) + ")";
    QString entry = "\"entry\": ISODate(\"" + jsonData.value("entry").toString() + "\")";
    QString jobStatus = "\"jobStatus\": \"" + jsonData["jobs"].toArray().first().toObject().value("jobStatus").toString() + "\"";
    QString machine = "\"machine\": \"" + jsonData["jobs"].toArray().first().toObject().value("machine").toString() + "\"";
    QString open = "\"open\": " + QString(jsonData.value("open").toBool() ? "true": "false");

    dbData.append("db.getCollection('serviceOrders').update({"
                  + soNumber + ","
                  "$or:[{\"jobs\":{$elemMatch:{"
                  + jobName + ","
                  + jobNumber + "}}},{"
                  + open + "}]},"
                  "{$setOnInsert:{"
                  + entry + ","
                  + soNumber + ","
                  + open + "},"
                  "$addToSet:{\"jobs\": {"
                  + jobName + ","
                  + jobNumber + ","
                  + jobStatus + ","
                  + machine + "}}},"
                  "{upsert: true})");
    // The \n is needed for the shell know the commit of the input
    dbData.append("\n");
//    qDebug().noquote() << '\n' << dbData << '\n';

    client(dbData);
}

void Database::updateJobStarted(QJsonObject jsonData)
{
    QByteArray dbData;

    QString soNumber = "\"soNumber\": NumberInt(" + QString::number(jsonData.value("soNumber").toDouble()) + ")";
    QString jobName = "\"jobName\": \"" + jsonData["jobs"].toArray().first().toObject().value("jobName").toString() + "\"";
    QString jobNumber = "\"jobNumber\": NumberInt(" + QString::number(jsonData["jobs"].toArray().first().toObject().value("jobNumber").toDouble()) + ")";
    QString started = "\"jobs.$.started\": ISODate(\"" + jsonData["jobs"].toArray().first().toObject().value("started").toString() + "\")";
    QString jobStatus = "\"jobs.$.jobStatus\": \"" + jsonData["jobs"].toArray().first().toObject().value("jobStatus").toString() + "\"";

    dbData.append("db.getCollection('serviceOrders').update({"
                  + soNumber + ","
                  "\"jobs\":{$elemMatch:{"
                  + jobName + ","
                  + jobNumber + "}}},"
                  "{$set:{"
                  + started + ","
                  + jobStatus + "}})");
    dbData.append("\n");

//    qDebug().noquote() << '\n' << dbData << '\n';
    client(dbData);
}

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
    client(dbData);
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
    client(dbData);
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
    client(dbData);
}

void Database::inputCode(CodeType code)
{
    updateCode(code);
}

void Database::receiveData(QJsonObject data)
{
    switch(code){
    case PWETRT10:
        insertOrUpdateJob(data);
        break;
    case PWEUJI10:
        updateJobStarted(data);
        break;
    case PWETRT20:
        updateJobStep(data);
        break;
    case PWETRT40:
        updateJobCheck(data);
        break;
    case PWETRT30:
        updateJobEnded(data);
        break;
    default:
        qDebug() << "Database: code not defined, yet!";
        break;
    }
}

void Database::openMongoConn()
{
    mongoClient.start(program, arguments, QProcess::ReadWrite);
    if (mongoClient.waitForStarted()){
//        mongoClient.write("use ACME\n");
    } else {
        qDebug() << "Mongo client couldn't connect to server";
        exit(1);
    }
}

void Database::closeMongoConn()
{
    mongoClient.write("exit\n");
    mongoClient.waitForBytesWritten();
    mongoClient.waitForFinished();
    mongoClient.close();
}

void Database::updateCode(CodeType code)
{
    this->code = code;
}
