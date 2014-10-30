#ifndef UPDATECHECKER_H
#define UPDATECHECKER_H

#include <QThread>
#include <QUrl>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtXml/QDomDocument>

class UpdateChecker : public QObject
{
    Q_OBJECT
public:
    explicit UpdateChecker();
    static const QString URL;

signals:
    void done(QDomDocument data);
    void error(int error);
    
private slots:
    void response(QNetworkReply* reply);
    void run();
    void destroy();

private:
    QNetworkAccessManager *nam;
    QThread *thread;

};

#endif // UPDATECHECKER_H
