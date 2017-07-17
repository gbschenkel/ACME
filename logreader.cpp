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

#include "logreader.h"

#include <QtCore/QDateTime>
#include <QtCore/QDebug>

LogReader::LogReader(QObject *parent) : QObject(parent)
{

    connect(&watcher, SIGNAL(fileChanged(QString)), this, SLOT(fileChanged(QString)));
    connect(this, SIGNAL(dataReceived(QString)), &sHandle, SLOT(checkString(QString)));
    connect(&sHandle, SIGNAL(codeChanged(CodeType)), &db, SLOT(inputCode(CodeType)));

    connect(&sHandle, SIGNAL(dataValidated(QRegularExpressionMatch)), &db, SLOT(receiveData(QRegularExpressionMatch)));

    //  directory.setPath("Z:/Log");
    //  fileList << "/Pvqvscmw.log"  << "/Pvqvscmw1.log"
    //           << "/Pvqvscmw2.log" << "/Pvqvscmw3.log"
    //           << "/Pvqvscmw4.log" << "/Pvqvscmw5.log"
    //           << "/Pvqvscmw6.log" << "/Pvqvscmw7.log"
    //           << "/Pvqvscmw8.log" << "/Pvqvscmw9.log";

    //  directory.setPath("C:/Temp/Data");
    //  fileList << "/Pvqvscmw.log";

    directory.setPath("C:/Temp/Data");
    fileList << "/Log (1).log"
             << "/Log (2).log"
             << "/Log (3).log"
             << "/Log (4).log"
             << "/Log (5).log"
             << "/Log (6).log"
             << "/Log (7).log"
             << "/Log (8).log"
             << "/Log (9).log"
             << "/Log (10).log"
             << "/Log (11).log"
             << "/Log (12).log"
             << "/Log (13).log"
             << "/Log (14).log"
             << "/Log (15).log"
             << "/Log (16).log"
             << "/Log (17).log"
             << "/Log (18).log"
             << "/Log (19).log"
             << "/Log (20).log"
             << "/Log (21).log"
             << "/Log (22).log"
             << "/Log (23).log"
             << "/Log (24).log"
             << "/Log (25).log"
             << "/Log (26).log"
             << "/Log (27).log"
             << "/Log (28).log"
             << "/Log (29).log"
             << "/Log (30).log"
             << "/Log (31).log"
             << "/Log (32).log"
             << "/Log (33).log"
             << "/Log (34).log"
             << "/Log (35).log"
             << "/Log (36).log"
             << "/Log (37).log"
             << "/Log (38).log"
             << "/Log (39).log"
             << "/Log (40).log"
             << "/Log (41).log"
             << "/Log (42).log"
             << "/Log (43).log"
             << "/Log (44).log"
             << "/Log (45).log"
             << "/Log (46).log"
             << "/Log (47).log"
             << "/Log (48).log"
             << "/Log (49).log"
             << "/Log (50).log"
             << "/Log (51).log";

    // Força a leitura no arquivo na inicialização.
    qDebug() << "Testando\n" << directory.absolutePath() + fileList.at(0);
    readFile(0);
    watcher.addPath(directory.absolutePath() + fileList.at(0));
}

void LogReader::fileChanged(QString str)
{
    if (watcher.files().empty())
        watcher.addPath(directory.absolutePath() + fileList.at(0));
    str.remove(directory.path());
    readFile(fileList.indexOf(str));
}

void LogReader::readFile(int i)
{
    qint64 index = configuration.getIndex();
    QString lastLine;

    QFile file(directory.absolutePath() + fileList.at(i));
    if (!file.exists())
        return;
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "File not found or can't be read.";
        return;
    }
    qDebug() << "File " + directory.absolutePath() + fileList.at(i) + " opened at " +
                    QDateTime::currentDateTime().toString();

    QTextStream in(&file);
    in.seek(index);

    // O objeto recebe o conteúdo da linha na posição 'index' do arquivo texto
    lastLine = in.readLine();
    /*
       * Testa se o index não aponta para o inicio da linha.
       * Testa se o conteudo do objeto é o mesmo do conteudo do registro salvo.
       * No caso de ser diferente, ele invoca este mesmo programa \
       * para ver se o conteudo do objeto está no outro arquivo.
       * Se no final do processo ele não achar, seta o index como 0 \
       * para que o arquivo seja lido desde o inicio.
      */
    if (lastLine != configuration.getLastLine()) {
        if (i < fileList.size() - 1) {
            qDebug() << "Last string readed not found, searching in a older file...";
            readFile(i + 1);
        }
        index = 0;
        in.seek(index);
    }

    qDebug() << "Reading file...";
    emit(readToWrite());

    /*
       * Posiciona entrada de dados na última posição lida no arquivo;
       * Como essa linha já foi lida e processada na última vez, ele lê ela
       * e não faz nada, para que no próximo passo ele possa processar dados novos.
       * O objeto lastLine recebe os dados para que caso ele não tenha mais nada que processar,
       * não salve o lastLine em branco no procedimento de salvar.
       */
    while (!in.atEnd()) {
        lastLine = in.readLine();
        // qDebug () << lastLine;
        /*
           * Aqui processará os dados contidos no objeto lastLine
          */
        emit dataReceived(lastLine);
    }

    emit(writenDone());
    /*
       * O valor do index é igual a posição atual no arquivo, diminuido do tamanho do registro
     * anterior,
       * e decrementado 2(duas) unidades, que é o valor do '\n'.
       *
       */
    index = in.pos() - lastLine.size() - 2;

    configuration.setIndex(index);
    configuration.setLastLine(lastLine);
    configuration.doWrite();

    file.close();
    qDebug() << "File " + directory.absolutePath() + fileList.at(i) + " closed at " +
                    QDateTime::currentDateTime().toString();
}

bool LogReader::isRunning(const QString &process)
{
    QProcess tasklist;
    tasklist.start("tasklist",
                   QStringList() << "/NH"
                                 << "/FO"
                                 << "CSV"
                                 << "/FI" << QString("IMAGENAME eq %1").arg(process));
    tasklist.waitForFinished();
    QString output = tasklist.readAllStandardOutput();
    return output.startsWith(QString("\"%1").arg(process));
}
