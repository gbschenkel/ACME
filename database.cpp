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
    qDebug() << "MongoDB";
    //startServer();
    //checkCollection();
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

//    qDebug() << database;

    mongoClient.write("use ACME\n");
    mongoClient.write(database);
    //mongoClient.write("exit\n");
    mongoClient.waitForBytesWritten();
    //mongoClient.closeWriteChannel();
    /*
    mongoClient.closeReadChannel(QProcess::StandardOutput);
    mongoClient.closeReadChannel(QProcess::StandardOutput);

    if (mongoClient.waitForFinished()) {
        mongoClient.deleteLater();
    }
    */
}

void Database::insertOrUpdateJob(QByteArray data)
{
    data.prepend("db.serviceOrders.insert(");
    data.append(")\n");

    QByteArray y("\"entry\":");
    int x;
    if (data.contains(y)){
        x = data.indexOf(y);
        data.insert(x+y.size(),"new Date(");
        y = "\"jobs\":";
        if (data.contains(y)){
            x = data.indexOf(y);
            data.insert(x-1,")");
        }
    }
    y = "\"jobNumber\":";
    if (data.contains(y)){
        x = data.indexOf(y);
        data.insert(x+y.size(),"NumberInt(");
        y = "\"jobStatus\":";
        if (data.contains(y)){
            x = data.indexOf(y);
            data.insert(x-1,")");
        }
    }
    y = "\"soNumber\":";
    if (data.contains(y)){
        x = data.indexOf(y);
        data.insert(x+y.size(),"NumberInt(");
        data.insert(data.size()-3,")");
//        y = "\"soStatus\":";
//        if (database.contains(y)){
//            x = database.indexOf(y);
//            database.insert(x-1,")");
//        }
    }
//    qDebug().noquote() << data;
//    client(data);
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

//    dbData.append("db.getCollection('serviceOrders').update({");
//    dbData.append("\"soNumber\": NumberInt(");
//    dbData.append(QString::number(jsonData.value("soNumber").toDouble()));
//    dbData.append("),");
//    dbData.append("\"jobs\":{$elemMatch:{jobName:\"");
//    dbData.append(jsonData["jobs"].toArray().first().toObject().value("jobName").toString());
//    dbData.append("\",");
//    dbData.append("jobNumber:");
//    dbData.append(QString::number(jsonData["jobs"].toArray().first().toObject().value("jobNumber").toDouble()));
//    dbData.append("}}},{$setOnInsert:{\"entry\": new Date(");
//    dbData.append(jsonData.value("entry").toString());
//    dbData.append("),");
//    dbData.append("\"jobs\":{\"jobName:");
//    dbData.append(jsonData.value("jobName").toString());

//    dbData.append("db.getCollection('serviceOrders').update({"
//                  "\"soNumber\": " + soNumber + ","
//                  "\"jobs\":{$elemMatch:{"
//                  "\"jobName\": \"" + jobName + "\","
//                  "\"jobNumber\": " + jobNumber + "}}},{$setOnInsert:{"
//                  "\"entry\": ISODate(\"" + entry + "\"),"
//                  "\"soNumber\": NumberInt(" + soNumber + "),"
//                  "\"open\": " + open + ","
//                  "\"jobs\": {"
//                  "\"jobName\": \"" + jobName + "\","
//                  "\"jobNumber\": NumberInt(" + jobNumber + "),"
//                  "\"jobStatus\": \"" + jobStatus + "\","
//                  "\"machine\": \"" + machine + "\"}}},{upsert: true})");

    dbData.append("db.getCollection('serviceOrders').update({"
                  + soNumber + ","
                  "\"jobs\":{$elemMatch:{"
                  + jobName + ","
                  + jobNumber + "}}},"
                  "{$setOnInsert:{"
                  + entry + ","
                  + soNumber + ","
                  + open + ","
                  "\"jobs\": [{"
                  + jobName + ","
                  + jobNumber + ","
                  + jobStatus + ","
                  + machine + "}]}},"
                  "{upsert: true})");

    // The \n is needed for the shell know the commit of the input
    dbData.append("\n");

//    qDebug().noquote() << '\n' << dbData << '\n';
    client(dbData);
}

void Database::updateJobStarted(QByteArray data)
{
    data.prepend("db.getCollection('serviceOrders').update(");

    int x;
    QByteArray y("\"jobName\":");
    if (data.contains(y)){
        x = data.indexOf(y);
        data.insert(x, "\"jobs\": {$elemMatch: {");
        y = "\"soNumber\":";
        if (data.contains(y)){
            x = data.indexOf(y);
            data.insert(x-1,"}}");
        }
    }
    y = "\"started\":";
    if (data.contains(y)){
        x = data.indexOf(y);
        data.insert(x-1,"}");
    }
    y = "\"started\":";
    if (data.contains(y)){
        x = data.indexOf(y);
        data.remove(x,y.size());
        data.insert(x,"{$set:{\"jobs.$.jobStatus\":\"running\",\"jobs.$.started\":");
    }
    y = "{\"jobs.$.started\":";
    if (data.contains(y)){
        x = data.indexOf(y);
        data.insert(x+y.size(),"new Date(");
        y = "}})";
        if (data.contains(y)){
            x = data.indexOf(y);
            data.insert(x,")");
        }
    }

    data.append("})\n");

    //    qDebug().noquote() << database;
    client(data);
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

void Database::updateJobCheck(QByteArray data)
{
    data.prepend("db.getCollection('serviceOrders').update(");

    int x;
    QByteArray y("\"jobName\":");
    if (data.contains(y)){
        x = data.indexOf(y);
        data.insert(x, "\"jobs\": {$elemMatch: {");
        y = "\"soNumber\":";
        if (data.contains(y)){
            x = data.indexOf(y);
            data.insert(x-1,"}}");
        }
    }
    data.append(",{$set:{\"open\":false}})\n");

//    qDebug().noquote() << database;
    client(data);

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
                  "\"jobs\":{$elemMatch:{"
                  + jobName + ","
                  + jobNumber + "}}},"
                  "{$set:{"
                  + open + ","
                  + finished + "}})");
    dbData.append("\n");

//    qDebug().noquote() << '\n' << dbData << '\n';

    client(dbData);
}

void Database::updateJobEnded(QByteArray data)
{
//    database.prepend("db.getCollection('serviceOrders').update(");

//    int x;
//    QByteArray y("\"jobName\":");
//    if (database.contains(y)){
//        x = database.indexOf(y);
//        database.insert(x, "\"jobs\": {$elemMatch: {");
//        y = "\"soNumber\":";
//        if (database.contains(y)){
//            x = database.indexOf(y);
//            database.insert(x-1,"}}");
//        }
//    }

//    database.append("})\n");

    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj(doc.object());

    QString string;
    QByteArray command;
    command.append("db.getCollection('serviceOrders').update({");
    command.append("\"soNumber\":");
    command.append(QString::number(obj["jobNumber"].toDouble()));

//    qDebug().noquote() << command;
    //qDebug().noquote() << QString::number(obj["jobNumber"].toDouble());
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

void Database::updateJobStep(QByteArray data)
{
    data.prepend("db.getCollection('serviceOrders').update(");

    int x;
    QByteArray y("\"jobName\":");
    if (data.contains(y)){
        x = data.indexOf(y);
        data.insert(x, "\"jobs\": {$elemMatch: {");
        y = "\"soNumber\":";
        if (data.contains(y)){
            x = data.indexOf(y);
            data.insert(x-1,"}}");
        }
    }
    y = "\"steps\":";
    if (data.contains(y)){
        x = data.indexOf(y);
        data.insert(x-1,"}");
    }
    y = "\"steps\":[";
    if (data.contains(y)){
        x = data.indexOf(y);
        data.remove(x,y.size());
        data.insert(x,"{$addToSet:{\"jobs.$.steps\":");
    }
//    y = "\"conditionCode\":";
//    if (database.contains(y)){
//        x = database.indexOf(y);
//        database.insert(x+y.size(),"NumberInt(");
//        y = "\"cpuTime\":";
//        if (database.contains(y)){
//                x = database.indexOf(y);
//                database.insert(x-1,")");
//        }
//    }
    y = "\"finished\":";
    if (data.contains(y)){
        x = data.indexOf(y);
        data.insert(x+y.size(),"new Date(");
        y = "\"program\":";
        if (data.contains(y)){
            x = data.indexOf(y);
            data.insert(x-1,")");
        }
    }
    y = "]";
    if (data.contains(y)){
        x = data.indexOf(y);
        data.replace(x,y.size(),"}");
    }
    data.append(")\n");

//    qDebug().noquote() << data;
//    client(data);
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
                  + srbTime + "}}})");
    dbData.append("\n");

//    qDebug().noquote() << dbData;

    client(dbData);
}

void Database::inputCode(CodeType code)
{
    updateCode(code);
}

void Database::receiveData(QByteArray data)
{

    switch(code){
    case PWETRT10:
//        qDebug() << "Database: Job Entry";
//        qDebug().noquote() << "-------Inicia aqui-------\n" << data << "\n-------Termina aqui-------\n";
//        insertOrUpdateJob(data);
        break;
    case PWEUJI10:
//        qDebug() << "Database: Job Started";
//        qDebug().noquote() << "-------Inicia aqui-------\n" << data << "\n-------Termina aqui-------\n";
//        updateJobStarted(data);
        break;
    case PWETRT20:
//        qDebug() << "Database: Processing Job - Step Ended";
//        qDebug().noquote() << "-------Inicia aqui-------\n" << data << "\n-------Termina aqui-------\n";
//        updateJobStep(data);
        break;
    case PWETRT40:
//        qDebug() << "Database: Job runned without errors";
//        qDebug().noquote() << "-------Inicia aqui-------\n" << data << "\n-------Termina aqui-------\n";
//        updateJobCheck(data);
        break;
    case PWETRT30:
//        qDebug() << "Database: Job ended";
//        qDebug().noquote() << "-------Inicia aqui-------\n" << data << "\n-------Termina aqui-------\n";
//        updateJobEnded(data);
        break;
    default:
        qDebug() << "Database: code not defined, yet!";
        break;
    }
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
    mongoClient.start(program, arguments, QProcess::WriteOnly);
    if (!mongoClient.waitForFinished(1000)){
        mongoClient.closeReadChannel(QProcess::StandardOutput);
        mongoClient.closeReadChannel(QProcess::StandardError);
    } else {
        qDebug() << "Mongo client couldn't connect to server";
        exit(1);
    }
}

void Database::closeMongoConn()
{
    mongoClient.write("exit\n");
    mongoClient.waitForBytesWritten();
    mongoClient.closeWriteChannel();
    mongoClient.waitForFinished();
    mongoClient.close();
}

void Database::updateCode(CodeType code)
{
    this->code = code;
    //qDebug() << "Database::code >" << *this->code;
}
