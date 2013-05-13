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

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>

#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include <QtCore/QDebug>

#if defined(_MSC_VER)
#	if defined(_DEBUG)
#		define DEBUG_LOG qDebug() << __FILE__ << __LINE__ << __FUNCTION__
#		define WARN_LOG qWarning() << __FILE__ << __LINE__ << __FUNCTION__
#	else
#		define DEBUG_LOG / ## /
#		define WARN_LOG / ## / 
#	endif
#elif defined(__GCC__)
#   define DEBUG_LOG qDebug() 
#	define WARN_LOG qWarning() 
#endif

typedef websocketpp::client<websocketpp::config::asio_client> WebsocketppClient;
typedef websocketpp::config::asio_client::message_type::ptr MessagePtr;
typedef websocketpp::connection_hdl ConnectionHandler;


class WebSocketWrapperPrivate
{
private:
    WebSocketWrapper* _wrapper;
    WebsocketppClient *_client;
    WebsocketppClient::connection_ptr _connectionPtr;

    QString url;
    bool _connected;

public:
    WebSocketWrapperPrivate(const QString& theUrl, WebSocketWrapper* wrapper) 
        : _wrapper(wrapper)
        , _connected(false)
        , url(theUrl) 
    {
        this->_client = new WebsocketppClient();
        
        this->_client->set_message_handler(bind(&WebSocketWrapperPrivate::onMessage, this, ::_1, ::_2));
        this->_client->set_open_handler(bind(&WebSocketWrapperPrivate::onOpen, this, ::_1));
        this->_client->set_close_handler(bind(&WebSocketWrapperPrivate::onClose, this, ::_1));
        this->_client->set_fail_handler(bind(&WebSocketWrapperPrivate::onFail, this, ::_1));
    }

    virtual ~WebSocketWrapperPrivate()
    {
        delete this->_client;
    }

public:
    void onFail(ConnectionHandler hdl)
    {
        DEBUG_LOG << "Connection Failed: " << this->url;
        this->_wrapper->failed();
    }

    void onOpen(ConnectionHandler hdl)
    {
        DEBUG_LOG << "Connection Opened: " << this->url;
        this->_connected = true;
        this->_wrapper->opened();
    }

    void onClose(ConnectionHandler hdl)
    {
        DEBUG_LOG << "Connection Closed: " << this->url;
        this->_connected = false;
        this->_wrapper->closed();
    }
  
    void onMessage(ConnectionHandler hdl, MessagePtr msg) 
    {
        const QString message = QString::fromStdString(msg->get_payload());
        DEBUG_LOG << "Got Message:" << message;        

        this->_wrapper->message(message);
    }

    void send(const QString& msg)
    {
        if (!this->_connected) {
          DEBUG_LOG << "Tried to send on a disconnected connection! Aborting.";
          return;
        }
        
        this->_client->send(this->_connectionPtr, msg.toStdString(), websocketpp::frame::opcode::value::text);
    }

    void open(const QString& url) 
    {
        websocketpp::lib::error_code ec;
        this->_connectionPtr = this->_client->get_connection(url.toStdString(), ec);
        
        if (ec) {
            //m_client.get_alog().write(websocketpp::log::alevel::app, "Get Connection Error: "+ec.message());
            
            this->_wrapper->failed();
        	return;
        }
        
        this->_client->set_user_agent("Qt WebSocket++");

        this->_client->init_asio();
        this->_client->connect(this->_connectionPtr);
        this->_client->run();
    }

    void close()
    {
        if (!this->_connected) {
            DEBUG_LOG << "Tried to close a disconnected connection!";
            return;
        }

        this->_client->close(this->_connectionPtr, websocketpp::close::status::normal, "");
    }
};

WebSocketWrapper::WebSocketWrapper(const QString& url, QObject* parent)
  : QThread(parent)
  , _url(url)
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
        this->_handler->open(this->_url);
    } catch (const std::exception & e) {
        DEBUG_LOG << e.what();
    } catch (websocketpp::lib::error_code e) {
        //DEBUG_LOG << e.message();
    } catch (...) {
        DEBUG_LOG << "other exception";
    }
}

void WebSocketWrapper::send(const QString& msg)
{
  Q_ASSERT(!this->_handler.isNull());
  if (this->_handler.isNull())
    return;

  this->_handler->send(msg);
}

void WebSocketWrapper::stop()
{
  Q_ASSERT(!this->_handler.isNull());
  if (this->_handler.isNull())
    return;

  this->_handler->close();
}