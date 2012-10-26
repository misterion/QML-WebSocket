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
#include <WebSocket/WebSocketWrapper.h>

WebSocket::WebSocket(QDeclarativeItem *parent /*= 0*/) : QDeclarativeItem(parent)
{
}

WebSocket::~WebSocket()
{
}

void WebSocket::connect(const QString &uri)
{
  this->_wrapper.reset(new WebSocketWrapper(uri, this));

  QObject::connect(this->_wrapper.data(), SIGNAL(message(const QString&)), this, SIGNAL(message(const QString&)));
  QObject::connect(this->_wrapper.data(), SIGNAL(opened()), this, SIGNAL(opened()));
  QObject::connect(this->_wrapper.data(), SIGNAL(closed()), this, SIGNAL(closed()));
  QObject::connect(this->_wrapper.data(), SIGNAL(failed()), this, SIGNAL(failed()));

  this->_wrapper->start();
}

void WebSocket::disconnect()
{
  if (this->_wrapper.isNull()) {
    return;
  }

  this->_wrapper->stop();
  this->_wrapper.reset();
}

void WebSocket::send(const QString& message)
{
  if (this->_wrapper.isNull()) {
    return;
  }

  this->_wrapper->send(message);
}
