#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include <QLayout>
#include "config.h"
#include "mi.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(PROGRAM_NAME + " :: " + tr("About"));

    String MIversion = MediaInfoDLL::MediaInfo::Option_Static(__T("Info_Version"));

    ui->about->setText(ui->about->text()
                       .replace("%program%",PROGRAM_NAME)
                       .replace("%version%",VERSION)
                       .replace("%author%",AUTHOR)
                       .replace("%mediainfoLib%",QString::fromStdWString(MIversion))
                       );
    this->setWindowFlags( (windowFlags() | Qt::CustomizeWindowHint) & ~Qt::WindowMaximizeButtonHint & ~Qt::WindowContextHelpButtonHint);
    this->setFixedSize(this->geometry().width(),this->geometry().height());
}
bool AboutDialog::eventFilter(QObject *o, QEvent *e)
{
    if(e->type() == QEvent::Hide){
        delete this;
        return true;
    }

    return QDialog::eventFilter(o, e);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::on_btnClose_clicked()
{
    this->close();
}
