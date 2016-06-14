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
#include "definition.h"

CodeType checkCode(QString s){
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
    else return NOT_DEFINED_CODE;
}
