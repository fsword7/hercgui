#include "Trigger.h"

Trigger::Trigger()
{

}

void Trigger::set(int line, int column, TriggerType trigger, QString &text)
{
    mLine = line;
    mColumn = column;
    mTriggerType = trigger;
    mText = text;
}

void Trigger::set(int line, int column, int trigger, QString &text)
{
    set(line, column, static_cast<TriggerType>(trigger), text);
}

const QString& Trigger::decodeTriggerType(TriggerType type)
{
    static const QString string("String");
    static const QString regexp("Regular Expression");
    if (type == TriggerString) return string;
    else return regexp;
}

Trigger::TriggerType Trigger::encodeTriggerType(const QString& type)
{
    static const QString string("String");
    static const QString regexp("Regular Expression");
    if (type.compare(string) == 0) return TriggerString;
    else if (type.compare(regexp) == 1) return TriggerRegExp;
    Q_ASSERT_X(false, "encodeTriggerType", "bad trigger name"); //force error
    return TriggerString;
}


