#-------------------------------------------------
#
# Project created by QtCreator 2013-06-27T10:16:07
#
#-------------------------------------------------

QT       += core gui xml network

#message(asd: $$QMAKE_TARGET.arch)

CONFIG += qt \
    warn_on \
    debug_and_release \
    static_build \
    $$[build_mode]

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MediaInfo
TEMPLATE = app

DEFINES += NOMINMAX
DEFINES += THREAD_SLEEP=5 #miliseconds
DEFINES += X64

static_build {
    CONFIG += STATIC
    DEFINES += MI_STATIC
    message(Building Static version)
}

SOURCES += main.cpp\
        mainwindow.cpp \
        mediaitem.cpp \
    fileprocessthread.cpp \
    fileprocessdialog.cpp \
    audioitem.cpp \
    videoitem.cpp \
    mediamanager.cpp \
    settings.cpp \
    aboutdialog.cpp \
    filelistwidget.cpp \
    updatechecker.cpp \
    languagemanager.cpp

TRANSLATIONS = translations/MediaInfo_hu.ts

HEADERS  += mainwindow.h \
        mediaitem.h \
    fileprocessthread.h \
    fileprocessdialog.h \
    audioitem.h \
    videoitem.h \
    mediamanager.h \
    settings.h \
    aboutdialog.h \
    filelistwidget.h \
    updatechecker.h \
    config.h \
    languagemanager.h \
    mi.h

FORMS    += mainwindow.ui \
    fileprocessdialog.ui \
    aboutdialog.ui

RESOURCES += \
    resources.qrc

win32 {
    RC_FILE += mediainfo.rc
    LIBS += -lshell32
}

INCLUDEPATH += D:/Sektor/Develop/Projects/QtProjects/MediaInfo/libmediainfo/include
DEPENDPATH += D:/Sektor/Develop/Projects/QtProjects/MediaInfo/libmediainfo/include

contains(DEFINES, MI_STATIC) {
    QMAKE_LFLAGS_RELEASE += /FORCE:MULTIPLE /NODEFAULTLIB:MSVCRT.lib /DEFAULTLIB:LIBCMT.lib

    contains(DEFINES, X64) {
        win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libmediainfo/lib/x64/static/ -lCrashRptLIB # -lMediaInfo -lzlib -lZenLib
        else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libmediainfo/lib/x64/static/ -lCrashRptLIBd # -lMediaInfod -lzlibd -lZenLibd
        else:unix: LIBS += -L$$PWD/libmediainfo/lib/x64/static/ -lCrashRptLIB # -lMediaInfo-lzlib -lZenLib

        #win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/libmediainfo/lib/x64/static/zlib.lib
        #else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/libmediainfo/lib/x64/static/zlibd.lib
        #else:unix: PRE_TARGETDEPS += $$PWD/libmediainfo/lib/x64/static/libzlib.a

        #win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/libmediainfo/lib/x64/static/ZenLib.lib
        #else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/libmediainfo/lib/x64/static/ZenLibd.lib
        #else:unix: PRE_TARGETDEPS += $$PWD/libmediainfo/lib/x64/static/libZenLib.a

        #win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/libmediainfo/lib/x64/static/MediaInfo.lib
        #else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/libmediainfo/lib/x64/static/MediaInfod.lib
        #else:unix: PRE_TARGETDEPS += $$PWD/libmediainfo/lib/x64/static/libMediaInfo.a

        win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/libmediainfo/lib/x64/static/CrashRptLIB.lib
        else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/libmediainfo/lib/x64/static/CrashRptLIBd.lib
        else:unix: PRE_TARGETDEPS += $$PWD/libmediainfo/lib/x64/static/libCrashRptLIB.a
    }
    else{
        win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libmediainfo/lib/x86/static/ -lCrashRptLIB # -lMediaInfo -lzlib -lZenLib
        else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libmediainfo/lib/x86/static/ -lCrashRptLIBd # -lMediaInfod -lzlibd -lZenLibd
        else:unix: LIBS += -L$$PWD/libmediainfo/lib/x86/static/ -lCrashRptLIB # -lMediaInfo -lzlib -lZenLib

        #win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/libmediainfo/lib/x86/static/zlib.lib
        #else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/libmediainfo/lib/x86/static/zlibd.lib
        #else:unix: PRE_TARGETDEPS += $$PWD/libmediainfo/lib/x86/static/libzlib.a

        #win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/libmediainfo/lib/x86/static/ZenLib.lib
        #else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/libmediainfo/lib/x86/static/ZenLibd.lib
        #else:unix: PRE_TARGETDEPS += $$PWD/libmediainfo/lib/x86/static/libZenLib.a

        #win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/libmediainfo/lib/x86/static/MediaInfo.lib
        #else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/libmediainfo/lib/x86/static/MediaInfod.lib
        #else:unix: PRE_TARGETDEPS += $$PWD/libmediainfo/lib/x86/static/libMediaInfo.a

        win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/libmediainfo/lib/x86/static/CrashRptLIB.lib
        else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/libmediainfo/lib/x86/static/CrashRptLIBd.lib
        else:unix: PRE_TARGETDEPS += $$PWD/libmediainfo/lib/x86/static/libCrashRptLIB.a
    }


} else {
    QMAKE_LFLAGS_RELEASE += /FORCE:MULTIPLE

    contains(DEFINES, X64) {
        win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libmediainfo/lib/x64/dynamic/ -lCrashRpt
        else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libmediainfo/lib/x64/dynamic/ -lCrashRptd
        else:unix: LIBS += -L$$PWD/libmediainfo/lib/x64/dynamic/ -lCrashRpt

        win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/libmediainfo/lib/x64/dynamic/CrashRpt.lib
        else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/libmediainfo/lib/x64/dynamic/CrashRptd.lib
        else:unix: PRE_TARGETDEPS += $$PWD/libmediainfo/lib/x64/dynamic/libCrashRpt.a
    }
    else{
        win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libmediainfo/lib/x86/dynamic/ -lCrashRpt
        else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libmediainfo/lib/x86/dynamic/ -lCrashRptd
        else:unix: LIBS += -L$$PWD/libmediainfo/lib/x86/dynamic/ -lCrashRpt

        win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/libmediainfo/lib/x86/dynamic/CrashRpt.lib
        else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/libmediainfo/lib/x86/dynamic/CrashRptd.lib
        else:unix: PRE_TARGETDEPS += $$PWD/libmediainfo/lib/x86/dynamic/libCrashRpt.a
    }

}

include(qtsingleapplication/qtsingleapplication.pri)
