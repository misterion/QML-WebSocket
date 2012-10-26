#pragma once

#include <QtCore/qglobal.h>

#ifdef QMLWEBSOCKET_LIB
# define QMLWEBSOCKET_EXPORT Q_DECL_EXPORT
#else
# define QMLWEBSOCKET_EXPORT Q_DECL_IMPORT
#endif