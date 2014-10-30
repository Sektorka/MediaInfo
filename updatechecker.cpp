#include "updatechecker.h"
#include "config.h"

#ifdef MI_STATIC
QString link = "static";
#else
QString link = "dynamic";
#endif

#ifdef X64
QString target = "x64";
#else
QString target = "x86";
#endif

const QString UpdateChecker::URL = "http://mediainfo.sektor.hu/versioncheck/" + VERSION + "_" + target + "_" + link;

UpdateChecker::UpdateChecker()
{
    thread = new QThread();
    nam = new QNetworkAccessManager();

    connect(nam,SIGNAL(finished(QNetworkReply*)),this,SLOT(response(QNetworkReply*)));
    connect(thread,SIGNAL(started()),this,SLOT(run()));
    connect(thread,SIGNAL(finished()),this,SLOT(destroy()));

    this->moveToThread(thread);
    thread->start();
}

void UpdateChecker::run(){
    nam->get(QNetworkRequest(QUrl(URL)));
}

void UpdateChecker::destroy(){
    delete thread;
    delete this;
}

void UpdateChecker::response(QNetworkReply* reply){
    if(reply->error() == QNetworkReply::NoError) {
        QDomDocument data;
        data.setContent(QString(reply->readAll()));
        emit done(data);
    }
    else{
        emit error(reply->error());
    }

    delete reply;
    delete nam;
}
