#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QString>

class Settings : public QSettings
{
    Q_OBJECT
public:
    static Settings *instance(QObject *parent = 0);

    static const QString IterateDirectoryRecursive;
    static const QString Format;
    static const QString CheckForUpdates;
    static const QString MWMax;
    static const QString TBArea;
    static const QString SingleInstace;
    static const QString LangFile;
    static const QString ContextMenu;
    static const QString AddMediaFileOpenDialogDir;
    static const QString AddFolderOpenDialogDir;
    static const QString ResultFileSaveDialogDir;
    static const QString MWRect;

private:
    explicit Settings(QObject *parent = 0);
    static Settings *settings;
    static const QString file;
    static const QString dir;
    
signals:
    
public slots:
    
};

#endif // SETTINGS_H
