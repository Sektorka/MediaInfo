#ifndef MediaItem_H
#define MediaItem_H

#include <QObject>
#include <QtXml/QDomDocument>
#include <QList>
#include <QStringList>
#include <QString>
#include <QtCore/qmath.h>
#include <QDir>
#include "settings.h"
#include "mi.h"

class MediaItem : public QObject, public MediaInfo
{
    Q_OBJECT
public:
    friend class FileProcessThread;

    enum Format{ NONE = 0, BB, HTML };
    explicit MediaItem(QString &filePath, QObject *parent = 0);
    ~MediaItem();

    QString getFilePath() const;
    QString getFileName() const;
    QString getAnalyzeResult();

    static QString formatSeconds(long sec);
    static QString formatBytes(long bytes);
    static QString formatBits(long bits);
    static QString formatRate(long rate);
    static QString getExtension(QString path);

protected:
    void analize();
    QDomDocument *data;
    QString filePath;

    virtual QString getNonFormattedResult() = 0;
    virtual QString getBBFormattedResult() = 0;
    virtual QString getHtmlFormattedResult() = 0;

private:
    bool analyzed;
    
signals:
    
public slots:
    
};

#endif // MediaItem_H
