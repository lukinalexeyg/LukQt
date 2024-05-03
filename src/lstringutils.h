#ifndef LSTRINGUTILS_H
#define LSTRINGUTILS_H

#include "lchars.h"
#include "lukqtglobal.h"

#include <QVariant>

#define QSTRING_TO_CHAR_ARRAY(string) string.toStdString().c_str()

#define NUMBER_TO_CHAR_ARRAY(number) std::to_string(number).c_str()

class LUKQT_DECLSPEC LStringUtils
{
public:
    enum Mib {
        Utf8 = 106,
        IBM866 = 2086,
        Windows1251 = 2251
    };

    enum JustifyOrientation {
        Left,
        Right
    };

    enum WordWrapPolicy {
        DisabledAfterDigits,
        EnabledAfterAllChars
    };

public:
    static QTextCodec *windows1251TextCodec();

    static QString fromLocal8Bit(const char *string, Mib mib = Mib::Windows1251);

    template<typename T>
    static QString fromNumber(const T number,
                              const int length,
                              const int base = 10,
                              const QChar &fill = LChars::Number::zero)
    {
        return QString::number(number, base).rightJustified(length, fill);
    }

    static QString fromBool(bool value);

    static QString fromVariant(const QVariant &value);

    static wchar_t* toWCharArray(const QString &string);

    static const wchar_t* toConstWCharArray(const QString &string);

    static int toInt(const QString &string, int def, int base = 10, bool *ok = nullptr);

    static bool toBool(const QString &string, bool def, bool *ok = nullptr);

    static QString concatStrings(const QString &string1, const QChar &c, const QString &string2);

    static QString concatStrings(const QString &string1,
                                 const QChar &c1,
                                 const QString &string2,
                                 const QChar &c2,
                                 const QString &string3);

    static QString tagString(const QString &source, const QString &tag);

    static QString attributeString(const QString &source, const QString &attribute);

    static QString betweenString(const QString &source, const QString &beginString, const QString &endString);

    static QString removeTags(const QString &string);

    static QString chopString(const QString &string, const QString &fromWhat);

    static QString wordWrapText(const QString &string,
                                const int length,
                                const JustifyOrientation justifyOrientation = JustifyOrientation::Left,
                                const WordWrapPolicy wordWrapPolicy = WordWrapPolicy::DisabledAfterDigits);

    static QStringList wordWrapString(const QString &string,
                                      const int length,
                                      const JustifyOrientation justifyOrientation = JustifyOrientation::Left,
                                      const WordWrapPolicy wordWrapPolicy = WordWrapPolicy::DisabledAfterDigits);

    static QString fixString(const QString &string,
                             const int length,
                             const JustifyOrientation justifyOrientation = JustifyOrientation::Left);    
};

#endif // LSTRINGUTILS_H
