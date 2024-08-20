INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/lapputils.h \
    $$PWD/lchars.h \
    $$PWD/lcrashhandler.h \
    $$PWD/lfileutils.h \
    $$PWD/llog.h \
    $$PWD/lmacros.h \
    $$PWD/lnumberutils.h \
    $$PWD/lpath.h \
    $$PWD/lsettings.h \
    $$PWD/lstringutils.h \
    $$PWD/lukqtglobal.h \
    $$PWD/lworker.h

SOURCES += \
    $$PWD/lapputils.cpp \
    $$PWD/lcrashhandler.cpp \
    $$PWD/lfileutils.cpp \
    $$PWD/lnumberutils.cpp \
    $$PWD/lpath.cpp \
    $$PWD/lsettings.cpp \
    $$PWD/lstringutils.cpp \
    $$PWD/lworker.cpp

win32-msvc* {
    LIBS += -lAdvapi32
    LIBS += -lDbgHelp
}
