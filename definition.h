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
#ifndef DEFINITION_H
#define DEFINITION_H

#include <QtCore/QRegularExpression>

#include "mainframe.h"
#include "mmd.h"
#include "script.h"
#include "payware.h"

enum CodeType { PWETRT10, PWETRT20, PWETRT30, PWETRT40, PWEUJI10,   //MAINFRAME
                PWELNX00, PWELNX01, PWELNX02, PWELNX03,             //MMD
                PWEUNX00, PWEUNX01, PWEUNX02, PWEUNX03,             //SCRIPT
                PWEPWR00, PWEPWR01, PWEPWR02,                       //PAYWARE
                PWEUTR10, PWEUTR30, PWEUTR90,                       //BIG
                PWEONL01, PWEONL02,                                 //ONLINE
                PWEBGW01,                                           //BGW
                PWEBRC01,                                           //BRC
                PWESPA01,                                           //BCR, BRS
                NOT_DEFINED_CODE };

inline CodeType checkCode(QString s)
{
    // qDebug() << s;
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
    else if (s == "PWELNX00")
        return PWELNX00;
    else if (s == "PWELNX01")
        return PWELNX01;
    else if (s == "PWELNX02")
        return PWELNX02;
    else if (s == "PWELNX03")
        return PWELNX03;
    else if (s == "PWEUNX00")
        return PWEUNX00;
    else if (s == "PWEUNX01")
        return PWEUNX01;
    else if (s == "PWEUNX02")
        return PWEUNX02;
    else if (s == "PWEUNX03")
        return PWEUNX03;
    else if (s == "PWEPWR00")
        return PWEPWR00;
    else if (s == "PWEPWR01")
        return PWEPWR01;
    else if (s == "PWEPWR02")
        return PWEPWR02;
    else if (s == "PWEONL01")
        return PWEONL01;
    else if (s == "PWEONL02")
        return PWEONL02;
    else if (s == "PWEUTR10")
        return PWEUTR10;
    else if (s == "PWEUTR30")
        return PWEUTR30;
    else if (s == "PWEBGW01")
        return PWEBGW01;
    else if (s == "PWEBRC01")
        return PWEBRC01;
    else if (s == "PWESPA01")
        return PWESPA01;
    return NOT_DEFINED_CODE;
}

QRegularExpression const stringCode("(?<code>\\w{6}\\d{2})\\s");

// PWELNX00, PWELNX01, PWELNX02, PWELNX03
QRegularExpression const LNX00(
        "^(?<bufferReadDate>\\d{2}/\\d{2}/\\d{4})\\s"
        "(?<bufferReadTime>\\d{2}:\\d{2}:\\d{2})\\s-\\s"
        "(?<server>\\w{4})."
        "(?<code>\\w{6}\\d{2})\\s+"
        "(?<jobName>\\w+)\\s+"
        "ZN(?<soNumber>\\d{6})\\s+"
        "\\d{2}/\\d{2}/\\d{2}\\s"
        "\\d{2}:\\d{2}:\\d{2}"
        "(?<program>\\S+)\\s?"
        "(?<extra>)\\.*)");

QRegularExpression const LNX01(
        "^(?<bufferReadDate>\\d{2}/\\d{2}/\\d{4})\\s"
        "(?<bufferReadTime>\\d{2}:\\d{2}:\\d{2})\\s-\\s"
        "(?<server>\\w{4})."
        "(?<code>\\w{6}\\d{2})\\s+"
        "ZN(?<soNumber>\\d{6})\\s+"
        "(?<jobName>\\w+)\\s+"
        "(?<executionDate>\\d{2}/\\d{2}/\\d{2})\\s"
        "(?<executionTime>\\d{2}:\\d{2}:\\d{2})");

QRegularExpression const LNX02 = LNX01;

QRegularExpression const LNX03(
        "^(?<bufferReadDate>\\d{2}/\\d{2}/\\d{4})\\s"
        "(?<bufferReadTime>\\d{2}:\\d{2}:\\d{2})\\s-\\s"
        "(?<server>\\w{4})."
        "(?<code>\\w{6}\\d{2})\\s+"
        "ZN(?<soNumber>\\d{6})\\s+"
        "(?<jobName>\\w+)\\s+"
        "(?<executionDate>\\d{2}/\\d{2}/\\d{2})\\s"
        "(?<executionTime>\\d{2}:\\d{2}:\\d{2})\\s?"
        "(?<error>\\.*)");

QRegularExpression const UNX00(
        "^(?<bufferReadDate>\\d{2}/\\d{2}/\\d{4})\\s"
        "(?<bufferReadTime>\\d{2}:\\d{2}:\\d{2})\\s-\\s"
        "(?<server>\\w{4})."
        "(?<code>\\w{6}\\d{2})\\s+"
        "(?<jobName>\\w+)\\s+"
        "ZN(?<soNumber>\\d{6})\\s+"
        "(?<executionDate>\\d{2}/\\d{2}/\\d{2})\\s"
        "(?<executionTime>\\d{2}:\\d{2}:\\d{2})\\s?");

QRegularExpression const UNX01(
        "^(?<bufferReadDate>\\d{2}/\\d{2}/\\d{4})\\s"
        "(?<bufferReadTime>\\d{2}:\\d{2}:\\d{2})\\s-\\s"
        "(?<server>\\w{4})."
        "(?<code>\\w{6}\\d{2})\\s"
        "(?<pid>\\d+)\\s"
        "(?<jobName>\\w+)\\s"
        "(?<program>\\w+)\\s"
        "\\.\\s\\.\\s"
        "(?<executionDate>\\d{8})"
        "(?<executionTime>\\d{4})\\s"
        "Inicio\\s?(?<message>\\.*)\\s?"
        "Z?N?(?<soNumber>\\d{0,6})$");

QRegularExpression const UNX02(
        "^(?<bufferReadDate>\\d{2}/\\d{2}/\\d{4})\\s"
        "(?<bufferReadTime>\\d{2}:\\d{2}:\\d{2})\\s-\\s"
        "(?<server>\\w{4})."
        "(?<code>\\w{6}\\d{2})\\s"
        "(?<pid>\\d+)\\s"
        "(?<jobName>\\w+)\\s"
        "(?<program>\\w+)\\s"
        "\\.\\s\\.\\s"
        "(?<executionDate>\\d{8})"
        "(?<executionTime>\\d{4})\\s"
        "Fim\\s?(?<message>\\.*)\\s*"
        "Z?N?(?<soNumber>\\d{0,6})$");

QRegularExpression const UNX03 = UNX02;

QRegularExpression const PWR00 = LNX00;
QRegularExpression const PWR01 = LNX01;
QRegularExpression const PWR02 = LNX02;
QRegularExpression const PWR03 = LNX03;

QRegularExpression const UTR10(
        "^(?<bufferReadDate>\\d{2}/\\d{2}/\\d{4})\\s"
        "(?<bufferReadTime>\\d{2}:\\d{2}:\\d{2})\\s-\\s"
        "(?<server>\\w{4})."
        "(?<code>\\w{6}\\d{2})\\s"
        "(?<pid>\\d+)\\s"
        "(?<jobName>\\w+)\\s"
        "(?<executionDate>\\d{2}/\\d{2}/\\d{4})\\s"
        "(?<executionTime>\\d{2}:\\d{2}:\\d{2})\\s"
        "(?<referenceDate>\\d{2}/\\d{2}/\\d{4})");

QRegularExpression const UTR30 = UTR10;
QRegularExpression const UTR90(
        "^(?<bufferReadDate>\\d{2}/\\d{2}/\\d{4})\\s"
        "(?<bufferReadTime>\\d{2}:\\d{2}:\\d{2})\\s-\\s"
        "(?<server>\\w{4})."
        "(?<code>\\w{6}\\d{2})\\s"
        "(?<pid>\\d+)\\s"
        "(?<jobName>\\w+)\\s"
        "(?<executionDate>\\d{2}/\\d{2}/\\d{4})\\s"
        "(?<executionTime>\\d{2}:\\d{2}:\\d{2})\\s"
        "(?<error>\\.*)");

#endif // DEFINITION_H
