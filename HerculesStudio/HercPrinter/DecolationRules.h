/*
 *  File: DecolationRules.h
 *
 *  Author:     Jacob Dekel
 *  Created on: May 10, 2014
 *
 *  Copyright (c) 2009-2014 Jacob Dekel
 *  $Id$
 *
 *  This object represents the decolation rules for the Hercules Studio printer
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

#ifndef DECOLATIONRULES_H
#define DECOLATIONRULES_H

#include "Trigger.h"

#include <QList>
#include <QList>

typedef QList<Trigger> Triggers;

struct Seperator
{
public:
  int mPages;         // number of consequtive pages which make a separator
  Triggers mTriggers; // sum of triggers that make a separator page
};

struct AccountingField
{
  QString mName;
  int mLine;
  int mColumn;
  int mLength;
};

class  AccountingFields
{
public:
    void add(const AccountingField& fld) { mFields.push_back(fld);}
public:
    QList<AccountingField> mFields;
};



class DecolationRules
{
public:
    DecolationRules();

    const QString& name();
    void setName(const QString &name);
    void setFileNameTemplate(const QString & name);
    QString filenameTemplate();

public:
    Seperator mSeperator;
    AccountingFields mAccounting;
    QString mFileNameTemplate;

private:
    QString mName;
};

#endif // DECOLATIONRULES_H
