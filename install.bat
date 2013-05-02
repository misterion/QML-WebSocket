SET dst="%QTDIR%\imports\WebSocket"
MD %dst%

xcopy /Y ".\!build\qmlwebsocket\Release\qmlwebsocket.dll" ".\qmlsocketio\plugin"
@%QTDIR%\bin\qmlplugindump -path %dst% > ".\qmlsocketio\plugin\plugins.qmltypes"

xcopy /Y ".\qmlsocketio\plugin\*.*" %dst%



