#include "SMSExporter.h"

#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QDir>
#include <QUrl>
#include <QDesktopServices>

#include <qmessage.h>
#include <qmessageservice.h>

SMSExporter::SMSExporter(QObject *parent) :
    QObject(parent)
  , needExport(false)
{
    connect(&service, SIGNAL(messagesFound(QMessageIdList)), this, SLOT(messagesFound(QMessageIdList)));
}

void SMSExporter::exportSMS()
{
    needExport = true;

    querySMS();
}

void SMSExporter::readSMS()
{
    needExport = false;

    querySMS();
}

void SMSExporter::openArchive()
{
    if (!last_filename.isEmpty())
        QDesktopServices::openUrl(QUrl(last_filename));
}

void SMSExporter::querySMS()
{
    // Create the filter needed to locate sms messages
    QMessageFilter smsFilter(QMessageFilter::byType(QMessage::Sms));

    // Only search for messages containing addresses to include
    service.queryMessages(smsFilter);
}

void SMSExporter::messagesFound(const QMessageIdList &ids)
{
    qDebug() << "Messaged IDs found Count: " << ids.count();

    // check files
    QDir dir = QDir::home();
    dir.cd("MyDocs");
    if (!dir.exists("SMSExporter")) {
        qDebug () << "Added SMSExporter dir to home";
        dir.mkdir("SMSExporter");
    }


    QString filename = QDir::homePath() + "/MyDocs/SMSExporter" + "/out.txt";
    QFile file(filename);
    QTextStream out(&file);

    if (needExport) {
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qDebug() << "Error on file open !!!";
            return;
        }

        qDebug() << "Created file : " << file.fileName();
        qDebug() << "in dir : " << QDir::homePath();
    } else {
        emit foundMessages(ids.count());
    }

    // load all messages
    for (int i = 0; i < ids.size(); ++i) {
         // we have the messagesIDs
         QMessage message(ids.at(i));

         qDebug() << " ---------------------------------------- ";
         qDebug() << "message.date() = " << message.date().toString();
         qDebug() << "message.from() = " << message.from().addressee();
         qDebug() << "message.to() = " << message.to().first().addressee();
         qDebug() << "message.subject() = " << message.subject();
         qDebug() << "message.content() = " << message.content();
         qDebug() << "message.textContent() = " << message.textContent();
         qDebug() << " ---------------------------------------- ";

         if (needExport) {
             out << "Date : " << message.date().toString() << "\r\n";
             out << "From : " << message.from().addressee()  << "\r\n";
             out << "To : " << message.to().first().addressee() << "\r\n";
             out << message.textContent() << "\r\n";
             out << "\r\n";
             out << "\r\n";
         } else {
             emit foundMessageDetails(message.date().toString(),
                                      message.from().addressee(),
                                      message.to().first().addressee(),
                                      message.textContent());
         }
    }

    if (needExport) {
        out.flush();
        file.close();

        emit messagesExportedTo(filename);

        last_filename = filename;
    }
}
