#include "StringedObject.h"

StringedObject::StringedObject()
{
}

bool StringedObject::getOneField(QString& left, QString& field, const char sep)
{
    const QString q(sep);
    int i=left.indexOf(q);
    left = left.mid(i+1);
    i=left.indexOf(q);
    field = left.left(i);
    left = left.mid(i+1);
    if (left.startsWith(",")) left.remove(0,1);
    return true;
}

bool StringedObject::getOneField(QString& left, float& field, const char sep)
{
    QString str;
    bool ret = getOneField(left, str, sep);
    field = str.toFloat();
    return ret;
}

bool StringedObject::getOneField(QString& left, int& field, const char sep)
{
    QString str;
    bool ret = getOneField(left, str, sep);
    field = str.toInt();
    return ret;
}

bool StringedObject::getOneField(QString& left, bool& field, const char sep)
{
    QString str;
    bool ret = getOneField(left, str, sep);
    field = (str.compare("true") == 0);
    return ret;
}
