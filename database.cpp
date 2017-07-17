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

#include "database.h"
#include "bsonhandler.h"

#include <QtCore/QDate>
#include <QtCore/QDebug>
#include <QtCore/QTime>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

Database::Database(QObject *parent) : QObject(parent)
{
    try {
        const auto uri = mongocxx::uri{"mongodb://192.168.121.11:27017"};
        mongocxx::options::client client_options;

        if (uri.ssl()) {
            mongocxx::options::ssl ssl_options;
            client_options.ssl_opts(ssl_options);
        }

        client = mongocxx::client{uri, client_options};
        collection = client["ACME"]["ServiceOrders"];
    } catch (std::exception &xcp) {
        qDebug() << "An error occurred: " << xcp.what() << "\n";
        exit(EXIT_FAILURE);
    }
    try {
        createIndex(collection);
    } catch (std::exception &xcp) {
        qDebug() << "An error occurred: " << xcp.what() << "\n";
        // exit(EXIT_FAILURE);
    }
}

void Database::createIndex(mongocxx::collection collection)
{
    bsoncxx::document::value index1 =
        make_document(kvp("soNumber", 1), kvp("jobs.jobName", 1), kvp("jobs.jobNumber", 1));

    mongocxx::options::index index1_options{};
    index1_options.name("noDuplicates");
    index1_options.unique(true);

    bsoncxx::document::value index2 = make_document(kvp("entry", -1), kvp("finished", -1));

    mongocxx::options::index index2_options{};
    index2_options.name("jobsView");
    index2_options.background(true);

    bsoncxx::document::value index3 = make_document(kvp("jobs.elapsedTime", -1));

    mongocxx::options::index index3_options{};
    index3_options.name("cpuTime");
    index3_options.background(true);

    collection.create_index(index1.view(), index1_options);
    collection.create_index(index2.view(), index2_options);
    collection.create_index(index3.view(), index3_options);
}

void Database::inputCode(CodeType code) { this->code = code; }

void Database::receiveData(QRegularExpressionMatch match)
{
    BsonHandler bsonHandler;
    switch (code) {
    case PWETRT10:
        try {
            mongocxx::options::update option{};
            option.upsert(true);
            collection.update_one(bsonHandler.filterSO(match), bsonHandler.newEntry(match), option);
        } catch (mongocxx::exception e) {
            qDebug() << "An error occurred: " << e.what();
        }
        break;
    case PWEUJI10:
        try {
            collection.update_one(bsonHandler.filter(match), bsonHandler.started(match));
        } catch (mongocxx::exception e) {
            qDebug() << "An error occurred: " << e.what();
        }
        break;
    case PWETRT20:
        try {
            collection.update_one(bsonHandler.filterClosedSO(match), bsonHandler.processing(match));
        } catch (mongocxx::exception e) {
            qDebug() << "An error occurred: " << e.what();
        }
        break;
    case PWETRT40:
        try {
            collection.update_one(bsonHandler.filter(match), bsonHandler.closeOS(match));
        } catch (mongocxx::exception e) {
            qDebug() << "An error occourred:" << e.what();
        }
        break;
    case PWETRT30:
        try {
            collection.update_one(bsonHandler.filterClosedSO(match), bsonHandler.ended(match));
        } catch (mongocxx::exception e) {
            qDebug() << "An error occurred: " << e.what();
        }
        try {
            auto res = collection.update_one(bsonHandler.filterWithErrors(match), bsonHandler.check());
            if (res->matched_count())
                qDebug() << "match: " << res->matched_count() << "  modified: " << res->modified_count();
        } catch (mongocxx::exception e) {
            qDebug() << "An error occurred checking: " << e.what();
        }
        break;
    case NOT_DEFINED_CODE:
        qDebug() << "Database: code not defined, yet!";
        break;
    }
}
