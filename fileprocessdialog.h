#ifndef FILEPROCESSDIALOG_H
#define FILEPROCESSDIALOG_H

#include <QDialog>
#include <QStringList>
#include <QList>
#include <QCloseEvent>
#include "mediaitem.h"
#include "fileprocessthread.h"

namespace Ui {
class FileProcessDialog;
}

class FileProcessDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FileProcessDialog(QStringList fileList, QWidget *parent = 0, bool ignoreSleep = false);
    explicit FileProcessDialog(QString dir, QWidget *parent = 0, bool ignoreSleep = false);
    explicit FileProcessDialog(QList<MediaItem *> &listItems, QWidget *parent = 0, bool ignoreSleep = false);
    ~FileProcessDialog();  
    void join() const;
    
private slots:
    void DoneWork();
    void updateList(QString, int, bool ignoreEmit = false);
    void updateList(MediaItem *,int);


protected:
    bool eventFilter(QObject *o, QEvent *e);
    void closeEvent(QCloseEvent *e);

signals:
    void currentFile(QString &file);
    void currentFile(MediaItem *item);

private:
    Ui::FileProcessDialog *ui;
    QStringList files;
    QList<MediaItem *> listItems;
    QString dir;
    FileProcessThread *thread;
    bool _close;
    bool ignoreSleep;

    void init();
};

#endif // FILEPROCESSDIALOG_H
