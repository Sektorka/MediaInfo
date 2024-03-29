#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

namespace Ui {
class AboutDialog;
}

class AboutDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AboutDialog(QWidget *parent = 0);
    ~AboutDialog();

protected:
    virtual bool eventFilter(QObject *o, QEvent *e);
    
private slots:
    void on_btnClose_clicked();

private:
    Ui::AboutDialog *ui;
};

#endif // ABOUTDIALOG_H
