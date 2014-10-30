#ifndef MEDIAMANAGER_H
#define MEDIAMANAGER_H

#include <QObject>
#include <QString>
#include "mediaitem.h"
#include "audioitem.h"
#include "videoitem.h"
#include "fileprocessdialog.h"

class MediaManager : public QObject
{
    Q_OBJECT
public:
    explicit MediaManager(QObject *parent = 0);
    bool addMediaItem(QString &);
    bool removeMediaItem(QString &);
    void removeAllItem();
    void analyze();
    int getMediaItemCount() const;
    MediaItem *getMediaItemByFilePath(QString &) const;

    static QString getFileExtension(QString &filePath);

    
private:
    QList<MediaItem *> listItems;
    bool fileExists(QString &file) const;

signals:
    void mediaAdded(MediaItem *);
    void mediaRemoved(QString &);
    void mediaAnalyzed(MediaItem *);
    
};

#endif // MEDIAMANAGER_H
