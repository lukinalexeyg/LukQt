#ifndef LNUMBERUTILS_H
#define LNUMBERUTILS_H

#include "lukqtglobal.h"

#include <QtGlobal>

class LUKQT_DECLSPEC LNumberUtils
{
public:
    static quint64 pow(int x, uint n);

    static int compareReals(qreal a, qreal b = 0.0, qreal epsilon = 1e-6);

    static bool isEqual(qreal a, qreal b = 0.0, qreal epsilon = 1e-6);

    static bool isLess(qreal a, qreal b = 0.0, qreal epsilon = 1e-6);

    static bool isLessOrEqual(qreal a, qreal b = 0.0, qreal epsilon = 1e-6);

    static bool isGreater(qreal a, qreal b = 0.0, qreal epsilon = 1e-6);

    static bool isGreaterOrEqual(qreal a, qreal b = 0.0, qreal epsilon = 1e-6);

    static void boundZero(qreal &a);

    static bool change(qreal &value, qreal newValue, qreal epsilon = 1e-6);
};

#endif // LNUMBERUTILS_H
