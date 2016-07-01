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

    mongoClient.write("use druidDB\n");
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

void Database::insertNewDoc(QByteArray database)
{
    database.prepend("db.serviceOrders.insert(");
    database.append(")\n");

    QByteArray y("\"entry\":");
    int x;
    if (database.contains(y)){
        x = database.indexOf(y);
        database.insert(x+y.size(),"new Date(");
        y = "\"jobs\":";
        if (database.contains(y)){
            x = database.indexOf(y);
            database.insert(x-1,")");
        }
    }
    y = "\"jobNumber\":";
    if (database.contains(y)){
        x = database.indexOf(y);
        database.insert(x+y.size(),"NumberInt(");
        y = "\"jobStatus\":";
        if (database.contains(y)){
            x = database.indexOf(y);
            database.insert(x-1,")");
        }
    }
    y = "\"soNumber\":";
    if (database.contains(y)){
        x = database.indexOf(y);
        database.insert(x+y.size(),"NumberInt(");
        database.insert(database.size()-3,")");
//        y = "\"soStatus\":";
//        if (database.contains(y)){
//            x = database.indexOf(y);
//            database.insert(x-1,")");
//        }
    }
//    qDebug().noquote() << database;
    client(database);
}

void Database::updateJobStarted(QByteArray database)
{
    database.prepend("db.getCollection('serviceOrders').update(");

    int x;
    QByteArray y("\"jobName\":");
    if (database.contains(y)){
        x = database.indexOf(y);
        database.insert(x, "\"jobs\": {$elemMatch: {");
        y = "\"soNumber\":";
        if (database.contains(y)){
            x = database.indexOf(y);
            database.insert(x-1,"}}");
        }
    }
    y = "\"started\":";
    if (database.contains(y)){
        x = database.indexOf(y);
        database.insert(x-1,"}");
    }
    y = "\"started\":";
    if (database.contains(y)){
        x = database.indexOf(y);
        database.remove(x,y.size());
        database.insert(x,"{$set:{\"jobs.$.jobStatus\":\"running\",\"jobs.$.started\":");
    }
    y = "{\"jobs.$.started\":";
    if (database.contains(y)){
        x = database.indexOf(y);
        database.insert(x+y.size(),"new Date(");
        y = "}})";
        if (database.contains(y)){
            x = database.indexOf(y);
            database.insert(x,")");
        }
    }

    database.append("})\n");

    //    qDebug().noquote() << database;
    client(database);
}

void Database::updateJobCheck(QByteArray database)
{
    database.prepend("db.getCollection('serviceOrders').update(");

    int x;
    QByteArray y("\"jobName\":");
    if (database.contains(y)){
        x = database.indexOf(y);
        database.insert(x, "\"jobs\": {$elemMatch: {");
        y = "\"soNumber\":";
        if (database.contains(y)){
            x = database.indexOf(y);
            database.insert(x-1,"}}");
        }
    }
    database.append(",{$set:{\"open\":false}})\n");

//    qDebug().noquote() << database;
    client(database);

}

void Database::updateJobEnded(QByteArray database)
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

    QJsonDocument doc = QJsonDocument::fromJson(database);
    QJsonObject obj(doc.object());

    QString string;
    QByteArray command;
    command.append("db.getCollection('serviceOrders').update({");
    command.append("\"soNumber\":");
    command.append(QString::number(obj["jobNumber"].toDouble()));

    qDebug().noquote() << command;
    //qDebug().noquote() << QString::number(obj["jobNumber"].toDouble());
}

void Database::updateJobStep(QByteArray database)
{
    database.prepend("db.getCollection('serviceOrders').update(");

    int x;
    QByteArray y("\"jobName\":");
    if (database.contains(y)){
        x = database.indexOf(y);
        database.insert(x, "\"jobs\": {$elemMatch: {");
        y = "\"soNumber\":";
        if (database.contains(y)){
            x = database.indexOf(y);
            database.insert(x-1,"}}");
        }
    }
    y = "\"steps\":";
    if (database.contains(y)){
        x = database.indexOf(y);
        database.insert(x-1,"}");
    }
    y = "\"steps\":[";
    if (database.contains(y)){
        x = database.indexOf(y);
        database.remove(x,y.size());
        database.insert(x,"{$addToSet:{\"jobs.$.steps\":");
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
    if (database.contains(y)){
        x = database.indexOf(y);
        database.insert(x+y.size(),"new Date(");
        y = "\"program\":";
        if (database.contains(y)){
            x = database.indexOf(y);
            database.insert(x-1,")");
        }
    }
    y = "]";
    if (database.contains(y)){
        x = database.indexOf(y);
        database.replace(x,y.size(),"}");
    }
    database.append(")\n");

//    qDebug().noquote() << database;
    client(database);
}

void Database::inputCode(CodeType *code)
{
    updateCode(code);
}

void Database::receiveData(QByteArray data)
{

    switch(*code){
    case PWETRT10:
//        qDebug() << "Database: Job Entry";
//        qDebug().noquote() << "-------Inicia aqui-------\n" << data << "\n-------Termina aqui-------\n";
//        insertNewDoc(data);
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
        qDebug() << "Database: Job ended";
        qDebug().noquote() << "-------Inicia aqui-------\n" << data << "\n-------Termina aqui-------\n";
        updateJobEnded(data);
        break;
    default:
//        qDebug() << "Database: Code Not Defined!";
        break;
    }
}

void Database::openMongoConn()
{
    mongoClient.start(program, arguments, QProcess::WriteOnly);
    mongoClient.closeReadChannel(QProcess::StandardOutput);
    mongoClient.closeReadChannel(QProcess::StandardError);
}

void Database::closeMongoConn()
{
    mongoClient.write("exit\n");
    mongoClient.waitForBytesWritten();
    mongoClient.closeWriteChannel();
    if (mongoClient.waitForFinished()) {
        mongoClient.close();
    }
}

void Database::updateCode(CodeType *code)
{
    this->code = code;
    //qDebug() << "Database::code >" << *this->code;
}

