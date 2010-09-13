/*
 *  File: PunchProperties.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id$
 *
 *	This object manages the card punch configuration properties
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

#ifndef PUNCHPROPERTIES_H
#define PUNCHPROPERTIES_H

#include "ConfigLine.h"
#include "GenericDeviceProperties.h"
#include "HerculesStudio.h"

#include "ui_PunchProperties.h"

class PunchProperties : public GenericDeviceProperties
{
    Q_OBJECT

public:
    PunchProperties(ConfigLine& line,QWidget *parent = 0);
    virtual ~PunchProperties();
    void setBoxesEnabled(bool enabled);

private:
    Ui::PunchPropertiesClass ui;

private slots:
    void ok();
    void cancel();
};

class PunchFileNameValidator : public QValidator
{
public:
    PunchFileNameValidator(QObject * parent);
    virtual QValidator::State validate(QString&, int&) const;

private:
    PunchProperties * mParent;
};


#endif // PUNCHPROPERTIES_H
