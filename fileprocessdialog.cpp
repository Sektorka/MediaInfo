#include "fileprocessdialog.h"
#include "ui_fileprocessdialog.h"

FileProcessDialog::FileProcessDialog(QStringList fileList, QWidget *parent, bool ignoreSleep) :
    QDialog(parent,Qt::WindowSystemMenuHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint), files(fileList), ui(new Ui::FileProcessDialog), ignoreSleep(ignoreSleep)
{
    init();
}

FileProcessDialog::FileProcessDialog(QString dir, QWidget *parent, bool ignoreSleep) :
    QDialog(parent,Qt::WindowSystemMenuHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint), dir(dir), ui(new Ui::FileProcessDialog), ignoreSleep(ignoreSleep)
{
    init();
}

FileProcessDialog::FileProcessDialog(QList<MediaItem *> &listItems, QWidget *parent, bool ignoreSleep) :
    QDialog(parent,Qt::WindowSystemMenuHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint), listItems(listItems), ui(new Ui::FileProcessDialog), ignoreSleep(ignoreSleep)
{
    init();
}

void FileProcessDialog::init()
{
    ui->setupUi(this);

    if(files.count() > 0){
        ui->progressBar->setMaximum(files.count());
    }
    else if(listItems.count() > 0){
        ui->progressBar->setMaximum(listItems.count());
    }
    else{
        ui->progressBar->setMaximum(0);
    }

    this->setModal(true);
    this->installEventFilter(this);
    this->setFixedSize(750,85);
    _close = false;
}

bool FileProcessDialog::eventFilter(QObject *o, QEvent *e)
{
    if(e->type() == QEvent::Show){
        if(files.count() > 0){
            thread = new FileProcessThread(files, this, ignoreSleep);

            connect(thread,SIGNAL(updateList(QString,int)), this, SLOT(updateList(QString,int)));
            connect(thread,SIGNAL(finished()), this, SLOT(DoneWork()));

            thread->start();

            return true;
        }
        else if(listItems.count()){
            thread = new FileProcessThread(listItems, this);

            connect(thread,SIGNAL(mediaAnalyzed(MediaItem*,int)), this, SLOT(updateList(MediaItem*,int)));
            connect(thread,SIGNAL(finished()), this, SLOT(DoneWork()));

            thread->start();

            return true;
        }
        else if(dir != ""){
            thread = new FileProcessThread(dir, this);

            connect(thread,SIGNAL(updateList(QString,int)), this, SLOT(updateList(QString,int)));
            connect(thread,SIGNAL(finished()), this, SLOT(DoneWork()));

            thread->start();

            return true;
        }
    }

    return QDialog::eventFilter(o, e);
}

void FileProcessDialog::closeEvent(QCloseEvent *e){
    if(!_close){
        thread->cancel();
        e->ignore();
    }
}

FileProcessDialog::~FileProcessDialog()
{
    delete ui;
}

void FileProcessDialog::DoneWork()
{
    thread->deleteLater();
    _close = true;
    this->close();
}

void FileProcessDialog::updateList(MediaItem *item,int counter){
    updateList(item->getFilePath(),counter,true);
    emit currentFile(item);
}

void FileProcessDialog::updateList(QString file, int counter, bool ignoreEmit)
{
    ui->lblFileName->setText(file);

    if(listItems.count() > 0){
        ui->lblCounter->setText(QString("[%1/%2]").arg(counter).arg(listItems.count()));
        ui->progressBar->setValue(counter);
    }
    else if(files.count() > 0){
        ui->lblCounter->setText(QString("[%1/%2]").arg(counter).arg(files.count()));
        ui->progressBar->setValue(counter);
    }
    else{
        ui->lblCounter->setText(QString("[%1]").arg(counter));
    }

    if(!ignoreEmit){
        emit currentFile(file);
    }
}

void FileProcessDialog::join() const{
    thread->wait();
}
