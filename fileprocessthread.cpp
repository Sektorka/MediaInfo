#include "fileprocessthread.h"

FileProcessThread::FileProcessThread(QStringList &fileList, QObject *parent, bool ignoreSleep) :
    QThread(parent), files(fileList), ignoreSleep(ignoreSleep)
{
    init();
}

FileProcessThread::FileProcessThread(QString &dir, QObject *parent, bool ignoreSleep) :
    QThread(parent), dir(dir), ignoreSleep(ignoreSleep)
{
    init();

    foreach(QString filt, AudioItem::extensions){
        filter.append("*" + filt);
    }

    foreach(QString filt, VideoItem::extensions){
        filter.append("*" + filt);
    }
}

FileProcessThread::FileProcessThread(QList<MediaItem *> &listItems, QObject *parent, bool ignoreSleep) :
    QThread(parent), listItems(listItems), ignoreSleep(ignoreSleep)
{
    init();
}

void FileProcessThread::init(){
    running = true;
}

void FileProcessThread::run(){
    if(dir != ""){
        QDir d(dir);
        int counter = 0;
        if(d.exists()){
            iterateDirectory(d,counter);
        }
    }
    else if(files.count() > 0){
        int counter = 1;

        foreach(QString file, files)
        {
            if(!running){
                return;
            }

            emit updateList(file,counter);
            counter++;
            if(!ignoreSleep){
                this->msleep(THREAD_SLEEP);
            }
        }
    }
    else if(listItems.count() > 0){
        int counter = 1;

        foreach(MediaItem *item, listItems)
        {
            if(!running){
                return;
            }

            item->analize();
            emit mediaAnalyzed(item,counter);
            counter++;
            if(!ignoreSleep){
                this->msleep(THREAD_SLEEP);
            }
        }
    }
}

void FileProcessThread::iterateDirectory(const QDir &sDir, int &counter)
{
    QFileInfoList list = sDir.entryInfoList(filter);
    foreach(QFileInfo info, list)
    {
        if(!running){
            return;
        }

        emit updateList(info.filePath(),++counter);
        if(!ignoreSleep){
            this->msleep(THREAD_SLEEP);
        }
    }

    if(Settings::instance()->value(Settings::IterateDirectoryRecursive ,true).toBool()){
        list = QDir(sDir).entryInfoList(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
        foreach(QFileInfo info, list)
        {
            if(!running){
                return;
            }

            iterateDirectory(QDir(info.filePath()),counter);
        }
    }
}

void FileProcessThread::cancel(){
    running = false;
}
