#ifndef __QTVP_GLOBAL_H_
#define __QTVP_GLOBAL_H_

#include <QtCore/qglobal.h>

#if defined(QTVP_LIBRARY)
#  define QTVPSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QTVPSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // __QTVP_GLOBAL_H_
