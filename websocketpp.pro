_boostRoot=$$(BOOSTROOT)
isEmpty(_boostRoot){
  !build_pass:error(Please set the environment variable `BOOSTROOT`. For example BOOSTROOT=c:\boost_1_53_0)
}

TEMPLATE = lib
QT -= core gui
CONFIG += staticlib

QMAKE_CFLAGS_RELEASE = -MD
QMAKE_CFLAGS_DEBUG = -MDd

CONFIG(debug, debug|release) {
    TARGET = websocketpp
    DESTDIR = $$_PRO_FILE_PWD_/!build/websocketpp/debug
    OBJECTS_DIR = $$_PRO_FILE_PWD_/!obj/websocketpp/debug
    MOC_DIR = $$_PRO_FILE_PWD_/!obj/websocketpp/debug
}

CONFIG(release, debug|release) {
    TARGET = websocketpp
    DESTDIR = $$_PRO_FILE_PWD_/!build/websocketpp/release
    OBJECTS_DIR = $$_PRO_FILE_PWD_/!obj/websocketpp/release
    MOC_DIR = $$_PRO_FILE_PWD_/!obj/websocketpp/release
}

DEFINES += \
    _LIB \
    _WIN32_WINNT=0x0600 \
    NOCOMM \
    WIN32

TARGET = $$qtLibraryTarget($$TARGET)

INCLUDEPATH += \
    $$_boostRoot

LIBS += \
    -L$$_boostRoot/stage/lib \

libs.path = $$_PRO_FILE_PWD_/vendor/websocketpp/lib
libs.files += $$DESTDIR/*.*
INSTALLS += libs

# Input
OTHER_FILES = qmldir \
    vendor/websocketpp/src/sha1/Makefile.nt \
    vendor/websocketpp/src/sha1/Makefile \
    vendor/websocketpp/src/sha1/license.txt

HEADERS += \
    vendor/websocketpp/src/websocketpp.hpp \
    vendor/websocketpp/src/websocket_session.hpp \
    vendor/websocketpp/src/websocket_server_session.hpp \
    vendor/websocketpp/src/websocket_server.hpp \
    vendor/websocketpp/src/websocket_frame.hpp \
    vendor/websocketpp/src/websocket_endpoint.hpp \
    vendor/websocketpp/src/websocket_connection_handler.hpp \
    vendor/websocketpp/src/websocket_client_session.hpp \
    vendor/websocketpp/src/websocket_client.hpp \
    vendor/websocketpp/src/network_utilities.hpp \
    vendor/websocketpp/src/base64/base64.h \
    vendor/websocketpp/src/sha1/sha1.h \
    vendor/websocketpp/src/utf8_validator/utf8_validator.hpp

SOURCES += \
    vendor/websocketpp/src/websocket_session.cpp \
    vendor/websocketpp/src/websocket_server_session.cpp \
    vendor/websocketpp/src/websocket_server.cpp \
    vendor/websocketpp/src/websocket_frame.cpp \
    vendor/websocketpp/src/websocket_client_session.cpp \
    vendor/websocketpp/src/websocket_client.cpp \
    vendor/websocketpp/src/network_utilities.cpp \
    vendor/websocketpp/src/base64/base64.cpp \
    vendor/websocketpp/src/sha1/shatest.cpp \
    vendor/websocketpp/src/sha1/shacmp.cpp \
    vendor/websocketpp/src/sha1/sha1.cpp \
    vendor/websocketpp/src/sha1/sha.cpp

