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

#ifndef DATABASE_H
#define DATABASE_H

#include "stringhandle.h"
#include "definition.h"

#include <QtCore/QByteArray>
#include <QtCore/QJsonObject>
#include <QtCore/QObject>
#include <QtCore/QRegularExpressionMatch>
#include <QtCore/QThread>
#include <QtCore/QProcess>

class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(QObject *parent = 0);

private:
    CodeType code;
    QProcess mongoClient;
    const QString program = "C:/mongodb/bin/mongo.exe";
    const QStringList arguments = (QStringList() << "--quiet" << "--verbose" << "--host" << "localhost" << "--port" << "27017");

    void client(QByteArray database);
    void startServer();
    void updateCode(CodeType code);
    void insertOrUpdateJob(QJsonObject jsonData);
    void updateJobStep(QJsonObject jsonData);
    void updateJobStarted(QJsonObject jsonData);
    void updateJobCheck(QJsonObject jsonData);
    void updateJobEnded(QJsonObject jsonData);
    QByteArray otherClient(QByteArray database);
signals:

public slots:
    void inputCode(CodeType code);
    void receiveData(QJsonObject data);
    void openMongoConn();
    void closeMongoConn();

};

#endif // DATABASE_H
