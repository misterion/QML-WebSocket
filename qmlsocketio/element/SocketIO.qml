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
import QtQuick 1.1
import WebSocket 1.0

import 'lib/request.js' as Request
import 'lib/container.js' as Container

Item {
    id: handler

    property string uri;
    property bool connected: false;
    property bool debug: false;

    property int heartbeatTimeout: 15000;  //@readonly
    property int disconnectTimeout: 25000; //@readonly
    property string sid;                   //@readonly

    property variant _uri;

    /**
     * Endpoint and id don`t used usualy. So they must be last arguments. In
     * most of cases we need only data.
     */
    signal message(string data, string endPoint, int id);
    signal jsonMessage(variant args, string endPoint, int id);
    signal eventMessage(string name, variant args, string endPoint, int id);
    signal errorMessage(string reason, string advice, string endPoint);

    signal opened();
    signal closed();
    signal failed();

    /**
     * Most of real socket.io application use events(type 5) for communicate between server and client. So it`s a
     * high level api for that.
     *
     * on('chatMessage', function(fromId, toId, text) { ... });
     *
     */
    function on(name, func) {
        return _registerEvent(Container.onHandlers, name, func);
    }

    function _registerEvent(container, name, func) {
        if (container.hasOwnProperty(name)) {
            if (container[name].filter(function(e) { return e === func; }).length) {
                return false;
            }

            container[name].push(func);
        } else {
            container[name] = [func];
        }

        return true;
    }

    function once(name, func) {
       return _registerEvent(Container.onceHandlers, name, func);
    }

    function connect() {
        if (!connected) {
            _handshake();
        }
    }

    function disconnect() {
        if (connected) {
            socket.disconnect();
        }
    }

    function forceDisconnect() {
        if (!connected) {
            return;
        }

        var uri = new Request.Uri(handler.uri);

        uri.setProtocol(protocol === 'wss' ? 'https' : 'http');
        uri.setPath('/socket.io/1/websocket/' + sid);
        uri.addQueryItem('disconnect', '');

        Request.http.request(uri, function(response) {});
    }

    function send(type, message, endpoint, id) {
        if (!connected) {
            return;
        }

        socket.send(type + ':' + (id || '') + ':' + (endpoint || '') + ':' + message);
    }

    function sendJson(data, endPoint, id) {
        send(4, JSON.stringify(data), endPoint, id);
    }

    function emit(name, args, endPoint, id) {
        send(5, JSON.stringify({ name: name, args: args }), endPoint, id);
    }

    function _debug(message) {
        if (debug) {
            console.debug(message)
        }
    }

    function _handshake() {
        var uri = new Request.Uri(handler.uri),
            protocol = uri.protocol();

        uri.setProtocol(protocol === 'wss' ? 'https' : 'http');
        uri.setPath('/socket.io/1/');

        Request.http.request(uri, function(response) {
            var expr = /([0-9a-zA-Z_-]*):([0-9]*):([0-9]*):(.*)/,
                result, sid;

            if (response.status !== 200) {
                console.log('Socket.IO Handshake: Server rejected connection with code ' +  response.status)
                handler.failed();
                return;
            }

            result = expr.exec(response.body);
            if (null === result) {
                console.log('Socket.IO Handshake: Server response not match protocol ' +  response.body)
                handler.failed();
                return;
            }

            sid = result[1];

            handler.heartbeatTimeout = parseInt(result[2], 10);
            handler.disconnectTimeout = parseInt(result[3], 10);

            uri.setPath('/socket.io/1/websocket/' + sid);
            uri.setProtocol(protocol);

            _debug('Socket.IO Handshake: Connecting to '  + uri)

            socket.connect(uri);
        });
    }

    function _sendHeartbeat()
    {
        socket.send('2::');
    }

    /**
     * Parse response according to socket.IO rules.
     * https://github.com/LearnBoost/socket.io-spec
     */
    function _parseMessage(message) {

        var expr = /([0-8]):([0-9]*):([^:]*)[:]?(.*)/,
            result = expr.exec(message),
            type,
            parsedObject;

        if (null === result) {
            console.log('Socket.IO wrong packet data ' +  message);
            return;
        }

        type = Number(result[1]);
        if (NaN === type) {
            console.log('Socket.IO wrong packet type ' + result[1]);
            return;
        }

        switch(type) {
        case 0:
            _debug('Received message type 0 (Disconnect)');
            socket.disconnect();
            break;
        case 1: //Connection Acknowledgement
            _debug('Received Message type 1 (Connect)');
            break;
        case 2:
            _debug('Received Message type 2 (Heartbeat)');
            _sendHeartbeat();
            break;
        case 3:
            _debug('Received Message type 3 (Message): ' + message);
            message(result[4] || '', result[3] || '', result[2] || '');
            break;
        case 4:
            _debug('Received Message type 4 (JSON Message): ' + message)
            try {
                parsedObject = JSON.parse(result[4]);
                jsonMessage(parsedObject, result[3] || '', result[2] || 0);
            } catch(e) {
                console.log('JSON Parse Error: ' + result[4]);
            }
            break;
        case 5:
            _debug('Received Message type 5 (Event): ' + message)
            try {
                parsedObject = JSON.parse(result[4]);
            } catch(e) {
                console.log('JSON Parse Error: ' + result[4]);
                return;
            }

            if (!parsedObject.hasOwnProperty('name')) {
                console.log('Invalid socket.IO Event');
                return;
            }

            eventMessage(parsedObject.name, parsedObject.args || {}, result[3] || '', result[2] || '');
            _handleEvents(parsedObject.name, parsedObject.args);

            break;
        case 6:
            _debug('Received Message type 6 (ACK)');
            break;
        case 7:
            _debug('Received Message type 7 (Error): ' + message);
            _sendErrorPacket(result[3] || '', result[4]);
            break;
        case 8:
            _debug('Received Message type 8 (Noop)');
            break;
        default:
            _debug('Invalid Socket.IO message type: ' + result[1]);
            break;
        }
    }

    function _handleEvents(name, args) {
        if (Container.onceHandlers.hasOwnProperty(name)) {
            Container.onceHandlers[name].forEach(function(e) {
                _executeHandler(e, args);
            });
            delete Container.onceHandlers[name];
        }

        if (!Container.onHandlers.hasOwnProperty(name)) {
            return;
        }

        Container.onHandlers[name] = Container.onHandlers[name].filter(function(e) {
            return _executeHandler(e, args);
        });
    }

    function _executeHandler(handler, args) {
        if (!handler) {
            return false;
        }

        try {
            handler.apply(this, args);
        } catch (e) {
             _debug('Handler execution error: ' + e.message);
            return false;
        }

        return true;
    }

    function _sendErrorPacket(endPoint, rawString) {
        var plusPos = rawString.search('+');
        if (-1 === plusPos)
            return;

        errorMessage(rawString.substring(0, plusPos),
                     rawString.substring(plusPos + 1, rawString.length),
                     endPoint || '');
    }

    WebSocket {
        id: socket

        onMessage: handler._parseMessage(message);
        onFailed: handler.failed();
        onOpened: {
            handler.connected = true;
            handler.opened();
        }
        onClosed: {
            handler.connected = false;
            handler.closed();
        }
    }
}
