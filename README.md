QML-WebSocket + QML SocketIO client
=============

This is a lightweight Qt Qml wrapper for legacy version of websocketpp + + by zaphoyd.

The main reason is # 105 issue in zaphoyd websocketpp + + and related issue # 90 (the master branch
code of websocketpp + + crashes in qt wrapper by Leo Franchi). Both issues stil
active for a 6 last months. So i turn wrapper to use legace 0.1 version of websocket + +.

Also it is designed for embedding and using directly in a Qt Qml application.

WebSocketWrapper based on originals from Leo Franchi <lfranchi@kde.org>

Preparation for build
=============

Download the source code for the vendor/websocketpp, it is added to the project as a git submodule.
	git submodule init
	git submodule update
 
        cd vendor/websocketpp
        git checkout legacy

Download and assemble boost (http://www.boost.org/).
To build with visual studio

	Download Boost from http://www.boost.org/

	Unzip boost_1_53_1.zip to C:\SDK\boost_1_53_1

	Open a "Visual Studio Command Prompt (2010)" (not a regular cmd.exe!).

	cd C:\SDK\boost_1_53_1

        bootstrap

        .\B2 --prefix=C:\SDK\boost_1_53_1 --build-type =commplete -j 4
Now set a system environment variable:
	BOOSTROOT = C: \ SDK \ boost_1_47_0

If the environment variable is not set BOOSTROOT - the project will not meet.

Set the variable Cerda otkruzheniya QTDIR - on the folder in which you laden with Qt SDK.
For example, in my case
	QTDIR = C:\SDK\QT\4.8.4

Environment variable QTDIR allow generate qmltypes and copy the files in a directory for Import Qt Sdk.

Building
=============

The assembly of this project consists of three parts - build libraries websocketp, extension for qml
and finally up the libraries in your project and imports for QtCreator (earned autocomplete to the editor QML).

Step 1.

If you build a project in visual studio, on the instructions located in the project websocketpp you must change
Code Generation / Runtime Library on MD (MDd for debug build). Just change the Output for the debug version on
websocketppd. After assembly, copy the files to vendor/websocketpp/lib.

If you build a project from QtCreator just open and build the project websocketpp.pro. Perform (or enter into
project settings) qmake install to copy the resulting library vendor/websocketpp/lib.

Step 2.

If you build a project in visual studio, just open qmlwebsocket.sln and build the project.
If you build a project from QtCreator just open and build the project qmlwebsocket.pro.


How to use
=============

Look at example folder. You may find simple echo server and client.

(C) 2012 Nikolay Bondarenko <misterionkell@gmail.com>