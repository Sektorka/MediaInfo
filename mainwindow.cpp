#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fileprocessdialog.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QClipboard>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(PROGRAM_NAME);
    this->init();
}

MainWindow::~MainWindow()
{
    delete group;
    delete lblFormats;
    delete cbFormats;
    delete ui;
}

void MainWindow::init(){
    //formats
    pgUpdater = NULL;
    lblUpdater = NULL;
    uc = NULL;
    startupUpdate = false;

    group = new QActionGroup(ui->menuOutputFormat);

    ui->qaFormatNone->setActionGroup(group);
    ui->qaFormatBB->setActionGroup(group);
    ui->qaFormatHtml->setActionGroup(group);

    cbFormats = new QComboBox(ui->mainToolBar);
    cbFormats->addItem(ui->qaFormatNone->text());
    cbFormats->addItem(ui->qaFormatBB->text());
    cbFormats->addItem(ui->qaFormatHtml->text());

    langGroup = new QActionGroup(ui->menuLanguage);
    ui->qaDefaultLang->setActionGroup(langGroup);
    connect(ui->qaDefaultLang,SIGNAL(triggered()),this,SLOT(langSetted()));

    foreach(QString lng, LanguageManager::instance()->getLangFiles()){
        if(LanguageManager::instance()->exists(lng)){
            QString *prefix = new QString();
            QAction *act = new QAction(LanguageManager::getLanguageNameByPrefix(lng,prefix) + " (" + *prefix + ")",ui->menuLanguage);
            delete prefix;

            act->setActionGroup(langGroup);
            act->setCheckable(true);
            langAct.append(act);
            connect(act,SIGNAL(triggered()),this,SLOT(langSetted()));
        }
    }

    ui->menuLanguage->addActions(langAct);

    lblFormats = new QLabel(ui->mainToolBar);
    lblFormats->setText(tr("Output format:") + " ");

    ui->mainToolBar->addWidget(lblFormats);
    ui->mainToolBar->addWidget(cbFormats);

    fileListWidget = new FileListWidget(ui->splitter);
    fileListWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);

    textResult = new QPlainTextEdit(ui->splitter);
    textResult->setAcceptDrops(false);
    textResult->setReadOnly(true);

    ui->splitter->addWidget(fileListWidget);
    ui->splitter->addWidget(textResult);

    ui->mainToolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea | Qt::LeftToolBarArea | Qt::RightToolBarArea);

    connect(ui->qaFormatNone,SIGNAL(toggled(bool)), this, SLOT(updateFormats()));
    connect(ui->qaFormatBB,SIGNAL(toggled(bool)), this, SLOT(updateFormats()));
    connect(ui->qaFormatHtml,SIGNAL(toggled(bool)), this, SLOT(updateFormats()));

    connect(cbFormats,SIGNAL(currentIndexChanged(int)),this,SLOT(updateFormats(int)));

    connect(&manager,SIGNAL(mediaAdded(MediaItem *)),this,SLOT(addFileToList(MediaItem *)));
    connect(&manager,SIGNAL(mediaRemoved(QString &)),this,SLOT(removeFileFromList(QString &)));

    connect(&manager,SIGNAL(mediaAnalyzed(MediaItem*)),this,SLOT(mediaItemAnalyzed(MediaItem*)));

    //split panels
    QList<int> sizes;
    sizes << 100 << 200;
    ui->splitter->setSizes(sizes);

    //load settings
    loadSettings();

    //check for updates
    startupUpdate = true;
    on_qaCheckForUpdates_triggered();
}

void MainWindow::loadSettings(){
    QLocale loci = QLocale::system();
    QString syslang = loci.name().mid(0,loci.name().indexOf("_"));

    QRect rect = Settings::instance()->value(Settings::MWRect).toRect();
    if(rect.width() > 0 && rect.height() > 0){
        this->setGeometry(rect);
    }
    ui->qaIterateDirectoryRecursive->setChecked(Settings::instance()->value(Settings::IterateDirectoryRecursive,true).toBool());
    ui->qaCheckUpdates->setChecked(Settings::instance()->value(Settings::CheckForUpdates,true).toBool());
    ui->qaSingleInstance->setChecked(Settings::instance()->value(Settings::SingleInstace,true).toBool());
    ui->qaAddToWinExplorerContextMenu->setChecked(Settings::instance()->value(Settings::ContextMenu,false).toBool());
    updateFormats(Settings::instance()->value(Settings::Format,MediaItem::Format::BB).toInt(),true);
    this->addToolBar((Qt::ToolBarArea)Settings::instance()->value(Settings::TBArea,Qt::ToolBarArea::TopToolBarArea).toInt(),ui->mainToolBar);

    QString lang = Settings::instance()->value(Settings::LangFile,syslang).toString();
    QString *prefix = new QString();
    LanguageManager::getLanguageNameByPrefix(lang,prefix);

    foreach(QAction *act,ui->menuLanguage->actions()){
            QString pref = act->text();
            pref = pref.mid(pref.lastIndexOf("(")+1,2);
            if(*prefix == pref){
                act->setChecked(true);
            }
    }

    delete prefix;

    if(Settings::instance()->value(Settings::MWMax,false).toBool()){
        this->setWindowState(Qt::WindowState::WindowMaximized);
    }
}

void MainWindow::updateFormats(){
    int index = 0;

    foreach(QAction *format, this->group->actions()){
        if(format->isChecked()){
            this->updateFormats(index);
            return;
        }
        index++;
    }
}

void MainWindow::updateFormats(int index, bool ignoreSave){
    switch(index){
        case 0:
            ui->qaFormatNone->setChecked(true);
            break;
        case 1:
            ui->qaFormatBB->setChecked(true);
            break;
        case 2:
            ui->qaFormatHtml->setChecked(true);
            break;
    }
    this->cbFormats->setCurrentIndex(index);

    if(!ignoreSave){
        Settings::instance()->setValue(Settings::Format,index);
    }
}

void MainWindow::addMediaItem(QString &file){
    manager.addMediaItem(file);
}

void MainWindow::removeMediaItem(QString &file){
    manager.removeMediaItem(file);
}

void MainWindow::addFileToList(MediaItem *item){
    if(item == NULL){
        return;
    }

    QListWidgetItem *listitem = new QListWidgetItem(fileListWidget);
    listitem->setText(item->getFilePath());

    if(typeid(*item) == typeid(AudioItem)){
        listitem->setIcon(QIcon(":/icons/icon_audio"));
    }
    else if(typeid(*item) == typeid(VideoItem)){
        listitem->setIcon(QIcon(":/icons/icon_video"));
    }

    fileListWidget->addItem(listitem);
}

void MainWindow::removeFileFromList(QString &file){
    for (int i = fileListWidget->count()-1; i >= 0; i--){
        if(fileListWidget->item(i)->text() == file){
            fileListWidget->takeItem(i);
        }
    }
}

void MainWindow::mediaItemAnalyzed(MediaItem *item){
    textResult->appendPlainText(item->getAnalyzeResult());
}

//TOOLBAR TRIGGERS

void MainWindow::on_qaNewAnalyze_triggered()
{
    manager.removeAllItem();
    textResult->clear();
}

void MainWindow::on_qaSaveResult_triggered()
{
    if(textResult->toPlainText().count() == 0){
        QMessageBox box;

        box.setWindowTitle(PROGRAM_NAME);
        box.setText(tr("Result box is empty. Please run analyze!"));
        box.setStandardButtons(QMessageBox::Close);
        box.setButtonText(QMessageBox::Close,tr("Close"));
        box.setIcon(QMessageBox::Icon::Warning);

        box.exec();

        //QMessageBox::warning(this,PROGRAM_NAME,tr("Result box is empty. Please run analyze!"),QMessageBox::Close);
        return;
    }

    QString defDir = Settings::instance()->value(Settings::ResultFileSaveDialogDir,QDir::currentPath()).toString();
    QString filename = QFileDialog::getSaveFileName(
            this,
            tr("Save analyze result"),
            defDir,
            QString(Settings::instance()->value(Settings::Format,MediaItem::BB).toInt() == (int)MediaItem::HTML ? tr("Html document") + " (*.html)" : tr("Plain text") + " (*.txt);;" + tr("NFO file") + " (*.nfo)") + ";;" + tr("All file") + " (*.*)"
    );

    QFileInfo fi(filename);
    Settings::instance()->setValue(Settings::ResultFileSaveDialogDir,fi.absolutePath());

    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);

    QTextStream out(&file);

    out.setCodec("UTF-8");
    out << textResult->toPlainText();
    out.flush();

    file.close();
}

void MainWindow::on_qaAddItem_triggered()
{
    QString audioFilter = "";
    foreach(QString ext, AudioItem::extensions){
        audioFilter += "*" + ext + "; ";
    }
    audioFilter = audioFilter.left(audioFilter.length()-2);

    QString videoFilter = "";
    foreach(QString ext, VideoItem::extensions){
        videoFilter += "*" + ext + "; ";
    }
    videoFilter = videoFilter.left(videoFilter.length()-2);

    QString defDir = Settings::instance()->value(Settings::AddMediaFileOpenDialogDir,QDir::currentPath()).toString();
    QStringList fileList =
            QFileDialog::getOpenFileNames(this,tr("Add media files"),defDir,
                                          tr("All supported files") + " (" + audioFilter + "; " + videoFilter + ");;" +
                                          tr("Audio files") + " (" + audioFilter + ");;" +
                                          tr("Video files") + " (" + videoFilter + ")");

    if(fileList.count() > 0){
        QFileInfo first(fileList.at(0));
        Settings::instance()->setValue(Settings::AddMediaFileOpenDialogDir,first.absolutePath());

        FileProcessDialog fpd(fileList,this);
        connect(&fpd,SIGNAL(currentFile(QString &)),this,SLOT(addMediaItem(QString &)));
        fpd.setWindowTitle(tr("Adding files..."));
        fpd.exec();
    }
}

void MainWindow::on_qaAddFolder_triggered()
{
    QString defDir = Settings::instance()->value(Settings::AddFolderOpenDialogDir,QDir::currentPath()).toString();
    QString dir = QFileDialog::getExistingDirectory(this,tr("Add directory"), defDir,QFileDialog::ShowDirsOnly);

    if(dir != ""){
        Settings::instance()->setValue(Settings::AddFolderOpenDialogDir,dir);

        FileProcessDialog fpd(dir,this);
        connect(&fpd,SIGNAL(currentFile(QString &)),this,SLOT(addMediaItem(QString &)));
        fpd.setWindowTitle(tr("Searching files..."));
        fpd.exec();
    }
}

void MainWindow::on_qaRemoveItem_triggered()
{
    if(fileListWidget->selectedItems().count() == 0){
        QMessageBox box;

        box.setWindowTitle(PROGRAM_NAME);
        box.setText(tr("Please select media item from the list!"));
        box.setStandardButtons(QMessageBox::Close);
        box.setButtonText(QMessageBox::Close,tr("Close"));
        box.setIcon(QMessageBox::Icon::Warning);

        box.exec();
        //QMessageBox::warning(this,PROGRAM_NAME,tr("Please select media item from the list!"),QMessageBox::Close);
        return;
    }

    QStringList fileList;
    QListIterator<QListWidgetItem *> it(fileListWidget->selectedItems());
    it.toBack();

    while(it.hasPrevious()){
        fileList << it.previous()->text();
    }

    if(fileList.size() > 0){
        FileProcessDialog fpd(fileList,this,true);
        connect(&fpd,SIGNAL(currentFile(QString &)),this,SLOT(removeMediaItem(QString &)));
        fpd.setWindowTitle(tr("Removing files..."));
        fpd.exec();
    }
}

void MainWindow::on_qaAnalyze_triggered()
{
    if(fileListWidget->count() == 0){
        QMessageBox box;

        box.setWindowTitle(PROGRAM_NAME);
        box.setText(tr("Please add media item to the list!"));
        box.setStandardButtons(QMessageBox::Close);
        box.setButtonText(QMessageBox::Close,tr("Close"));
        box.setIcon(QMessageBox::Icon::Warning);

        box.exec();
        //QMessageBox::warning(this,PROGRAM_NAME,tr("Please add media item to the list!"),QMessageBox::Close);
        return;
    }

    textResult->clear();
    manager.analyze();
}

void MainWindow::on_qaCopy_triggered()
{
    if(textResult->toPlainText() != ""){
        QApplication::clipboard()->setText(textResult->toPlainText());

        QMessageBox box;

        box.setWindowTitle(PROGRAM_NAME);
        box.setText(tr("The analyze result copied to clipboard."));
        box.setStandardButtons(QMessageBox::Close);
        box.setButtonText(QMessageBox::Close,tr("Close"));
        box.setIcon(QMessageBox::Icon::Information);

        box.exec();
        //QMessageBox::information(this,PROGRAM_NAME,tr("The analyze result copied to clipboard."),QMessageBox::Close);
    }
}

//MENU


//SETTINGS MENU

void MainWindow::on_qaIterateDirectoryRecursive_triggered(bool checked)
{
    Settings::instance()->setValue(Settings::IterateDirectoryRecursive,checked);
}

void MainWindow::on_qaCheckUpdates_triggered(bool checked)
{
    Settings::instance()->setValue(Settings::CheckForUpdates,checked);
}

void MainWindow::on_qaSingleInstance_triggered(bool checked)
{
    Settings::instance()->setValue(Settings::SingleInstace,checked);
}

void MainWindow::on_qaAddToWinExplorerContextMenu_triggered(bool checked)
{
    putContextMenu(checked);
    Settings::instance()->setValue(Settings::ContextMenu,checked);
}
//FILE MENU

void MainWindow::on_qaExit_triggered()
{
    qApp->exit();
}

//HELP MENU
void MainWindow::on_qaAbout_triggered()
{
    AboutDialog *ad = new AboutDialog(this);
    ad->show();
}

void MainWindow::on_qaSelectAllItem_triggered()
{
    if(fileListWidget->isFocused()){
        fileListWidget->selectAll();
    }
    else{
        textResult->selectAll();
    }
}

void MainWindow::on_qaCheckForUpdates_triggered()
{
    if(pgUpdater != NULL) return;

    pgUpdater = new QProgressBar(ui->statusBar);
    pgUpdater->setMaximum(0);
    pgUpdater->setValue(0);
    pgUpdater->setMaximumWidth(150);
    pgUpdater->setMaximumHeight(18);
    pgUpdater->setTextVisible(false);

    lblUpdater = new QLabel(tr("Checking for updates"),this);

    ui->statusBar->addWidget(lblUpdater);
    ui->statusBar->addWidget(pgUpdater);

    UpdateChecker *uc = new UpdateChecker();
    connect(uc,SIGNAL(done(QDomDocument)),this,SLOT(updateDone(QDomDocument)));
    connect(uc,SIGNAL(error(int)),this,SLOT(updateError(int)));
}

void MainWindow::updateDone(QDomDocument data){
    ui->statusBar->removeWidget(pgUpdater);
    ui->statusBar->removeWidget(lblUpdater);

    delete pgUpdater;
    pgUpdater = NULL;

    delete lblUpdater;
    lblUpdater = NULL;

    QDomNodeList uaNodes = data.elementsByTagName("updateavailable");
    if(uaNodes.count() == 1 && uaNodes.item(0).toElement().text() == "true"){
        QString yourversion, currentversion;

        uaNodes = data.elementsByTagName("updater").at(0).childNodes();
        for(int i = 0; i < uaNodes.count(); i++){
            if(uaNodes.at(i).nodeName() == "currentversion"){
                currentversion = uaNodes.at(i).toElement().text();
            }
            else if(uaNodes.at(i).nodeName() == "yourversion"){
                yourversion = uaNodes.at(i).toElement().text();
            }
        }

        QMessageBox box;

        box.setWindowTitle(PROGRAM_NAME + " :: " + tr("Updater"));
        box.setText(tr("New version available now!") + "\n" + tr("Your version:") + " " + yourversion + "\n" + tr("New version:") + " " + currentversion + "\n\n" + tr("Do you want update now?") + "\n" + tr("(This program will be closed while updating!)"));
        box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        box.setButtonText(QMessageBox::Yes,tr("Yes"));
        box.setButtonText(QMessageBox::No,tr("No"));
        box.setIcon(QMessageBox::Icon::Question);

        if(box.exec() == QMessageBox::Yes)
        {
#ifdef Q_OS_WIN32
            QString sApp = "\"" + QDir::toNativeSeparators("updater.exe") + "\"";
            QString sParams = "\""+ UpdateChecker::URL + "\"";
            QString sPath = "\"" + QDir::toNativeSeparators(QDir::currentPath()) + "\"";
            // execute with admin rights
            RunElevated(NULL, LPWSTR(sApp.utf16()), LPWSTR(sParams.utf16()), LPWSTR(sPath.utf16()));
#else
            QProcess::startDetached("updater",QStringList() << UpdateChecker::URL);
#endif
            close();
        }

    }
    else{
        if(!startupUpdate){
            QMessageBox box;

            box.setWindowTitle(PROGRAM_NAME + " :: " + tr("Updater"));
            box.setText(tr("No updates available now!"));
            box.setStandardButtons(QMessageBox::Close);
            box.setButtonText(QMessageBox::Close,tr("Close"));
            box.setIcon(QMessageBox::Icon::Information);

            box.exec();
        }
    }

    startupUpdate = false;
}

void MainWindow::updateError(int error){
    ui->statusBar->removeWidget(pgUpdater);
    ui->statusBar->removeWidget(lblUpdater);

    delete pgUpdater;
    pgUpdater = NULL;

    delete lblUpdater;
    lblUpdater = NULL;

    if(!startupUpdate){
        QMessageBox box;

        box.setWindowTitle(PROGRAM_NAME + " :: " + tr("Updater"));
        box.setText(tr("Failed to check updates!") + "\r\n" + tr("Error code:") + QString(" %1").arg(error));
        box.setStandardButtons(QMessageBox::Close);
        box.setButtonText(QMessageBox::Close,tr("Close"));
        box.setIcon(QMessageBox::Icon::Critical);

        box.exec();
    }

    startupUpdate = false;
}

void MainWindow::changeEvent(QEvent *e){
    if(e->type() == QEvent::WindowStateChange){
        Settings::instance()->setValue(Settings::MWMax,this->windowState() == Qt::WindowState::WindowMaximized);
    }
}

void MainWindow::closeEvent(QCloseEvent *event){
    Settings::instance()->setValue(Settings::TBArea,this->toolBarArea(ui->mainToolBar));
    Settings::instance()->setValue(Settings::MWRect,this->geometry());
    #ifdef TEST_DEPRECATED_FUNCS
        Uninstall(lpvState); // Uninstall exception handlers
    #else
        int nUninstRes = crUninstall(); // Uninstall exception handlers
    #ifndef X64
        assert(nUninstRes==0);
        nUninstRes;
    #endif
    #endif //TEST_DEPRECATED_FUNCS
}

void MainWindow::langSetted(){
    foreach(QAction *act,ui->menuLanguage->actions()){
        if(act->isChecked()){
            QString prefix = act->text();
            prefix = prefix.mid(prefix.lastIndexOf("(")+1,2);
            Settings::instance()->setValue(Settings::LangFile,LanguageManager::instance()->getLangFileByPrefix(prefix));
        }
    }

    if(translator != NULL){
        qApp->removeTranslator(translator);
        delete translator;

        QString lang = Settings::instance()->value(Settings::LangFile,"").toString();
        translator = new QTranslator();
        translator->load(lang);
        qApp->installTranslator(translator);

        ui->retranslateUi(this);
        reTranslate();
    }
}

void MainWindow::SingleApplicationHandleMessage(const QString& message)
{
    //files from args
    QStringList files, dirs, args = message.split("\n", QString::SkipEmptyParts);

    foreach(QString file, args){
        QFileInfo fi(file);

        if(fi.exists()){
            QString extension = MediaItem::getExtension(file);

            if(AudioItem::extensionExists(extension) ||
               VideoItem::extensionExists(extension) ||
               fi.isDir())
            {
                if(fi.isFile()){
                    files.append(file);
                }
                else if(fi.isDir()){
                    dirs.append(file);
                }
            }
        }
    }

    foreach(QString dir, dirs){
        FileProcessDialog fpd(dir, this);
        QObject::connect(&fpd,SIGNAL(currentFile(QString &)),this,SLOT(addMediaItem(QString &)));
        fpd.setWindowTitle(tr("Searching files..."));
        fpd.exec();
    }

    if(files.count() > 0){
        FileProcessDialog fpd(files, this);
        QObject::connect(&fpd, SIGNAL(currentFile(QString &)), this, SLOT(addMediaItem(QString &)));
        fpd.setWindowTitle(tr("Adding files..."));
        fpd.exec();
    }
}

void MainWindow::putContextMenu(bool set){
#ifdef Q_OS_WIN32
    QSettings s("HKEY_CURRENT_USER\\Software\\Classes\\SystemFileAssociations",QSettings::NativeFormat);
    QString appPath = QDir::toNativeSeparators(qApp->applicationFilePath());
    foreach(QString ext, VideoItem::extensions){
        if(set){
            s.beginGroup(ext);
            s.beginGroup("Shell");
            s.beginGroup("MediaInfo");
            s.setValue("Icon","\"" + appPath + "\"");
            s.beginGroup("Command");
            s.setValue("","\"" + appPath + "\" \"%1\"");
            s.endGroup();
            s.endGroup();
            s.endGroup();
            s.endGroup();
        }
        else{
            s.beginGroup(ext);
            s.remove("Shell");
            s.endGroup();
        }
    }

    foreach(QString ext, AudioItem::extensions){
        if(set){
            s.beginGroup(ext);
            s.beginGroup("Shell");
            s.beginGroup("MediaInfo");
            s.setValue("Icon","\"" + QDir::toNativeSeparators(qApp->applicationFilePath()) + "\"");
            s.beginGroup("Command");
            s.setValue("","\"" + QDir::toNativeSeparators(qApp->applicationFilePath()) + "\" \"%1\"");
            s.endGroup();
            s.endGroup();
            s.endGroup();
            s.endGroup();
        }
        else{
            s.beginGroup(ext);
            s.remove("Shell");
            s.endGroup();
        }
    }


    QSettings s2("HKEY_CURRENT_USER\\Software\\Classes",QSettings::NativeFormat);
    if(set){
        s2.beginGroup("Directory");
        s2.beginGroup("Shell");
        s2.beginGroup("MediaInfo");
        s2.setValue("Icon","\"" + QDir::toNativeSeparators(qApp->applicationFilePath()) + "\"");
        s2.beginGroup("Command");
        s2.setValue("","\"" + QDir::toNativeSeparators(qApp->applicationFilePath()) + "\" \"%L\"");
        s2.endGroup();
        s2.endGroup();
        s2.endGroup();
        s2.endGroup();
    }
    else{
        s2.beginGroup("Directory");
        s2.remove("Shell");
        s2.endGroup();
    }

#endif
}

void MainWindow::reTranslate(){
    lblFormats->setText(tr("Output format:") + " ");

    cbFormats->clear();

    cbFormats->addItem(ui->qaFormatNone->text());
    cbFormats->addItem(ui->qaFormatBB->text());
    cbFormats->addItem(ui->qaFormatHtml->text());

    int c = 0;
    foreach(QAction *lng, ui->menuLanguage->actions()){
        if(c > 0){
            QString *prefix = new QString();
            lng->setText( LanguageManager::getLanguageNameByPrefix(lng->text(),prefix) + " (" + *prefix + ")");
            delete prefix;
        }
        c++;
    }
}

#ifdef Q_OS_WIN32
// special function used to execute the updater with administrator rights
BOOL RunElevated(HWND hwnd, LPCTSTR pszPath, LPCTSTR pszParameters, LPCTSTR pszDirectory)
{
    QString sVerb = isWindowsVista() ? "runas" : "";

    SHELLEXECUTEINFO shex;

    memset(&shex, 0, sizeof(shex));

    shex.cbSize       = sizeof(SHELLEXECUTEINFO);
    shex.fMask        = 0;
    shex.hwnd         = hwnd;
    shex.lpVerb       = LPWSTR(sVerb.utf16());
    shex.lpFile       = pszPath;
    shex.lpParameters = pszParameters;
    shex.lpDirectory  = pszDirectory;
    shex.nShow        = SW_SHOW;

    return ::ShellExecuteEx(&shex);
}
#endif

bool isWindowsVista()
{
#ifdef Q_OS_WIN32
    OSVERSIONINFO osvi;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&osvi);
    // return if we are running the windows vista/7 or higher OS
    return (osvi.dwMajorVersion >= 6);// && (osvi.dwMinorVersion == 0));
#else
    return false;
#endif
}
