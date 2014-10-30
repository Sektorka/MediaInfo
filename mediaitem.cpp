#include "MediaItem.h"

MediaItem::MediaItem(QString &filePath, QObject *parent) :
    QObject(parent), filePath(filePath)
{
    data = NULL;
    analyzed = false;
}

MediaItem::~MediaItem(){
    if(data != NULL){
        delete data;
    }
}

void MediaItem::analize(){
    if(analyzed) return;

    this->Option(QString("Inform").toStdWString(),QString("XML").toStdWString());
    this->Option(QString("Complete").toStdWString(),QString("1").toStdWString());
    this->Open(this->filePath.toStdWString());

    data = new QDomDocument();
    data->setContent(QString::fromStdWString(this->Inform()));
}

QString MediaItem::getAnalyzeResult(){
    switch(Settings::instance()->value(Settings::Format,MediaItem::Format::BB).toInt()){
        case MediaItem::Format::NONE:
            return getNonFormattedResult();
        case MediaItem::Format::HTML:
            return getHtmlFormattedResult();
        default:
            return getBBFormattedResult();
    }
}

QString MediaItem::getFilePath() const{
    return filePath;
}

QString MediaItem::getFileName() const{
    return filePath.mid(filePath.lastIndexOf("/")+1);
}

//static methods

QString MediaItem::formatSeconds(long sec){
    int hour, min;
    QString retval;

    hour = (int)(sec / 3600);
    sec %= 3600;

    min = (int)(sec / 60);
    sec %= 60;

    if(hour > 0){
        retval += (hour < 10 ? QString("0%1:").arg(hour) : QString("%1:").arg(hour));
    }

    retval += (min < 10 ? QString("0%1:").arg(min) : QString("%1:").arg(min));
    retval += (sec < 10 ? QString("0%1").arg(sec) : QString("%1").arg(sec));

    return retval;
}

QString MediaItem::formatBytes(long bytes){
    const int scale = 1024;
    QStringList orders;
    orders << "GB" << "MB" << "KB" << "B";
    qreal max = qPow(scale,orders.count()-1);

    foreach(QString order, orders)
    {
        if (bytes > max){
            return QString::number(QString::number(bytes / max,'f',2).toDouble(),'g',-1) + " " + order;
        }

        max /= scale;
    }

    return "0 B";
}

QString MediaItem::formatBits(long bits){
    const int scale = 1000;
    QStringList orders;
    orders << "Gb" << "Mb" << "Kb" << "b";
    qreal max = qPow(scale,orders.count()-1);

    foreach(QString order, orders)
    {
        if (bits > max){
            return QString::number(QString::number(bits / max,'f',2).toDouble(),'g',-1) + " " + order;
        }

        max /= scale;
    }

    return "0 b";
}

QString MediaItem::formatRate(long rate){
    const int scale = 1000;
    QStringList orders;
    orders << "GHz" << "MHz" << "KHz" << "Hz";
    qreal max = qPow(scale,orders.count()-1);

    foreach(QString order, orders)
    {
        if (rate > max){
            return QString::number(QString::number(rate / max,'f',2).toDouble(),'g',-1) + " " + order;
        }

        max /= scale;
    }

    return "0 Hz";
}

QString MediaItem::getExtension(QString path){
    int index = path.lastIndexOf('.');

    if(index == -1){
        return "";
    }

    return path.mid(index);
}
