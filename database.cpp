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
    server->start(program, arguments);
}

void Database::client()
{
    QString program = "C:/mongodb/bin/mongo.exe";
    QStringList arguments;
    arguments << "--verbose" << "--host" << "localhost" << "--port" << "27017";

    QProcess *client = new QProcess();
    client->start(program, arguments);

    client->write("use druidDB\n");
    client->waitForBytesWritten();
    client->write("db.serviceOrders.insert({x:1})\n");
    client->waitForBytesWritten();
    client->write("exit\n");
    client->waitForBytesWritten();
    if (client->waitForFinished()) {
        client->kill();
    }
}

void Database::client(QByteArray dbInsert)
{
    QString program = "C:/mongodb/bin/mongo.exe";
    QStringList arguments;
    arguments << "--verbose" << "--host" << "localhost" << "--port" << "27017";

    QProcess *client = new QProcess();
    client->start(program, arguments);

    //dbInsert = "db.serviceOrders.insert("+jsonDocument.toJson(QJsonDocument::Compact).remove(9,1).remove(18,1).remove(38,1).remove(40,1)+")\n";

    qDebug() << dbInsert;
    client->write("use druidDB\n");
    client->waitForBytesWritten();
    client->write(dbInsert);
    client->waitForBytesWritten();
    client->write("exit\n");
    client->waitForBytesWritten();
    if (client->waitForFinished()) {
        client->close();
        client->kill();
    }
}

void Database::insertData(QByteArray dbInsert)
{
    dbInsert.prepend("db.serviceOrders.insert(");
    dbInsert.append(")\n");

    QByteArray y("\"entry\":");
    int x;
    if (dbInsert.contains(y)){
            x = dbInsert.indexOf(y);
            dbInsert.insert(x+y.size(),"new Date(");
            y = "\"jobs\":";
            if (dbInsert.contains(y)){
                    x = dbInsert.indexOf(y);
                    dbInsert.insert(x-1,")");
            }
    }
    y = "\"jobNumber\":";
    if (dbInsert.contains(y)){
            x = dbInsert.indexOf(y);
            dbInsert.insert(x+y.size(),"NumberInt(");
            y = "\"machine\":";
            if (dbInsert.contains(y)){
                    x = dbInsert.indexOf(y);
                    dbInsert.insert(x-1,")");
            }
    }
    y = "\"osNumber\":";
    if (dbInsert.contains(y)){
            x = dbInsert.indexOf(y);
            dbInsert.insert(x+y.size(),"NumberInt(");
            dbInsert.insert(dbInsert.size()-3,")");
    }
    client(dbInsert);
}

void Database::updateData(QByteArray dbUpdate)
{
    dbUpdate.prepend("db.serviceOrders.update(");
    dbUpdate.append(")\n");
}

void Database::checkCollection()
{

}
