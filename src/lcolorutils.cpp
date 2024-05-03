#include "lcolorutils.h"



QColor LColorUtils::absChangeColor(const QColor &color, const int saturation, const int value)
{
    return changeColor(color, saturation, 1.0, value, 1.0);
}



QColor LColorUtils::relChangeColor(const QColor &color, const qreal saturationFactor, const qreal valueFactor)
{
    return changeColor(color, -1, saturationFactor, -1, valueFactor);
}



QColor LColorUtils::changeColor(const QColor &color,
                                int saturation,
                                const qreal saturationFactor,
                                int value,
                                const qreal valueFactor)
{
    QColor newColor = color.toHsv();
    int h, s ,v;
    newColor.getHsv(&h, &s, &v);

    if (saturation == -1)
        saturation = s;
    saturation = qBound(0, qRound(saturation * saturationFactor), 255);

    if (h == -1)
        v = 255;

    if (value == -1)
        value = v;
    value = qBound(0, qRound(value * valueFactor), 255);

    newColor.setHsv(h, saturation, value);

    return newColor;
}
