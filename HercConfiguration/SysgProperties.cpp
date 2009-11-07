/*
 *  File: SysgProperties.cpp
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id: SysgProperties.cpp 34 2009-11-07 06:15:58Z jacob $
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

#include "SysgProperties.h"

SysgProperties::SysgProperties(ConfigLine& line, QWidget *parent)
: TerminalProperties(line, parent)
{
    getUi().terminalCombo->clear();
    getUi().terminalCombo->insertItems(0, QStringList()
     << QApplication::translate("TerminalPropertiesClass", "SYSG", 0, QApplication::UnicodeUTF8)
    );
    getUi().terminalCombo->setEnabled(false);
    getUi().deviceNumber->setText("0000");
    getUi().deviceNumber->setEnabled(false);
}

SysgProperties::~SysgProperties()
{
}

bool SysgProperties::isSysg()
{
	return true;
}

