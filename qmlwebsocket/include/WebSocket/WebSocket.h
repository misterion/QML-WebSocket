/*
  Copyright (c) 2012, Nikolay Bondarenko <misterionkell@gmail.com>  
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
  * Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.
  * Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.
  * Neither the name of the WebSocket++ Project nor the
  names of its contributors may be used to endorse or promote products
  derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
  ARE DISCLAIMED. IN NO EVENT SHALL PETER THORSON BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#pragma once 

#include <WebSocket/Global.h>
#include <QWSSocket.h>


#ifdef HAVE_QT5
  #include <QtQml/QQmlEngine>
  #include <QtQuick/QQuickView>
#else
  #include <QtDeclarative/QDeclarativeItem>
#endif

class WebSocketWrapper;

/*!
  \class WebSocket

  \brief A QML Qt wrapper for websocket++'s boost::asio-based websockets.
*/
class QMLWEBSOCKET_EXPORT WebSocket
#ifdef HAVE_QT5
    : public QQuickItem
#else
    : public QDeclarativeItem
#endif
{
  Q_OBJECT
  Q_ENUMS(SocketState)
  Q_PROPERTY(SocketState socketState READ socketState NOTIFY socketStateChanged)
#ifdef HAVE_QT5
  Q_PLUGIN_METADATA(IID "WebSocket")
#endif
public:
  enum SocketState {
    Unknown = 0,
    Unconnected,
    HostLookup,
    Connecting,
    Connected,
    Bound,
    Closing,
    Listening
  };

#ifdef HAVE_QT5
  explicit WebSocket(QQuickItem *parent = 0);
#else
  explicit WebSocket(QDeclarativeItem *parent = 0);
#endif
  virtual ~WebSocket();
  
  Q_INVOKABLE void connect(const QString &uri);
  Q_INVOKABLE void disconnect();
  Q_INVOKABLE void send(const QString& message);

signals:
  void message(const QString& message);
  void connected();
  void opened(); //depricated
  void disconnected();
  void closed();//depricated
  void failed();

  void socketStateChanged();

protected slots:
  void stateChanged(QAbstractSocket::SocketState socketState);

private:
  SocketState socketState();

private:
  Q_DISABLE_COPY(WebSocket)
  QWsSocket *_wsSocket;
  SocketState _state;
};
