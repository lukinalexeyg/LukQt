#ifndef LUKQTGLOBAL_H
#define LUKQTGLOBAL_H

#include <QtGlobal>

#if defined(Q_OS_WIN)
    #if defined(LUKQT_EXPORT)
        #define LUKQT_DECLSPEC Q_DECL_EXPORT
    #elif defined(LUKQT_IMPORT)
        #define LUKQT_DECLSPEC Q_DECL_IMPORT
    #endif
#endif

#if !defined(LUKQT_DECLSPEC)
    #define LUKQT_DECLSPEC
#endif

#endif // LUKQTGLOBAL_H
