#include "lstringutils.h"

#include "lmacros.h"

#include <QDateTime>
#include <QTextCodec>

static const QString s_trueString("true");
static const QString s_falseString("false");



QString LStringUtils::fromLocal8Bit(const char *string, const Mib mib, const int size)
{
    QTextCodec *textCodec = QTextCodec::codecForMib(mib);
    if (textCodec == nullptr)
        return QString();

    QTextCodec *oldTextCodec = QTextCodec::codecForLocale();
    QTextCodec::setCodecForLocale(textCodec);
    QString newString = QString::fromLocal8Bit(string, size);
    QTextCodec::setCodecForLocale(oldTextCodec);

    return newString;
}



QString LStringUtils::fromBool(const bool value)
{
    return value ? s_trueString : s_falseString;
}



QString LStringUtils::fromVariant(const QVariant &value, const FromVariantOptions options, const QString &indentString)
{
    return _fromVariant(value, options, indentString, 1);
}



QString LStringUtils::_fromVariant(const QVariant &value,
                                   const FromVariantOptions options,
                                   const QString &indentString,
                                   const int deep)
{
    QString string;

    if (options.testFlag(FromVariantOption::TypesNames)) {
        string += value.typeName();
        string += LChars::parenthesisLeft;
    }

    switch (value.type()) {
        case QVariant::Type::String: {
            string += value.toString();
            break;
        }

        case QVariant::Type::Char: {
            string += value.toChar();
            break;
        }

        case QVariant::Type::ByteArray: {
            string += QString(value.toByteArray());
            break;
        }

        case QVariant::Type::Int: {
            const int i = value.toInt();
            string += QString::number(i);
            break;
        }

        case QVariant::Type::UInt: {
            const uint i = value.toUInt();
            string += QString::number(i);
            break;
        }

        case QVariant::Type::LongLong: {
            const qint64 i = value.toLongLong();
            string += QString::number(i);
            break;
        }

        case QVariant::Type::ULongLong: {
            const quint64 i = value.toULongLong();
            string += QString::number(i);
            break;
        }

        case QVariant::Type::Double: {
            string += QString::number(value.toDouble());
            break;
        }

        case QVariant::Type::Bool: {
            string += fromBool(value.toBool());
            break;
        }

        case QVariant::Type::Date: {
            string += value.toDate().toString(Qt::ISODate);
            break;
        }

        case QVariant::Type::Time: {
            string += value.toTime().toString(Qt::ISODate);
            break;
        }

        case QVariant::Type::DateTime: {
            string += value.toDateTime().toString(Qt::ISODate);
            break;
        }

        case QVariant::Type::StringList: {
            const QStringList list = value.toStringList();
            if (!options.testFlag(FromVariantOption::TypesNames))
                string += LChars::bracketLeft;
            string += listToString(list, options, indentString, deep);
            if (!options.testFlag(FromVariantOption::TypesNames))
                string += LChars::bracketRight;
            break;
        }

        case QVariant::Type::List: {
            const QVariantList list = value.toList();
            QStringList stringList;
            for_index_inc (i, list.size())
                stringList.append(_fromVariant(list.at(i), options, indentString, deep + 1));
            if (!options.testFlag(FromVariantOption::TypesNames))
                string += LChars::bracketLeft;
            string += listToString(stringList, options, indentString, deep);
            if (!options.testFlag(FromVariantOption::TypesNames))
                string += LChars::bracketRight;
            break;
        }

        case QVariant::Type::Map: {
            const QVariantMap map = value.toMap();
            string += mapToString(map, options, indentString, deep);
            break;
        }

        case QVariant::Type::Hash: {
            const QVariantHash hash = value.toHash();
            string += mapToString(hash, options, indentString, deep);
            break;
        }

        default:
            break;
    }

    if (options.testFlag(FromVariantOption::TypesNames))
        string += LChars::parenthesisRight;

    return string;
}



template<typename T>
QString LStringUtils::mapToString(const T &map,
                                  const FromVariantOptions options,
                                  const QString &indentString,
                                  const int deep)
{
    QString string;
    QStringList stringList;

    for_iterator_const_inc (it, map) {
        QString s = it.key();
        s += LChars::colon;
        if (options.testFlag(FromVariantOption::Spaces))
            s += LChars::space;
        s += _fromVariant(it.value(), options, indentString, deep + 1);
        stringList.append(s);
    }

    if (!options.testFlag(FromVariantOption::TypesNames))
        string += LChars::braceLeft;

    string += listToString(stringList, options, indentString, deep);

    if (!options.testFlag(FromVariantOption::TypesNames))
        string += LChars::braceRight;

    return string;
}



QString LStringUtils::listToString(const QStringList &list,
                                   const FromVariantOptions options,
                                   const QString &indentString,
                                   const int deep)
{
    QString string;

    if (options.testFlag(FromVariantOption::Indented))
        string += LChars::Control::LF;

    for_index_inc (i, list.size()) {
        if (options.testFlag(FromVariantOption::Indented))
            for_index_inc (t, deep)
                string += indentString;

        string += list.at(i);

        if (i < list.size()-1) {
            string += LChars::comma;
            if (options.testFlag(FromVariantOption::Indented))
                string += LChars::Control::LF;
            else if (options.testFlag(FromVariantOption::Spaces))
               string += LChars::space;
        }
    }

    if (options.testFlag(FromVariantOption::Indented)) {
        string += LChars::Control::LF;
        for_index_inc (t, deep - 1)
            string += indentString;
    }

    return string;
}



wchar_t* LStringUtils::toWCharArray(const QString &string)
{
    return (wchar_t*)string.utf16();
}



const wchar_t* LStringUtils::toConstWCharArray(const QString &string)
{
    return R_CAST_LPCWSTR(string.utf16());
}



int LStringUtils::toInt(const QString &string, const int def, const int base, bool *ok)
{
    bool _ok;
    const int i = string.toInt(&_ok, base);

    if (ok != nullptr)
        *ok = _ok;

    if (_ok)
        return i;

    return def;
}



bool LStringUtils::toBool(const QString &string, const bool def, bool *ok)
{
    const QString _value = string.toLower();

    if (_value == s_trueString) {
        if (ok != nullptr)
            *ok = true;
        return true;
    }

    if (_value == s_falseString) {
        if (ok != nullptr)
            *ok = true;
        return false;
    }

    if (ok != nullptr)
        *ok = false;

    return def;
}



QString LStringUtils::tag(const QString &source, const QString &tag)
{
    return betweenStrings(source, QSL("<%1>").arg(tag), QSL("</%1>").arg(tag));
}



QString LStringUtils::attribute(const QString &source, const QString &attribute)
{
    return betweenStrings(source, QSL("%1=\"").arg(attribute), QSL("\""));
}



QString LStringUtils::betweenStrings(const QString &source, const QString &beginString, const QString &endString)
{
    int beginIndex = source.indexOf(beginString);
    if (beginIndex < 0)
        return QString();
    beginIndex += beginString.count();

    const int endIndex = source.indexOf(endString, beginIndex);
    if (endIndex <= 0)
        return QString();

    return source.mid(beginIndex, endIndex - beginIndex);
}



QString LStringUtils::removeTags(const QString &string)
{
    QString _string = string;

    for_index_inc (i, _string.length()) {
        const int openTagIndex = _string.indexOf(LChars::less);
        const int closeTagIndex = _string.indexOf(LChars::greater);

        if (openTagIndex < 0 || closeTagIndex < 0)
            break;

        const int count = closeTagIndex - openTagIndex + 1;
        _string.remove(openTagIndex, count);
        i -= count;
    }

    return _string;
}



QString LStringUtils::chop(const QString &string, const QString &fromWhat)
{
    const int pos = string.indexOf(fromWhat);

    if (pos == -1)
        return string;

    QString newString = string;
    newString.chop(string.length() - pos);

    return newString;
}



QString LStringUtils::wordWrapText(const QString &string,
                                   const int width,
                                   const JustifyOrientation justifyOrientation,
                                   const WordWrapPolicy wordWrapPolicy)
{
    const QStringList stringList = string.split(LChars::Control::LF);
    QStringList resultStringList;

    for_element_ref_inc_const (list0, stringList) {
        const QStringList list1 = wordWrapString(list0, width, justifyOrientation, wordWrapPolicy);
        if (!list1.isEmpty())
            resultStringList.append(list1);
        else
            resultStringList.append(QString());
    }

    return resultStringList.join(LChars::Control::LF);
}



QStringList LStringUtils::wordWrapString(const QString &string,
                                         const int width,
                                         const JustifyOrientation justifyOrientation,
                                         const WordWrapPolicy wordWrapPolicy)
{
    QString _string = string.simplified();
    const QStringList list0 = _string.split(LChars::space, QString::SkipEmptyParts);

    QStringList list1;
    if (wordWrapPolicy == WordWrapPolicy::EnabledAfterAllChars)
        list1 = list0;
    else
        for_index_inc (i, list0.count()) {
            if (i == 0)
                list1.append(list0.at(i));
            else {
                const QString previousWord = list0.at(i-1);
                if (!previousWord.at(previousWord.length()-1).isDigit())
                    list1.append(list0.at(i));
                else
                    list1.last().append(QSL(" %1").arg(list0.at(i)));
            }
        }

    QStringList list2;
    int line = 0;
    int wordsCount = 0;

    for_element_ref_inc_const (s, list1) {
        if (wordsCount == 0) {
            list2.append(s);
            ++wordsCount;
        }

        else {
            const int l = list2.at(line).length() + s.length() + 1;

            if (l <= width) {
                list2[line] += QSL(" %1").arg(s);
                ++wordsCount;
            }
            else {
                list2.append(s);
                ++line;
                wordsCount = 1;
            }
        }
    }

    for_index_inc (i, list2.count())
        list2[i] = fixLength(list2.at(i), width, justifyOrientation);

    return list2;
}



QString LStringUtils::fixLength(const QString &string, const int length, const JustifyOrientation justifyOrientation)
{
    QString _string = string.simplified();

    if (_string.length() > length)
        _string = QSL("%1.").arg(_string.left(length-1));

    if (_string.length() < length) {
        if (justifyOrientation == JustifyOrientation::Left)
            return _string.leftJustified(length, LChars::space);
        return _string.rightJustified(length, LChars::space);
    }

    return _string;
}
