QML-WebSocket + QML SocketIO client
=============

This is a lightweight Qt Qml wrapper for legacy version of websocketpp++ by zaphoyd.
The main reason is #105 issue in zaphoyd websocketpp++ and related issue #90 (the master branch 
code of websocketpp++ crashes in qt wrapper by Leo Franchi). Both issues stil 
active for a 6 last months. So i turn wrapper to use legace 0.1 version of websocket++.

Also it is designed for embedding and using directly in a Qt Qml application.

(c) 2012 Nikolay Bondarenko <misterionkell@gmail.com>
