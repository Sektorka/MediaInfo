#include "mainwindow.h"
#include <QtSingleApplication>
#include <QFileInfo>
#include <CrashRpt.h>
#include <assert.h>
#include <process.h>
#include <tchar.h>
#include "settings.h"
#include "languagemanager.h"
#include <QTranslator>

int main(int argc, char *argv[])
{
    CR_INSTALL_INFO info;
    memset(&info, 0, sizeof(CR_INSTALL_INFO));
    info.cb = sizeof(CR_INSTALL_INFO);
    info.pszAppName = LPWSTR(PROGRAM_NAME.utf16());
    info.pszAppVersion = LPWSTR(VERSION.utf16());
    info.pszUrl = _T("http://mediainfo.sektor.hu/crashreport.php");
    info.dwFlags |= CR_INST_ALL_POSSIBLE_HANDLERS;
    info.pszPrivacyPolicyURL = _T("http://mediainfo.sektor.hu/privacy.php");

    int nResult = crInstall(&info);
    if(nResult == 0)
    {
        crAddScreenshot2(CR_AS_MAIN_WINDOW|CR_AS_USE_JPEG_FORMAT, 95);
    }
    else{
        //TCHAR buff[256];
        //crGetLastErrorMsg(buff, 256);
        //MessageBox(NULL, buff, (PROGRAM_NAME.toStdWString() + _T(" :: Crash handler")).c_str(), MB_OK | MB_ICONWARNING);
        //return -1;
    }

    qRegisterMetaType<QDomDocument>("QDomDocument");

    QtSingleApplication a(PROGRAM_NAME + VERSION, argc, argv);

    QDir::setCurrent(a.arguments().at(0).mid(0,a.arguments().at(0).lastIndexOf("\\")));

    QLocale loci = QLocale::system();
    QString syslang = loci.name().mid(0,loci.name().indexOf("_"));
    QString lang = Settings::instance()->value(Settings::LangFile, LanguageManager::instance()->getLangFileByPrefix(syslang)).toString();

    translator = new QTranslator();
    translator->load(lang);
    a.installTranslator(translator);

    QString URLs;
    foreach(QString arg, a.arguments()){
        URLs += arg + "\n";
    }

    bool singleInstance = Settings::instance()->value(Settings::SingleInstace,true).toBool();
    if (singleInstance && a.sendMessage(URLs)) return 0;

    MainWindow w;
    w.show();

    // set instance message handler
    QObject::connect(&a, SIGNAL(messageReceived(const QString&)), &w, SLOT(SingleApplicationHandleMessage(const QString&)));

    a.setActivationWindow(&w, true);

    if (!URLs.isEmpty()){
        w.SingleApplicationHandleMessage(URLs);
    }

    return a.exec();
}
