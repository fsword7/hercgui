/*
 *  File: Trigger.h
 *
 *  Author:     Jacob Dekel
 *  Created on: May 10, 2014
 *
 *  Copyright (c) 2009-2013 Jacob Dekel
 *  $Id$
 *
 *  This object represents a trigger for the Hercules Studio printer. A trigger changes the behavior of the printer
 *  (E.g., creation of a job file).
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef TRIGGER_H
#define TRIGGER_H

#include <QString>

class Trigger
{
public:
    Trigger();
    Trigger(const QString& triggerString);
    enum TriggerType
    {
        TriggerString,
        TriggerRegExp
    };
    void set(int line, int column, TriggerType trigger, QString& text);
    void set(int line, int column, int trigger, QString& text);
    static const QString &decodeTriggerType(TriggerType type);
    static TriggerType encodeTriggerType(const QString &type);

    inline int line() { return mLine; }
    inline int column() { return mColumn; }
    inline TriggerType triggerType() { return mTriggerType; }
    inline QString text() { return mText; }

    inline void setLine(int line) { mLine = line; }
    inline void setColumn(int column) { mColumn = column; }
    inline void setTriggerType(TriggerType triggerType) { mTriggerType = triggerType; }
    inline void setText(QString text) { mText = text; }

private:
    int mLine;                  // line to look for
    int mColumn;                // column to look for
    TriggerType mTriggerType;   // trigger type
    QString mText;              // validation text
};

#endif // TRIGGER_H
