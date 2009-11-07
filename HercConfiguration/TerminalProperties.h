/*
 *  File: TerminalProperties.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id: TerminalProperties.h 34 2009-11-07 06:15:58Z jacob $
 *
 *	This object manages the terminal configuration properties
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

#ifndef TERMINALPROPERTIES_H
#define TERMINALPROPERTIES_H

#include "ConfigLine.h"
#include "GenericDeviceProperties.h"
#include "HerculesStudio.h"

#include "ui_TerminalProperties.h"

class TerminalProperties : public GenericDeviceProperties
{
    Q_OBJECT

public:
    TerminalProperties(ConfigLine& line,QWidget *parent = 0);
    virtual ~TerminalProperties();

protected:
    Ui::TerminalPropertiesClass & getUi() { return ui;};
    virtual bool isSysg();

private:
    Ui::TerminalPropertiesClass ui;

private slots:
    void ok();
    void cancel();

signals:
    void updateLine(bool keep);
};

#endif // TERMINALPROPERTIES_H
