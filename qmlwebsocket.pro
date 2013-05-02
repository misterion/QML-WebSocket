_boostRoot=$$(BOOSTROOT)
isEmpty(_boostRoot){
  !build_pass:error(Please set the environment variable `BOOSTROOT`. For example BOOSTROOT=c:\boost_1_53_0)
}

TEMPLATE = lib
QT += core gui declarative
CONFIG += dll

QMAKE_CFLAGS_RELEASE = -MD
QMAKE_CFLAGS_DEBUG = -MDd

CONFIG(debug, debug|release) {
    TARGET = qmlwebsocket
    DESTDIR = $$_PRO_FILE_PWD_/!build/qmlwebsocket/debug
    OBJECTS_DIR = ../../!obj/qmlwebsocket/debug
    MOC_DIR = ../../!obj/qmlwebsocket/debug
}

CONFIG(release, debug|release) {
    TARGET = qmlwebsocket
    DESTDIR = $$_PRO_FILE_PWD_/!build/qmlwebsocket/release
    OBJECTS_DIR = ../../!obj/qmlwebsocket/release
    MOC_DIR = ../../!obj/qmlwebsocket/release
}

DEFINES += \
    QMLWEBSOCKET_LIB \
    _WIN32_WINNT=0x0600 \
    WIN32

TARGET = $$qtLibraryTarget($$TARGET)
uri = com.mycompany.qmlcomponents

INCLUDEPATH += \
    $$_boostRoot \
    $$_PRO_FILE_PWD_/qmlwebsocket/include \
    "./vendor/websocketpp/src"

win32 {
    isEmpty(QT_DEBUG) {
        _websocketppLib = websocketpp
    } else {
        _websocketppLib = websocketppd
    }
}

LIBS += \
    -L$$_boostRoot/stage/lib \
    -l$$_PRO_FILE_PWD_/vendor/websocketpp/lib/$$_websocketppLib

# Input
SOURCES += \
    qmlwebsocket/src/WebSocketWrapper.cpp \
    qmlwebsocket/src/WebSocket.cpp \
    qmlwebsocket/src/QmlWebSocket.cpp

HEADERS += \
    qmlwebsocket/include/WebSocket/WebSocketWrapper.h \
    qmlwebsocket/include/WebSocket/WebSocket.h \
    qmlwebsocket/include/WebSocket/QmlWebSocket.h \
    qmlwebsocket/include/WebSocket/Global.h

OTHER_FILES = qmldir

!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$OUT_PWD/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) \"$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)\" \"$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)\"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

qmldir.files = qmldir
unix {
    maemo5 | !isEmpty(MEEGO_VERSION_MAJOR) {
        installPath = /usr/lib/qt4/imports/$$replace(uri, \\., /)
    } else {
        installPath = $$[QT_INSTALL_IMPORTS]/$$replace(uri, \\., /)
    }
    qmldir.path = $$installPath
    target.path = $$installPath
    INSTALLS += target qmldir
}

