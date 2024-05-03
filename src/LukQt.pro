TEMPLATE = lib

QT += core network

TARGET = LukQt

VERSION = 1.0.0.0

CONFIG += skip_target_version_ext

DEFINES += LUKQT_EXPORT

include(LukQt.pri)

DESTDIR = bin
MOC_DIR = moc
OBJECTS_DIR = obj

win32-msvc* {
    QMAKE_EXTRA_TARGETS += before_build makefilehook
    makefilehook.target = $(MAKEFILE)
    makefilehook.depends = .beforebuild
    PRE_TARGETDEPS += .beforebuild
    before_build.target = .beforebuild
    before_build.depends = FORCE
    before_build.commands = chcp 1251
}
