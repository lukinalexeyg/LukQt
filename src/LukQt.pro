TEMPLATE = lib

QT += core

TARGET = LukQt

VERSION = 1.0.0.0

CONFIG += skip_target_version_ext

DEFINES += LUKQT_EXPORT

# Windows and Unix get the suffix "d" to indicate a debug version of the library.
# Mac OS gets the suffix "_debug".
CONFIG(debug, debug|release) {
    win32:      TARGET = $$join(TARGET,,,d)
    mac:        TARGET = $$join(TARGET,,,_debug)
    unix:!mac:  TARGET = $$join(TARGET,,,d)
}

include(LukQt.pri)

DESTDIR = bin
MOC_DIR = moc
OBJECTS_DIR = obj
