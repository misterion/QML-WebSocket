#pragma once

#include <QtCore/qglobal.h>

#if (QT_VERSION >= 0x050000)
#define HAVE_QT5
#endif

#ifdef QMLWEBSOCKET_LIB
    #define QMLWEBSOCKET_EXPORT Q_DECL_EXPORT
#else
    #define QMLWEBSOCKET_EXPORT Q_DECL_IMPORT
#endif
