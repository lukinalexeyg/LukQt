INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/lapputils.h \
    $$PWD/lchars.h \
    $$PWD/lcolorutils.h \
    $$PWD/lcrashhandler.h \
    $$PWD/lfileutils.h \
    $$PWD/lftp.h \
    $$PWD/llog.h \
    $$PWD/lmacros.h \
    $$PWD/lnetworkutils.h \
    $$PWD/lnumberutils.h \
    $$PWD/lregistry.h \
    $$PWD/lsettings.h \
    $$PWD/lstringutils.h \
    $$PWD/lukqtglobal.h \
    $$PWD/lworker.h

SOURCES += \
    $$PWD/lapputils.cpp \
    $$PWD/lcolorutils.cpp \
    $$PWD/lcrashhandler.cpp \
    $$PWD/lfileutils.cpp \
    $$PWD/lftp.cpp \
    $$PWD/llog.cpp \
    $$PWD/lnetworkutils.cpp \
    $$PWD/lnumberutils.cpp \
    $$PWD/lregistry.cpp \
    $$PWD/lsettings.cpp \
    $$PWD/lstringutils.cpp \
    $$PWD/lworker.cpp

win32-msvc* {
    LIBS += -lAdvapi32
    LIBS += -lDbgHelp
}
