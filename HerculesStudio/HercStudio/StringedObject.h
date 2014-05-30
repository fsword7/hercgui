#ifndef STRINGEDOBJECT_H
#define STRINGEDOBJECT_H

#include <QString>

class StringedObject
{
public:
    StringedObject();

    QString toString();

protected:
    inline QString addSep(QString& sep, const char *name, QString& text, bool comma = true)
    {
        if (comma)
            return name + QString("=") + sep + text + sep + ",";
        else
            return name + QString("=") + sep + text + sep;
    }

    QString addSep(QString sep, const char *name, double number, bool comma = true)
    {
        QString s = QString::number(number);
        return addSep(sep, name, s, comma);
    }

    QString addSep(QString sep, const char *name, bool b, bool comma = true)
    {
        QString s(b ? "true" : "false");
        return addSep(sep, name, s, comma);
    }

    QString addSep(QString sep, const char *name, int number, bool comma = true)
    {
        QString s = QString::number(number);
        return addSep(sep, name, s, comma);
    }

    bool getOneField(QString& left, QString& field, const char sep='\"');
    bool getOneField(QString& left, float& field, const char sep='\"');
    bool getOneField(QString& left, int& field, const char sep='\"');
    bool getOneField(QString& left, bool& field, const char sep='\"');
};

#endif // STRINGEDOBJECT_H
