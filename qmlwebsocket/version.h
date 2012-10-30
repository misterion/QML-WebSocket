#pragma once

#ifndef HUDSON_BUILD
    #define FILEVER          1,0,0,0
    #define PRODUCTVER       FILEVER
    #define STRFILEVER       "1,0,0,0"
    #define STRPRODUCTVER    STRFILEVER

    #define COMPANYNAME      ""
    #define FILEDESCRIPTION  "Developer version of Qml WebSocket extension"
    #define INTERNALNAME     "Qml WebSocket"
    #define LEGALCOPYRIGHT   "Copyright(c) 2012"

    #ifdef DEBUG 
        #define ORIGINALFILENAME "qmlwebsocketd.dll"
    #else
        #define ORIGINALFILENAME "qmlwebsocket.dll"
    #endif

    #define PRODUCTNAME      "Qml WebSocket extension"
#else
    #define FILEVER          $$MAJOR$$,$$MINOR$$,$$HUDSON_BUILD$$
    #define PRODUCTVER       FILEVER
    #define STRFILEVER       "$$MAJOR$$,$$MINOR$$,$$HUDSON_BUILD$$,$$GIT_REVISION$$"
    #define STRPRODUCTVER    STRFILEVER

    #define COMPANYNAME      "$$COMPANYNAME$$"
    #define FILEDESCRIPTION  "$$FILEDESCRIPTION$$"
    #define INTERNALNAME     "$$INTERNALNAME$$"
    #define LEGALCOPYRIGHT   "$$LEGALCOPYRIGHT$$"
    #define ORIGINALFILENAME "$$FILENAME$$"
    #define PRODUCTNAME      "$$PRODUCTNAME$$"
#endif




