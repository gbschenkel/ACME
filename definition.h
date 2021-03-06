/****************************************************************************
**
** <one line to give the program's name and a brief idea of what it does.>
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
#ifndef DEFINITION_H
#define DEFINITION_H

#include <QtCore/QObject>
#include <QtCore/QRegularExpression>

enum CodeType{
    PWETRT10, PWETRT20, PWETRT30, PWETRT40, PWEUJI10, NOT_DEFINED_CODE
};

inline CodeType checkCode(QString s){
    //qDebug() << s;
    if (s == "PWETRT10")
        return PWETRT10;
    else if (s == "PWETRT20")
        return PWETRT20;
    else if (s == "PWETRT30")
        return PWETRT30;
    else if (s == "PWETRT40")
        return PWETRT40;
    else if (s == "PWEUJI10")
        return PWEUJI10;
    return NOT_DEFINED_CODE;
}

//PWETRT10 - PWETRT20 - PWETRT30 - PWETRT40 - PWEUJI10
QRegularExpression const stringCode("(?<code>\\w{6}\\d{2})\\s");

//07/12/2015 21:40:26 - SYSB.PWETRT10 BRWCK22  ZN013232   42946 07/12/15 21:40:12
QRegularExpression const entry("^(?<bufferReadDate>\\d{2}/\\d{2}/\\d{4})\\s"
                               "(?<bufferReadTime>\\d{2}:\\d{2}:\\d{2})\\s-\\s"
                               "(?<machine>\\w{4})\\."
                               "(?<code>\\w{6}\\d{2})\\s+"
                               "(?<jobName>\\w+)\\s+"
                               "ZN(?<soNumber>\\d{6})\\s+"
                               "(?<jobNumber>\\d{5})\\s"
                               "(?<entryDate>\\d{2}/\\d{2}/\\d{2})\\s"
                               "(?<entryTime>\\d{2}:\\d{2}:\\d{2})"
                               );

//07/12/2015 21:36:14 - SYSB.PWEUJI10 BQRC122  ZN557601             STARTED   2015.341  21:36 42308 07/12/15 21:36:02                                        .00
QRegularExpression const started("^(?<bufferReadDate>\\d{2}/\\d{2}/\\d{4})\\s"
                                 "(?<bufferReadTime>\\d{2}:\\d{2}:\\d{2})\\s-\\s"
                                 "(?<machine>\\w{4})\\."
                                 "(?<code>\\w{6}\\d{2})\\s+"
                                 "(?<jobName>\\w+)\\s+"
                                 "ZN(?<soNumber>\\d{6})\\s+"
                                 "STARTED\\s+"
                                 "\\d+\\D\\d+\\s+"
                                 "\\d{2}:\\d{2}\\s"
                                 "(?<jobNumber>\\d{5})\\s"
                                 "(?<executionDate>\\d{2}/\\d{2}/\\d{2})\\s"
                                 "(?<executionTime>\\d{2}:\\d{2}:\\d{2})"
                                 );


//08/12/2015 12:28:12 - SYSB.PWETRT20 BRRBAB2  SORE0001 SORT      0000 ZN559035             E=      .001M C=      .02 S S=      .00 S 48373 08/12/15 12:28:03.00
QRegularExpression const stepProcessed("^(?<bufferReadDate>\\d{2}/\\d{2}/\\d{4})\\s"
                                       "(?<bufferReadTime>\\d{2}:\\d{2}:\\d{2})\\s-\\s"
                                       "(?<machine>\\w{4})\\."
                                       "(?<code>\\w{6}\\d{2})\\s"
                                       "(?<jobName>\\w+)\\s+"
                                       "(?<stepName>\\w+)\\s+"
                                       "(?<program>\\w+)\\s+"
                                       "(?<conditionCode>\\w+)\\s"
                                       "ZN(?<soNumber>\\d{6})\\s+"
                                       "E=\\s+(?<elapsedTime>\\d*\\D\\d+)M\\s"
                                       "C=\\s+(?<cpuTime>\\d*\\D\\d+)\\sS\\s"
                                       "S=\\s+(?<srbTime>\\d*\\D\\d+)\\sS\\s"
                                       "(?<jobNumber>\\d{5})\\s"
                                       "(?<executionDate>\\d{2}/\\d{2}/\\d{2})\\s"
                                       "(?<executionTime>\\d{2}:\\d{2}:\\d{2})"
                                       );

//08/12/2015 12:30:27 - SYSB.PWETRT40 BJLC242  ZN559043   48570 08/12/15 12:30:21                                                                            .00
QRegularExpression const checkIfOkay("^(?<bufferReadDate>\\d{2}/\\d{2}/\\d{4})\\s"
                                     "(?<bufferReadTime>\\d{2}:\\d{2}:\\d{2})\\s-\\s"
                                     "(?<machine>\\w{4})\\."
                                     "(?<code>\\w{6}\\d{2})\\s+"
                                     "(?<jobName>\\w+)\\s+"
                                     "ZN(?<soNumber>\\d{6})\\s+"
                                     "(?<jobNumber>\\d{5})\\s"
                                     "(?<executionDate>\\d{2}/\\d{2}/\\d{2})\\s"
                                     "(?<executionTime>\\d{2}:\\d{2}:\\d{2})"
                                     );

//08/12/2015 12:30:28 - SYSB.PWETRT30 BJLC242  ENDED NAME=ZN559043             ELAPSED TIME=      .050M CPU=      .26S SRB=      .00S 48570 08/12/15 12:30:22.00
QRegularExpression const ended("^(?<bufferReadDate>\\d{2}/\\d{2}/\\d{4})\\s"
                               "(?<bufferReadTime>\\d{2}:\\d{2}:\\d{2})\\s-\\s"
                               "(?<machine>\\w{4})\\."
                               "(?<code>\\w{6}\\d{2})\\s+"
                               "(?<jobName>\\w+)\\s+"
                               "ENDED NAME=ZN(?<soNumber>\\d{6})\\s+"
                               "ELAPSED TIME=\\s+(?<elapsedTime>\\d*\\D\\d+)M\\s"
                               "CPU=\\s+(?<cpuTime>\\d*\\D\\d+)S\\s"
                               "SRB=\\s+(?<srbTime>\\d*\\D\\d+)S\\s"
                               "(?<jobNumber>\\d{5})\\s"
                               "(?<executionDate>\\d{2}/\\d{2}/\\d{2})\\s"
                               "(?<executionTime>\\d{2}:\\d{2}:\\d{2})"
                               );

#endif // DEFINITION_H
