@xcopy /Y /I /R "..\!build\QMLWebSocket\Release\*.dll" "plugin\"
@%QTDIR%\bin\qmlplugindump -path .\plugin > .\plugin\plugins.qmltypes