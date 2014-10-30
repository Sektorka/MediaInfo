#include "mediamanager.h"

MediaManager::MediaManager(QObject *parent) :
    QObject(parent)
{
}

bool MediaManager::addMediaItem(QString &file){
    if(fileExists(file)){
        return false;
    }

    MediaItem *item = NULL;
    QString ext = getFileExtension(file);

    if(AudioItem::extensionExists(ext)){
        item = new AudioItem(file,this);
    }
    else if(VideoItem::extensionExists(ext)){
        item = new VideoItem(file,this);
    }

    listItems.append(item);

    emit mediaAdded(item);

    return true;
}

bool MediaManager::removeMediaItem(QString &file){
    MediaItem *item = getMediaItemByFilePath(file);

    if(item == NULL){
        return false;
    }

    if(listItems.removeOne(item)){
        delete item;
        emit mediaRemoved(file);
        return true;
    }
    else{
        return false;
    }
}

void MediaManager::removeAllItem(){
    foreach(MediaItem *item, listItems){
        emit mediaRemoved(item->getFilePath());
    }

    qDeleteAll(listItems.begin(), listItems.end());
    listItems.clear();
}

void MediaManager::analyze(){
    if(listItems.count() > 0){
        FileProcessDialog fpd(listItems, dynamic_cast<QWidget *>(parent()),true);
        connect(&fpd,SIGNAL(currentFile(MediaItem *)),this,SIGNAL(mediaAnalyzed(MediaItem *)),Qt::QueuedConnection);
        fpd.setWindowTitle(tr("Analysing files..."));
        fpd.exec();
    }
}

bool MediaManager::fileExists(QString &file) const{
    foreach(MediaItem *item, listItems){
        if(item->getFilePath() == file){
            return true;
        }
    }

    return false;
}

QString MediaManager::getFileExtension(QString &filePath){
    int dotPos = -1;
    if((dotPos = filePath.lastIndexOf('.')) == -1){
        return "";
    }

    return filePath.mid(dotPos);
}

MediaItem *MediaManager::getMediaItemByFilePath(QString &filePath) const{
    foreach(MediaItem *item, listItems){
        if(item->getFilePath() == filePath){
            return item;
        }
    }

    return NULL;
}

int MediaManager::getMediaItemCount() const{
    return listItems.size();
}
