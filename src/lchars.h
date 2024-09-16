#ifndef LCHARS_H
#define LCHARS_H

#include <QChar>

namespace LChars {
    static const QChar space(' ');
    static const QChar exclam('!');
    static const QChar numberSign('#');
    static const QChar dollar('$');
    static const QChar percent('%');
    static const QChar ampersand('&');
    static const QChar asterisk('*');
    static const QChar plus('+');
    static const QChar comma(',');
    static const QChar minus('-');
    static const QChar dot('.');
    static const QChar colon(':');
    static const QChar semicolon(';');
    static const QChar equal('=');
    static const QChar question('?');
    static const QChar at('@');
    static const QChar circum('^');
    static const QChar underscore('_');
    static const QChar bar('|');
    static const QChar tilda('~');

    static const QChar slash('/');
    static const QChar backslash('\\');

    namespace Quote {
        static const QChar Double('\"');
        static const QChar single('\'');
        static const QChar left('`');
    }

    namespace Number {
        static const QChar zero('0');
        static const QChar one('1');
        static const QChar two('2');
        static const QChar three('3');
        static const QChar four('4');
        static const QChar five('5');
        static const QChar six('6');
        static const QChar seven('7');
        static const QChar eight('8');
        static const QChar nine('9');
    }

    static const QChar parenthesisLeft('(');
    static const QChar parenthesisRight(')');

    static const QChar less('<');
    static const QChar greater('>');

    static const QChar bracketLeft('[');
    static const QChar bracketRight(']');

    static const QChar braceLeft('{');
    static const QChar braceRight('}');

    namespace Control {
        static const QChar NUL('\0');
        static const QChar BEL('\a');
        static const QChar BS('\b');
        static const QChar HT('\t');
        static const QChar LF('\n');
        static const QChar VT('\v');
        static const QChar FF('\f');
        static const QChar CR('\r');
    }
}

#endif // LCHARS_H
