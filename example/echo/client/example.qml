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
import "./../../../qmlsocketio"



Rectangle {
    width: 400
    height: 300

    function responseHandler(data) {
        text.text = data;
    }

    Rectangle {
        width: 100
        height: 50
        color: "red"

        visible: !socket.connected
        anchors.centerIn: parent

        Text {
            anchors.centerIn: parent
            text: "Press to connect"
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                if (!socket.connected) {
                    socket.connect();
                }
            }
        }
    }

    Rectangle {
        width: 100
        height: 50
        color: "green"

        visible: socket.connected
        anchors.centerIn: parent

        Column {
            Text {
                anchors.centerIn: parent
                text: "Press to send request"

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if (socket.connected) {
                            socket.emit('request', Math.random());
                        }
                    }
                }
            }

            Text {
                id: text

                x: 10
                y : 10

                text: "No text"
            }
        }


    }



    SocketIO {
        id: socket
        debug: true

        uri: 'ws://localhost:8080'

        Component.onCompleted: {
            console.log(on('response', responseHandler)); //this one connected
            console.log(on('response', responseHandler)); //this one ignored
        }
    }
}
