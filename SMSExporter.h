#ifndef SMSEXPORTER_H
#define SMSEXPORTER_H

#include <QObject>

#include "qmessageservice.h"

QTM_USE_NAMESPACE

class SMSExporter : public QObject
{
    Q_OBJECT
public:
    explicit SMSExporter(QObject *parent = 0);

    Q_INVOKABLE void exportSMS();
    Q_INVOKABLE void readSMS();
    Q_INVOKABLE void openArchive();
    
signals:
    void foundMessages(int count);
    void messagesExportedTo(QString filepath);
    void foundMessageDetails(QString date, QString from, QString to, QString content);
    
public slots:


private slots:
    void messagesFound(const QMessageIdList &ids);
    
private:
    void querySMS();

private:
    QMessageService service;
    bool needExport;
    QString last_filename;
};

#endif // SMSEXPORTER_H
