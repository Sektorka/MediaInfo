#ifndef AUDIOITEM_H
#define AUDIOITEM_H

#include "mediaitem.h"
#include <QStringList>

class AudioItem : public MediaItem
{
    Q_OBJECT
public:
    explicit AudioItem(QString &filePath, QObject *parent = 0);

    static bool extensionExists(QString &);

    static const QStringList extensions;

protected:
    QString getNonFormattedResult();
    QString getBBFormattedResult();
    QString getHtmlFormattedResult();

private:
    class Data{
    public:
        QString fileName, audioCodec, bitRateMode;
        int duration, bitRate, samplingRate, channels;
        long fileSize;

        Data();
    };

    void getData(Data &d);

signals:

public slots:

};

#endif // AUDIOITEM_H
