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
#include <WebSocket/QmlWebSocket.h>
#include <WebSocket/WebSocket.h>

#include <QDeclarativeEngine>
#include <QDebug>

void QmlWebSocket::registerTypes(const char *uri)
{
  Q_ASSERT(QLatin1String(uri) == QLatin1String("WebSocket"));

  //UNDONE pass uri in register
  qmlRegisterType<WebSocket>("WebSocket", 1, 0, "WebSocket");
}

#ifdef HAVE_QT5
void QmlWebSocket::initializeEngine(QQmlEngine *engine, const char *uri)
#else
void QmlWebSocket::initializeEngine(QDeclarativeEngine *engine, const char *uri)
#endif
{
}

#ifndef HAVE_QT5
  #ifdef _DEBUG
    Q_EXPORT_PLUGIN2(QMLWebSocketd, QmlWebSocket);
  #else
    Q_EXPORT_PLUGIN2(QMLWebSocket, QmlWebSocket);
  #endif
#endif
