#include "filelistwidget.h"
#include "audioitem.h"
#include "videoitem.h"

FileListWidget::FileListWidget(QWidget *parent) :
    QListWidget(parent)
{
    setAcceptDrops(true);
    setMouseTracking(true);
    setDropIndicatorShown(true);
    setDragDropMode(QAbstractItemView::DropOnly);
    focused = false;
}

void FileListWidget::dragEnterEvent(QDragEnterEvent *event) {
    if (allowedDropItem(event)){
        event->acceptProposedAction();
    }
}

void FileListWidget::dragMoveEvent(QDragMoveEvent *event) {
    if (allowedDropItem(event)){
        event->acceptProposedAction();
    }
}

void FileListWidget::dropEvent(QDropEvent *event) {
    QStringList files, dirs;
    if (allowedDropItem(event, &files, &dirs)){
        foreach(QString dir, dirs){
            FileProcessDialog fpd(dir, this);
            connect(&fpd,SIGNAL(currentFile(QString &)),parent()->parent()->parent(),SLOT(addMediaItem(QString &)));
            fpd.setWindowTitle(tr("Searching files..."));
            fpd.exec();
        }

        if(files.count() > 0){
            FileProcessDialog fpd(files, this);
            connect(&fpd,SIGNAL(currentFile(QString &)),parent()->parent()->parent(),SLOT(addMediaItem(QString &)));
            fpd.setWindowTitle(tr("Adding files..."));
            fpd.exec();
        }

        event->acceptProposedAction();
    }
}

bool FileListWidget::allowedDropItem(QDropEvent *event, QStringList *files, QStringList *dirs) {
    bool retval = false;

    if (event->mimeData()->hasUrls())
    {
        QList<QUrl> urlList = event->mimeData()->urls();

        foreach(QUrl url, urlList)
        {
            QFileInfo fi(url.toLocalFile());
            QString extension = MediaItem::getExtension(url.toLocalFile());

            if(AudioItem::extensionExists(extension) ||
               VideoItem::extensionExists(extension) ||
               fi.isDir())
            {
                retval = true;

                if(files == NULL && dirs == NULL){
                    break;
                }

                if(fi.isFile() && files != NULL){
                    files->append(url.toLocalFile());
                }
                else if(fi.isDir() && dirs != NULL){
                    dirs->append(url.toLocalFile());
                }
            }
        }
    }

    return retval;
}

void FileListWidget::focusInEvent(QFocusEvent *event){
    focused = true;
}

void FileListWidget::focusOutEvent(QFocusEvent *event){
    focused = false;
}

bool FileListWidget::isFocused(){
    return this->focused;
}
