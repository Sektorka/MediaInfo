#ifndef FILEPROCESSTHREAD_H
#define FILEPROCESSTHREAD_H

#include <QThread>
#include <QStringList>
#include <QDir>
#include "audioitem.h"
#include "videoitem.h"

class FileProcessThread : public QThread
{
    Q_OBJECT
public:
    explicit FileProcessThread(QStringList &fileList, QObject *parent = 0, bool ignoreSleep = false);
    explicit FileProcessThread(QString &dir, QObject *parent = 0,  bool ignoreSleep = false);
    explicit FileProcessThread(QList<MediaItem *> &listItems, QObject *parent = 0, bool ignoreSleep = false);

    void cancel();

protected:
    void run();

private:
    QList<MediaItem *> listItems;
    QStringList files;
    QString dir;
    QStringList filter;
    bool running;
    bool ignoreSleep;

    void init();
    void iterateDirectory(const QDir &, int &);

signals:
    void updateList(QString,int);
    void mediaAnalyzed(MediaItem *,int);
    
public slots:
    
};

#endif // FILEPROCESSTHREAD_H
