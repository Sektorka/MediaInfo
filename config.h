#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <QTranslator>

#ifdef X64
const QString PROGRAM_NAME = "MediaInfo x64";
#elif
const QString PROGRAM_NAME = "MediaInfo";
#endif
const QString VERSION = "1.0.0.4";
const QString AUTHOR = "Sektor";

QTranslator *translator = NULL;

#endif // CONFIG_H
