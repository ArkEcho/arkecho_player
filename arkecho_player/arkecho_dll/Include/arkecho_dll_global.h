#ifndef ARKECHO_DLL_GLOBAL_H
#define ARKECHO_DLL_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef ARKECHO_DLL_LIB
# define ARKECHO_DLL_EXPORT Q_DECL_EXPORT
#else
# define ARKECHO_DLL_EXPORT Q_DECL_IMPORT
#endif

#endif // ARKECHO_DLL_GLOBAL_H
