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
#include <WebSocket/WebSocketWrapper.h>

#include <websocketpp.hpp>
#include <websocket_connection_handler.hpp>

#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include <QDebug>
#ifdef _DEBUG 
#  define DEBUG_LOG qDebug() << __FILE__ << __LINE__ << __FUNCTION__
#else
#  define DEBUG_LOG / ## /
#endif

using websocketpp::session_ptr;

class WebSocketWrapperPrivate: public websocketpp::connection_handler
{
public:
  WebSocketWrapperPrivate(const QString& theUrl, WebSocketWrapper* wrapper) 
    : _wrapper(wrapper)
    , url(theUrl) 
  {
  }

  virtual ~WebSocketWrapperPrivate()
  {
  }

public:
  void on_fail(session_ptr con)
  {
    DEBUG_LOG << "Connection Failed: " << this->url;

    this->_wrapper->failed();
  }

  void on_open(session_ptr con)
  {
    DEBUG_LOG << "Connection Opened: " << this->url;

    this->_wrapper->opened();
    this->_session = con;
  }

  void on_close(session_ptr con)
  {
    DEBUG_LOG << "Connection Closed: " << this->url;

    this->_wrapper->closed();
    this->_session = websocketpp::client_session_ptr();
  }
  
  void on_message(session_ptr con,const std::string &msg)
  {
    const QString message = QString::fromStdString(msg);
    DEBUG_LOG << "Got Message:" << message;

    this->_wrapper->message(message);
  }

  virtual void on_message(session_ptr session, const std::vector<unsigned char> &data) 
  {
    throw std::exception("The method or operation is not implemented.");
  }
  
  void send(const QString& msg)
  {
    if (!this->_session) {
      qDebug() << "Tried to send on a disconnected connection! Aborting.";
      return;
    }

    this->_session->send(msg.toStdString());
  }

  void close()
  {
    if (!this->_session) {
      qDebug() << "Tried to close a disconnected connection!";
      return;
    }

    this->_session->close(websocketpp::close::status::GOING_AWAY,"");
  }

  WebSocketWrapper* _wrapper;
  QString url;
  session_ptr _session;
};

WebSocketWrapper::WebSocketWrapper(const QString& url, QObject* parent)
  : QThread(parent)
  , _handler(new WebSocketWrapperPrivate(url, this))
{
}

WebSocketWrapper::~WebSocketWrapper() 
{
  if (this->isRunning()) {
    this->stop();
    this->wait(10000);
  }
}

void WebSocketWrapper::run()
{
  try {
    boost::asio::io_service io_service;

    websocketpp::client_ptr client(
      new websocketpp::client(io_service, boost::shared_ptr<WebSocketWrapperPrivate>(_handler.data())));

    client->init();
    client->set_header("User Agent","Qt WebSocket++");

    client->connect(_handler->url.toStdString());

    io_service.run();
  } catch(std::exception& e) {
    emit this->failed();
    qWarning() << "Caught exception trying to get connection to endpoint: " << _handler->url << e.what();
  } catch (const char* msg) {
    emit this->failed();
    qWarning() << "Const const char& exception:" << msg;
  }
}

void WebSocketWrapper::send(const QString& msg)
{
  Q_ASSERT(!_handler.isNull());
  if (_handler.isNull())
    return;

  _handler->send(msg);
}

void WebSocketWrapper::stop()
{
  Q_ASSERT(!_handler.isNull());
  if (_handler.isNull())
    return;

  _handler->close();
}