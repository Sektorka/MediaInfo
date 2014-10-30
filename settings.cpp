#include "settings.h"
#include "config.h"
#include <stdlib.h>
#include <QDir>

QString d = QDir::toNativeSeparators(QString(getenv("APPDATA")) + "/" + PROGRAM_NAME + "/");

const QString Settings::dir = (QDir(d).exists() ? d + "/" : QDir("C:/").exists() ? "C:/" : QDir::currentPath() + "/");
const QString Settings::file = PROGRAM_NAME + ".conf";
const QString Settings::IterateDirectoryRecursive = "IterateDirectoryRecursive";
const QString Settings::Format = "Format";
const QString Settings::CheckForUpdates = "CheckForUpdates";
const QString Settings::MWMax = "MWMax";
const QString Settings::TBArea = "TBArea";
const QString Settings::SingleInstace = "SingleInstace";
const QString Settings::LangFile = "LangFile";
const QString Settings::ContextMenu = "ContextMenu";
const QString Settings::AddMediaFileOpenDialogDir = "AddMediaFileOpenDialogDir";
const QString Settings::AddFolderOpenDialogDir = "AddFolderOpenDialogDir";
const QString Settings::ResultFileSaveDialogDir = "ResultFileSaveDialogDir";
const QString Settings::MWRect = "MWRect";

Settings *Settings::settings = NULL;

Settings::Settings(QObject *parent) :
    QSettings(dir + file, QSettings::IniFormat, parent)
{
}

Settings *Settings::instance(QObject *parent){
    if(settings == NULL){
        settings = new Settings(parent);
    }

    return settings;
}
