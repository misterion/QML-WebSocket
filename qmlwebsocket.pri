SOURCES += \
    qmlwebsocket/src/WebSocket.cpp \
    qmlwebsocket/src/QmlWebSocket.cpp \
    vendor/QtWebSocket/QtWebsocket/QWsSocket.cpp \
    vendor/QtWebSocket/QtWebsocket/QWsServer.cpp

HEADERS += \
    qmlwebsocket/include/WebSocket/WebSocket.h \
    qmlwebsocket/include/WebSocket/QmlWebSocket.h \
    qmlwebsocket/include/WebSocket/Global.h \
    vendor/QtWebSocket/QtWebsocket/QWsSocket.h \
    vendor/QtWebSocket/QtWebsocket/QWsServer.h

OTHER_FILES = qmldir

