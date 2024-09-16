#include "lnumberutils.h"

#ifdef Q_OS_UNIX
    #include <math.h>
#endif



quint64 LNumberUtils::pow(qint64 x, uint n)
{
    quint64 result = 1;

    while (n) {
        if (n & 0x1)
            result *= x;
        x *= x;
        n >>= 1;
    }

    return result;
}



int LNumberUtils::compareReals(const qreal a, const qreal b, const qreal epsilon)
{
    const qreal _a = qAbs(a);
    const qreal _b = qAbs(b);
    const qreal e = (_a < _b ? _b : _a) * epsilon;

    if (b - a > e)
        return -1; // a < b

    if (a - b > e)
        return 1; // a > b

    return 0; // a == b
}



bool LNumberUtils::isEqual(const qreal a, const qreal b, const qreal epsilon)
{
    return compareReals(a, b, epsilon) == 0;
}



bool LNumberUtils::isLess(const qreal a, const qreal b, const qreal epsilon)
{
    return compareReals(a, b, epsilon) < 0;
}



bool LNumberUtils::isLessOrEqual(const qreal a, const qreal b, const qreal epsilon)
{
    return compareReals(a, b, epsilon) <= 0;
}



bool LNumberUtils::isGreater(const qreal a, const qreal b, const qreal epsilon)
{
    return compareReals(a, b, epsilon) > 0;
}



bool LNumberUtils::isGreaterOrEqual(const qreal a, const qreal b, const qreal epsilon)
{
    return compareReals(a, b, epsilon) >= 0;
}



void LNumberUtils::boundZero(qreal &a)
{
    if (signbit(a))
        a = 0.0;
}



bool LNumberUtils::change(qreal &value, const qreal newValue, const qreal epsilon)
{
    if (!isEqual(value, newValue, epsilon)) {
        value = newValue;
        return true;
    }

    return false;
}
