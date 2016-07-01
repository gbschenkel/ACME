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
#include "json.h"

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtCore/QByteArray>
#include <QtCore/QDate>
#include <QtCore/QTime>
#include <QtCore/QDebug>

JSON::JSON(QObject *parent) : QObject(parent)
{

}

void JSON::newDocument(QRegularExpressionMatch match)
{
    QJsonObject serviceOrder;
    serviceOrder["soNumber"] = match.captured("soNumber").toInt();
    serviceOrder["entry"] = QDate::fromString(match.captured("entryDate"), "dd/MM/yy").addYears(100).toString(Qt::ISODate)+"T"+QTime::fromString(match.captured("entryTime"), "hh:mm:ss").toString();
    serviceOrder["open"] = true;

    QJsonObject job;
    job["jobName"] = match.captured("jobName");
    job["jobNumber"] = match.captured("jobNumber").toInt();
    job["machine"] = match.captured("machine");
    job["jobStatus"] = "queued";

    QJsonArray jobs;
    jobs.append(job);
    serviceOrder["jobs"] = jobs;

    QJsonDocument jsonDocument;
    jsonDocument.setObject(serviceOrder);
    documentToJSON(jsonDocument);
}

void JSON::jobStarted(QRegularExpressionMatch match)
{
    QJsonObject job;
    job["soNumber"] = match.captured("soNumber").toInt();
    job["jobName"] = match.captured("jobName");
    job["jobNumber"] = match.captured("jobNumber").toInt();
    job["started"] = QDate::fromString(match.captured("executionDate"), "dd/MM/yy").addYears(100).toString(Qt::ISODate)+"T"+QTime::fromString(match.captured("executionTime"), "hh:mm:ss").toString();

    QJsonDocument jsonDocument;
    jsonDocument.setObject(job);
    documentToJSON(jsonDocument);
}

void JSON::jobStep(QRegularExpressionMatch match)
{
    QJsonObject step;
    step["stepName"] = match.captured("stepName");
    step["program"] = match.captured("program");
    step["finished"] = QDate::fromString(match.captured("executionDate"), "dd/MM/yy").addYears(100).toString(Qt::ISODate)+"T"+QTime::fromString(match.captured("executionTime"), "hh:mm:ss").toString();
    step["conditionCode"] = match.captured("conditionCode");
    step["elapsedTime"] = match.captured("elapsedTime").toDouble()*60;
    step["cpuTime"] = match.captured("cpuTime").toDouble();
    step["srbTime"] = match.captured("srbTime").toDouble();

    QJsonObject job;
    job["jobName"] = match.captured("jobName");
    job["jobNumber"] = match.captured("jobNumber").toInt();

    //The expression 'soNumber' was to be a ServiceOrder object,
    //but since the QJsonDocument don't maintain the structure when creating a new document,
    //the easiest way was put 'soNumber' as a 'job' attribute.
    job["soNumber"] = match.captured("soNumber").toInt();

    QJsonArray steps;
    steps.append(step);
    job["steps"] = steps;

    QJsonDocument jsonDocument;
    jsonDocument.setObject(job);
    documentToJSON(jsonDocument);
}

void JSON::jobCheck(QRegularExpressionMatch match)
{
    QJsonObject job;
    job["soNumber"] = match.captured("soNumber").toInt();
    job["jobName"] = match.captured("jobName");
    job["jobNumber"] = match.captured("jobNumber").toInt();

    QJsonDocument jsonDocument;
    jsonDocument.setObject(job);
    documentToJSON(jsonDocument);
}

void JSON::jobEnded(QRegularExpressionMatch match)
{
    QJsonObject job;
    job["soNumber"] = match.captured("soNumber").toInt();
    job["jobName"] = match.captured("jobName");
    job["jobNumber"] = match.captured("jobNumber").toInt();
    job["finished"] = QDate::fromString(match.captured("executionDate"), "dd/MM/yy").addYears(100).toString(Qt::ISODate)+"T"+QTime::fromString(match.captured("executionTime"), "hh:mm:ss").toString();
    job["jobStatus"] = "finished";
    job["elapsedTime"] = match.captured("elapsedTime").toDouble()*60;
    job["cpuTime"] = match.captured("cpuTime").toDouble();
    job["srbTime"] = match.captured("srbTime").toDouble();

    QJsonDocument jsonDocument;
    jsonDocument.setObject(job);
    documentToJSON(jsonDocument);
}

void JSON::documentToJSON(QJsonDocument jsonDocument){
    emit(documentCreated(jsonDocument.toJson(QJsonDocument::Compact)));
}

void JSON::updateCode(CodeType *code)
{
    this->code = code;
    //qDebug() << "JSON::code >" << *this->code;
}

void JSON::inputData(QRegularExpressionMatch match)
{
    switch (*code){
    case PWETRT10:
//        qDebug() << "JSON: Job Entry";
        newDocument(match);
        break;
    case PWEUJI10:
//        qDebug() << "JSON: Job Started";
        jobStarted(match);
        break;
    case PWETRT20:
//        qDebug() << "JSON: Processing Job - Step Ended";
        jobStep(match);
        break;
    case PWETRT40:
//        qDebug() << "JSON: Job runned without errors";
        jobCheck(match);
        break;
    case PWETRT30:
        qDebug() << "JSON: Job ended";
        jobEnded(match);
        break;
    default:
        qDebug() << "JSON: Code Not Defined!";
        break;
    }
}

void JSON::inputCode(CodeType *code)
{
    updateCode(code);
}
