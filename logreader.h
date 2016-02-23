/****************************************************************************
**
** Druid - Is a piece of program for read text file and store as json data.
** This is part of it's code.
** Copyright (C) 2015  Gustavo Brondani Schenkel
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

#ifndef LOGREADER_H
#define LOGREADER_H

#include "config.h"
#include "database.h"

#include <QtCore/QObject>
#include <QtCore/QDir>
#include <QtCore/QFileSystemWatcher>
#include <QtCore/QTextStream>
#include <QtCore/qglobal.h>

class LogReader : public QObject
{
    Q_OBJECT
public:
    explicit LogReader(QObject *parent = 0);

private:
    Config *configuration;
    QStringList fileList;
    QDir directory;
    QTextStream input;
    QFileSystemWatcher watcher;
    Database db;

    void readFile(int i);
    void checkRegularExpression(QString string);

signals:

public slots:
    void fileChanged(QString str);
};

#endif // LOGREADER_H
