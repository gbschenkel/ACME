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

#ifndef CONFIG_H
#define CONFIG_H

#include <QtCore/QCoreApplication>
#include <QtCore/QObject>
#include <QtCore/QSettings>

class Config : public QObject
{
    Q_OBJECT
public:
    explicit Config(QObject *parent = 0);

    qint64 getIndex();
    QString getLastLine();

    void setIndex(qint64 newIndex);
    void setLastLine(QString line);
    void doWrite();

private:
    qint64 index;
    QString lastLine;
    QSettings *settings;

    void writeSettings();
    void readSettings();

signals:

public slots:


};

#endif // CONFIG_H
