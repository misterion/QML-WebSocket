QML-WebSocket SocketIO client
=============

This is a lightweight Qt Qml SocketIO client based on Qt QtWebSocket library.

It is designed for embedding and using directly in a Qt Qml application.

Preparation for build
=============

Download the source code for the vendor part, it is added to the project as a git submodule.
	git submodule init
	git submodule update
 
        cd vendor/websocketpp

Set the system env variable QTDIR - on the folder in which you laden with Qt SDK.
For example, in my case
	QTDIR = C:\SDK\QT\4.8.4

Environment variable QTDIR allow generate qmltypes and copy the files in a directory for Import Qt Sdk.

Building
=============

The assembly of this project consists of three parts - build libraries qtwebsocket, extension for qml
and finally up the libraries in your project and imports for QtCreator (earned autocomplete to the editor QML).

Use visual studio solution or qtcreator pro project for build library.

How to use
=============

Look at example folder. You may find simple echo server and client.

(C) 2012 - 2013 Nikolay Bondarenko <misterionkell@gmail.com>