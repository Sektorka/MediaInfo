#ifndef FILELISTWIDGET_H
#define FILELISTWIDGET_H

#include <QListWidget>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QList>
#include <QFileInfo>
#include "fileprocessdialog.h"

class FileListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit FileListWidget(QWidget *parent = 0);
    bool isFocused();

signals:
    
public slots:
    
protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    bool allowedDropItem(QDropEvent *event, QStringList *files = NULL, QStringList *dirs = NULL);
    void focusInEvent(QFocusEvent* event);
    void focusOutEvent(QFocusEvent* event);

private:
    bool focused;

};

#endif // FILELISTWIDGET_H
