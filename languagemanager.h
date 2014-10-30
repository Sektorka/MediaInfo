#ifndef LANGUAGEMANAGER_H
#define LANGUAGEMANAGER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QList>
#include <QFileInfo>
#include "config.h"

class LanguageManager : public QObject
{
    Q_OBJECT
public:
    static LanguageManager *instance();
    static QString getLanguageNameByPrefix(QString &, QString *pref = NULL);
    const QStringList &getLangFiles() const;
    const QString getLangFileByPrefix(QString &prefix) const;
    const bool exists(QString &) const;
    static const QString ext;
    
signals:
    
public slots:
    
private:
    explicit LanguageManager(QObject *parent = 0);
    static const QString dir;
    static LanguageManager *lm;

    QStringList langs;
};

#endif // LANGUAGEMANAGER_H
