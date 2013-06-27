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
#include <WebSocket/WebSocket.h>

#ifdef HAVE_QT5
WebSocket::WebSocket(QQuickItem *parent /*= 0*/)
    : QQuickItem(parent)
#else
WebSocket::WebSocket(QDeclarativeItem *parent /*= 0*/)
    : QDeclarativeItem(parent)
#endif
{
    this->_wsSocket = new QWsSocket(this);
    Q_ASSERT(QObject::connect(this->_wsSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(socketStateChanged(QAbstractSocket::SocketState))));
    Q_ASSERT(QObject::connect(this->_wsSocket, SIGNAL(frameReceived(QString)), this, SIGNAL(message(const QString&))));


    Q_ASSERT(QObject::connect(this->_wsSocket, SIGNAL(connected()), this, SIGNAL(connected())));
    Q_ASSERT(QObject::connect(this->_wsSocket, SIGNAL(connected()), this, SIGNAL(opened())));
    Q_ASSERT(QObject::connect(this->_wsSocket, SIGNAL(disconnected()), this, SIGNAL(disconnected())));
    Q_ASSERT(QObject::connect(this->_wsSocket, SIGNAL(disconnected()), this, SIGNAL(closed())));
}

WebSocket::~WebSocket()
{
}

void WebSocket::connect(const QString &uri)
{
  this->_wsSocket->connectToHost(QUrl(uri));
}


void WebSocket::disconnect()
{
  this->_wsSocket->disconnectFromHost();
}

void WebSocket::send(const QString& message)
{
  this->_wsSocket->write(message);
}

WebSocket::SocketState WebSocket::socketState()
{
  return this->_state;
}

void WebSocket::stateChanged(QAbstractSocket::SocketState socketState)
{
    switch (socketState)
    {
        case QAbstractSocket::UnconnectedState:
            this->_state = Unconnected;
            break;
        case QAbstractSocket::HostLookupState:
            this->_state = HostLookup;
            break;
        case QAbstractSocket::ConnectingState:
            this->_state = Connecting;
            break;
        case QAbstractSocket::ConnectedState:
            this->_state = Connected;
            emit this->connected();
            emit this->opened();
            break;
        case QAbstractSocket::BoundState:
            this->_state = Bound;
            break;
        case QAbstractSocket::ClosingState:
            emit this->disconnected();
            emit this->closed();
            this->_state = Closing;
            break;
        case QAbstractSocket::ListeningState:
            this->_state = Listening;
            break;
        default:
            this->_state = Unknown;
            break;
    }

    emit this->socketStateChanged();
}
