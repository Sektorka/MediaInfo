#include "languagemanager.h"

const QString LanguageManager::ext = ".qm";
const QString LanguageManager::dir = "languages";
LanguageManager *LanguageManager::lm = NULL;


LanguageManager::LanguageManager(QObject *parent) :
    QObject(parent)
{
    QStringList languageFiles;
    QDir languagesDir(dir);

    languageFiles = languagesDir.entryList(QDir::Files, QDir::Name);
    languageFiles = languageFiles.filter(ext);

    foreach (QString langFile, languageFiles){
        if (langFile.endsWith(ext)){
            langs.append(QDir::toNativeSeparators(dir + "/" + langFile));
        }
    }

}

LanguageManager *LanguageManager::instance(){
    if(lm == NULL){
        lm = new LanguageManager();
    }

    return lm;
}

const QStringList &LanguageManager::getLangFiles() const{
    return this->langs;
}

const QString LanguageManager::getLangFileByPrefix(QString &prefix) const{
    QString path = QDir::toNativeSeparators(dir + "/" + PROGRAM_NAME + "_" + prefix + ext);
    return (LanguageManager::instance()->exists(path) ? path : "");
}

QString LanguageManager::getLanguageNameByPrefix(QString &prefix, QString *pref){
    if(prefix.lastIndexOf("_") != -1){
        prefix = prefix.mid(prefix.lastIndexOf("_")+1,2);
    }

    if(prefix.lastIndexOf("(") != -1 && prefix.lastIndexOf(")") != -1){
        prefix = prefix.mid(prefix.lastIndexOf("(")+1,2);
    }

    if(pref != NULL){
        *pref = prefix;
    }

    if(prefix == "en"){
        return tr("English");
    }
    else if(prefix == "hu"){
        return tr("Hungarian");
    }
    else{
        return "";
    }
}

const bool LanguageManager::exists(QString &lang) const{
    foreach(QString lng, langs){
        if(lng == lang){
            QFileInfo info(lang);
            if(info.exists() && info.isFile()){
                return true;
            }
        }
    }

    return false;
}
