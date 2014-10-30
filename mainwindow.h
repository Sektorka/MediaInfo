#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QActionGroup>
#include <QComboBox>
#include <QLabel>
#include <QFile>
#include <typeinfo>
#include <QPlainTextEdit>
#include <QTextStream>
#include <QProcess>
#include <QProgressBar>
#include <QPoint>
#include <QSize>
#include <QList>
#include <assert.h>
#include <process.h>
#include <CrashRpt.h>
#include "mediamanager.h"
#include "settings.h"
#include "aboutdialog.h"
#include "filelistwidget.h"
#include "updatechecker.h"
#include "config.h"
#include "languagemanager.h"
#include <QTranslator>

#ifdef Q_OS_WIN32
#include <windows.h>
#endif

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void addMediaItem(QString &file);
    void removeMediaItem(QString &file);

    void addFileToList(MediaItem *item);
    void removeFileFromList(QString &);

    void SingleApplicationHandleMessage(const QString &message);

private slots:
    void updateFormats(int,bool ignoreSave = false);
    void updateFormats();
    void mediaItemAnalyzed(MediaItem *);

    void updateDone(QDomDocument data);
    void updateError(int error);
    void langSetted();

    void on_qaAddItem_triggered();
    void on_qaRemoveItem_triggered();
    void on_qaAddFolder_triggered();
    void on_qaIterateDirectoryRecursive_triggered(bool checked);
    void on_qaNewAnalyze_triggered();
    void on_qaSaveResult_triggered();
    void on_qaExit_triggered();
    void on_qaAnalyze_triggered();
    void on_qaCopy_triggered();
    void on_qaAbout_triggered();
    void on_qaSelectAllItem_triggered();
    void on_qaCheckForUpdates_triggered();
    void on_qaCheckUpdates_triggered(bool checked);
    void on_qaSingleInstance_triggered(bool checked);
    void on_qaAddToWinExplorerContextMenu_triggered(bool checked);

protected:
    void changeEvent(QEvent*);
    void closeEvent(QCloseEvent *);

private:
    Ui::MainWindow *ui;
    QActionGroup *group, *langGroup;
    QComboBox *cbFormats;
    QComboBox *cbLangs;
    QLabel *lblFormats;
    MediaManager manager;

    FileListWidget *fileListWidget;
    QPlainTextEdit *textResult;
    QProgressBar *pgUpdater;
    QLabel *lblUpdater;
    UpdateChecker *uc;

    QList<QAction *> langAct;

    void init();
    void loadSettings();
    void putContextMenu(bool);
    void reTranslate();

    bool startupUpdate;
};

BOOL RunElevated(HWND hwnd, LPCTSTR pszPath, LPCTSTR pszParameters = NULL, LPCTSTR pszDirectory = NULL);
bool isWindowsVista();

#endif // MAINWINDOW_H
