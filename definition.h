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

enum CodeType { PWETRT10, PWETRT20, PWETRT30, PWETRT40, PWEUJI10,   //MAINFRAME
                PWELNX00, PWELNX01, PWELNX02, PWELNX03,             //MMD
                PWEUNX00, PWEUNX01, PWEUNX02, PWEUNX03,             //SCRIPT
                PWEPWR00, PWEPWR01, PWEPWR02,                       //PAYWARE
                PWEUTR10, PWEUTR30, PWEUTR90,                       //BIG
                PWEONL01, PWEONL02,                                 //ONLINE
                PWEBGW01,                                           //BGW
                PWEBRC01,                                           //BRC
                PWESPA01,                                           //BCR, BRS
                PWEPWA40,                                           //WA
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

// 07/12/2015 21:40:26 - SYSB.PWETRT10 BRWCK22  ZN013232   42946 07/12/15 21:40:12
QRegularExpression const TRT10(
        "^(?<bufferReadDate>\\d{2}/\\d{2}/\\d{4})\\s+"
        "(?<bufferReadTime>\\d{2}:\\d{2}:\\d{2})\\s-\\s+"
        "(?<server>\\w{4})."
        "(?<code>\\w{6}\\d{2})\\s+"
        "(?<jobName>\\w+)\\s+"
        "ZN(?<soNumber>\\d{6})\\s+"
        "(?<jobNumber>\\d{5})\\s"
        "(?<entryDate>\\d{2}/\\d{2}/\\d{2})\\s"
        "(?<entryTime>\\d{2}:\\d{2}:\\d{2})");

// 07/12/2015 21:36:14 - SYSB.PWEUJI10 BQRC122  ZN557601             STARTED   2015.341  21:36 42308 07/12/15 21:36:02                                        .00
QRegularExpression const UJI10(
        "^(?<bufferReadDate>\\d{2}/\\d{2}/\\d{4})\\s+"
        "(?<bufferReadTime>\\d{2}:\\d{2}:\\d{2})\\s-\\s+"
        "(?<server>\\w{4})."
        "(?<code>\\w{6}\\d{2})\\s+"
        "(?<jobName>\\w+)\\s+"
        "ZN(?<soNumber>\\d{6})\\s+"
        "STARTED\\s+"
        "\\d+\\D\\d+\\s+"
        "\\d{2}:\\d{2}\\s"
        "(?<jobNumber>\\d{5})\\s"
        "(?<executionDate>\\d{2}/\\d{2}/\\d{2})\\s"
        "(?<executionTime>\\d{2}:\\d{2}:\\d{2})");

// 08/12/2015 12:28:12 - SYSB.PWETRT20 BRRBAB2  SORE0001 SORT      0000 ZN559035             E= .001M C=      .02 S S=      .00 S 48373 08/12/15 12:28:03.00
QRegularExpression const TRT20(
        "^(?<bufferReadDate>\\d{2}/\\d{2}/\\d{4})\\s+"
        "(?<bufferReadTime>\\d{2}:\\d{2}:\\d{2})\\s-\\s+"
        "(?<server>\\w{4})."
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
        "(?<executionTime>\\d{2}:\\d{2}:\\d{2})");

// 08/12/2015 12:30:27 - SYSB.PWETRT40 BJLC242  ZN559043   48570 08/12/15 12:30:21 .00
QRegularExpression const TRT40(
        "^(?<bufferReadDate>\\d{2}/\\d{2}/\\d{4})\\s+"
        "(?<bufferReadTime>\\d{2}:\\d{2}:\\d{2})\\s-\\s+"
        "(?<server>\\w{4})."
        "(?<code>\\w{6}\\d{2})\\s+"
        "(?<jobName>\\w+)\\s+"
        "ZN(?<soNumber>\\d{6})\\s+"
        "(?<jobNumber>\\d{5})\\s"
        "(?<executionDate>\\d{2}/\\d{2}/\\d{2})\\s"
        "(?<executionTime>\\d{2}:\\d{2}:\\d{2})");

// 08/12/2015 12:30:28 - SYSB.PWETRT30 BJLC242  ENDED NAME=ZN559043             ELAPSED TIME= .050M CPU=      .26S SRB=      .00S 48570 08/12/15 12:30:22.00
QRegularExpression const TRT30(
        "^(?<bufferReadDate>\\d{2}/\\d{2}/\\d{4})\\s+"
        "(?<bufferReadTime>\\d{2}:\\d{2}:\\d{2})\\s-\\s+"
        "(?<server>\\w{4})."
        "(?<code>\\w{6}\\d{2})\\s+"
        "(?<jobName>\\w+)\\s+"
        "ENDED NAME=ZN(?<soNumber>\\d{6})\\s+"
        "ELAPSED TIME=\\s+(?<elapsedTime>\\d*\\D\\d+)M\\s"
        "CPU=\\s+(?<cpuTime>\\d*\\D\\d+)S\\s"
        "SRB=\\s+(?<srbTime>\\d*\\D\\d+)S\\s"
        "(?<jobNumber>\\d{5})\\s"
        "(?<executionDate>\\d{2}/\\d{2}/\\d{2})\\s"
        "(?<executionTime>\\d{2}:\\d{2}:\\d{2})");

// PWELNX00, PWELNX01, PWELNX02, PWELNX03
QRegularExpression const LNX00(
        "^(?<bufferReadDate>\\d{2}/\\d{2}/\\d{4})\\s+"
        "(?<bufferReadTime>\\d{2}:\\d{2}:\\d{2})\\s-\\s+"
        "(?<server>\\w{4})."
        "(?<code>\\w{6}\\d{2})\\s+"
        "(?<jobName>\\w+)\\s+"
        "ZN(?<soNumber>\\d{6})\\s+"
        "\\d{2}/\\d{2}/\\d{2}\\s"
        "\\d{2}:\\d{2}:\\d{2}"
        "(?<program>\\S+)\\s?"
        "(?<extra>)\\.*)");

QRegularExpression const LNX01(
        "^(?<bufferReadDate>\\d{2}/\\d{2}/\\d{4})\\s+"
        "(?<bufferReadTime>\\d{2}:\\d{2}:\\d{2})\\s-\\s+"
        "(?<server>\\w{4})."
        "(?<code>\\w{6}\\d{2})\\s+"
        "ZN(?<soNumber>\\d{6})\\s+"
        "(?<jobName>\\w+)\\s+"
        "(?<executionDate>\\d{2}/\\d{2}/\\d{2})\\s"
        "(?<executionTime>\\d{2}:\\d{2}:\\d{2})");

QRegularExpression const LNX02 = LNX01;

QRegularExpression const LNX03(
        "^(?<bufferReadDate>\\d{2}/\\d{2}/\\d{4})\\s+"
        "(?<bufferReadTime>\\d{2}:\\d{2}:\\d{2})\\s-\\s+"
        "(?<server>\\w{4})."
        "(?<code>\\w{6}\\d{2})\\s+"
        "ZN(?<soNumber>\\d{6})\\s+"
        "(?<jobName>\\w+)\\s+"
        "(?<executionDate>\\d{2}/\\d{2}/\\d{2})\\s"
        "(?<executionTime>\\d{2}:\\d{2}:\\d{2})\\s?"
        "(?<error>\\.*)");

QRegularExpression const UNX00(
        "^(?<bufferReadDate>\\d{2}/\\d{2}/\\d{4})\\s+"
        "(?<bufferReadTime>\\d{2}:\\d{2}:\\d{2})\\s-\\s+"
        "(?<server>\\w{4})."
        "(?<code>\\w{6}\\d{2})\\s+"
        "(?<jobName>\\w+)\\s+"
        "ZN(?<soNumber>\\d{6})\\s+"
        "(?<executionDate>\\d{2}/\\d{2}/\\d{2})\\s"
        "(?<executionTime>\\d{2}:\\d{2}:\\d{2})\\s?");

QRegularExpression const UNX01(
        "^(?<bufferReadDate>\\d{2}/\\d{2}/\\d{4})\\s+"
        "(?<bufferReadTime>\\d{2}:\\d{2}:\\d{2})\\s-\\s+"
        "(?<server>\\w{4})."
        "(?<code>\\w{6}\\d{2})\\s+"
        "(?<pid>\\d+)\\s"
        "(?<jobName>\\w+)\\s"
        "(?<program>\\w+)\\s"
        "\\.\\s\\.\\s"
        "(?<executionDate>\\d{8})"
        "(?<executionTime>\\d{4})\\s"
        "Inicio\\s?(?<message>\\.*)\\s?"
        "Z?N?(?<soNumber>\\d{0,6})$");

QRegularExpression const UNX02(
        "^(?<bufferReadDate>\\d{2}/\\d{2}/\\d{4})\\s+"
        "(?<bufferReadTime>\\d{2}:\\d{2}:\\d{2})\\s-\\s+"
        "(?<server>\\w{4})."
        "(?<code>\\w{6}\\d{2})\\s+"
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
        "^(?<bufferReadDate>\\d{2}/\\d{2}/\\d{4})\\s+"
        "(?<bufferReadTime>\\d{2}:\\d{2}:\\d{2})\\s-\\s+"
        "(?<server>\\w{4})."
        "(?<code>\\w{6}\\d{2})\\s+"
        "(?<pid>\\d+)\\s"
        "(?<jobName>\\w+)\\s"
        "(?<executionDate>\\d{2}/\\d{2}/\\d{4})\\s"
        "(?<executionTime>\\d{2}:\\d{2}:\\d{2})\\s"
        "(?<referenceDate>\\d{2}/\\d{2}/\\d{4})");

QRegularExpression const UTR30 = UTR10;

QRegularExpression const UTR90(
        "^(?<bufferReadDate>\\d{2}/\\d{2}/\\d{4})\\s+"
        "(?<bufferReadTime>\\d{2}:\\d{2}:\\d{2})\\s-\\s+"
        "(?<server>\\w{4})."
        "(?<code>\\w{6}\\d{2})\\s+"
        "(?<pid>\\d+)\\s"
        "(?<jobName>\\w+)\\s"
        "(?<executionDate>\\d{2}/\\d{2}/\\d{4})\\s"
        "(?<executionTime>\\d{2}:\\d{2}:\\d{2})\\s"
        "(?<error>\\.*)");

// 27/11/2018 20:30:38 - SYSB.PWEONL01 PYDK122B STARTED
QRegularExpression const ONL01(
        "^(?<bufferReadDate>\\d{2}/\\d{2}/\\d{4})\\s+"
        "(?<bufferReadTime>\\d{2}:\\d{2}:\\d{2})\\s-\\s+"
        "(?<server>\\w{4})."
        "(?<code>\\w{6}\\d{2})\\s+"
        "(?<jobName>\\w+)\\s+");

// 28/11/2018 20:30:28 - SYSB.PWEONL02 PYDK122B ENDED - RC=0000
QRegularExpression const ONL02(
        "^(?<bufferReadDate>\\d{2}/\\d{2}/\\d{4})\\s+"
        "(?<bufferReadTime>\\d{2}:\\d{2}:\\d{2})\\s-\\s+"
        "(?<server>\\w{4})."
        "(?<code>\\w{6}\\d{2})\\s+"
        "(?<jobName>\\w+)\\s+"
        "ENDED - RC=");

// 29/11/2018 02:51:57 - SYSB.PWEBGW01 - MENSAGEM DE LIBERACAO ENVIADA - BSBC3123
QRegularExpression const BGW01(
        "^(?<bufferReadDate>\\d{2}/\\d{2}/\\d{4})\\s+"
        "(?<bufferReadTime>\\d{2}:\\d{2}:\\d{2})\\s-\\s+"
        "(?<server>\\w{4})."
        "(?<code>\\w{6}\\d{2})\\s+"
        "- MENSAGEM DE LIBERACAO ENVIADA -\\s+"
        "(?<jobName>\\w+)\\s+");

// 29/11/2018 04:34:21 - SYSB.PWEBRC01 - SYSIN BIZF652N LIBERADA PARA EXECUCAO
QRegularExpression const BRC01(
        "^(?<bufferReadDate>\\d{2}/\\d{2}/\\d{4})\\s+"
        "(?<bufferReadTime>\\d{2}:\\d{2}:\\d{2})\\s-\\s+"
        "(?<server>\\w{4})."
        "(?<code>\\w{6}\\d{2})\\s+"
        "- SYSIN\\s+"
        "(?<jobName>\\w+)\\s+"
        "LIBERADA PARA EXECUCAO");

// 28/11/2018 19:20:11 - SYSB.PWESPA01 BRSC112  SOLICITACAO PREPARO AUTOMATICO 28/11/18 ZN419691
QRegularExpression const SPA01(
        "^(?<bufferReadDate>\\d{2}/\\d{2}/\\d{4})\\s+"
        "(?<bufferReadTime>\\d{2}:\\d{2}:\\d{2})\\s-\\s+"
        "(?<server>\\w{4})."
        "(?<code>\\w{6}\\d{2})\\s+"
        "(?<jobName>\\w+)\\s+"
        "SOLICITACAO PREPARO AUTOMATICO"
        "\\d{2}/\\d{2}/\\d{4})\\s+"
        "Z?N?(?<soNumber>\\d{0,6})$");

// 29/11/2018 12:24:10 - SYSB.PWEPWA40 29/11/2018 12:24:06 BRJC212
QRegularExpression const PQWA40(
        "^(?<bufferReadDate>\\d{2}/\\d{2}/\\d{4})\\s+"
        "(?<bufferReadTime>\\d{2}:\\d{2}:\\d{2})\\s-\\s+"
        "(?<server>\\w{4})."
        "(?<code>\\w{6}\\d{2})\\s+"
        "(?<executionDate>\\d{2}/\\d{2}/\\d{2})\\s+"
        "(?<executionTime>\\d{2}:\\d{2}:\\d{2})\\s+"
        "(?<jobName>\\w+)");

#endif // DEFINITION_H
