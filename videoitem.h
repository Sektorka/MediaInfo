#ifndef VIDEOITEM_H
#define VIDEOITEM_H

#include "mediaitem.h"
#include <QStringList>

class VideoItem : public MediaItem
{
    Q_OBJECT
public:
    explicit VideoItem(QString &filePath, QObject *parent = 0);
    
    QString getAnalyzeResult() const;

    static bool extensionExists(QString &extension);

    static const QStringList extensions;

protected:
    QString getNonFormattedResult();
    QString getBBFormattedResult();
    QString getHtmlFormattedResult();

private:
    class Data{
    public:
        class Video{
        public:
            QString bitRateMode, ratio, videoCodec;
            int duration, bitRate, width, height;
            long size;
            float framePerSec;

            Video();
        };

        class Audio{
        public:
            QString audioCodec, bitRateMode;
            int duration, bitRate, samplingRate, channels;
            long size;

            Audio();
        };

        Data();

        long size;
        QString codec, codecInfo;
        Video video;
        QList<Audio> audios;

    };

    void getData(Data &d);

signals:
    
public slots:
    
};

#endif // VIDEOITEM_H
