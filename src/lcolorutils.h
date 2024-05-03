#ifndef LCOLORUTILS_H
#define LCOLORUTILS_H

#include "lukqtglobal.h"

#include <QColor>

class LUKQT_DECLSPEC LColorUtils
{
public:
    static QColor absChangeColor(const QColor &color, int saturation, int value);

    static QColor relChangeColor(const QColor &color, qreal saturationFactor, qreal valueFactor);

    static QColor changeColor(const QColor &color,
                              int saturation,
                              qreal saturationFactor,
                              int value,
                              qreal valueFactor);
};

#endif // LCOLORUTILS_H
