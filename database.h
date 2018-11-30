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

#ifndef DATABASE_H
#define DATABASE_H

#include "bsonhandler.h"
#include "definition.h"
#include "stringhandle.h"

#include <QtCore/QObject>
#include <QtCore/QRegularExpressionMatch>

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/stdx/make_unique.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/exception/exception.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/logger.hpp>
#include <mongocxx/options/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>

#include <iostream>

namespace
{

class logger final : public mongocxx::logger
{
  public:
    explicit logger(std::ostream *stream) : _stream(stream) {}

    void operator()(mongocxx::log_level level, mongocxx::stdx::string_view domain,
                    mongocxx::stdx::string_view message) noexcept override
    {
        if (level >= mongocxx::log_level::k_trace)
            return;
        *_stream << '[' << mongocxx::to_string(level) << '@' << domain << "] " << message << '\n';
    }

  private:
    std::ostream *const _stream;
};

} // namespace

class Database : public QObject
{
    Q_OBJECT
  public:
    explicit Database(QObject *parent = nullptr);

  private:
    CodeType code;
    //    const QString program = "C:/mongodb/bin/mongo.exe";
    //    const QStringList arguments = (QStringList() << "--quiet" <<
    //    "--verbose" << "--host" << "localhost" << "--port" << "27017" <<
    //    "ACME");

    mongocxx::instance inst{bsoncxx::stdx::make_unique<logger>(&std::cout)};
    mongocxx::client client;
    mongocxx::collection collection;

    void createIndex(mongocxx::collection collection);

  public slots:
    void inputCode(CodeType code);
    void receiveData(QRegularExpressionMatch match);
};

#endif // DATABASE_H
