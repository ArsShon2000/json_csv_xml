#ifndef LIBEXP_GLOBAL_H
#define LIBEXP_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LIBEXP_LIBRARY)
#  define LIBEXP_EXPORT Q_DECL_EXPORT
#else
#  define LIBEXP_EXPORT Q_DECL_IMPORT
#endif

#endif // LIBEXP_GLOBAL_H
