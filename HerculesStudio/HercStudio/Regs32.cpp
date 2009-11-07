/*
 *  File:       Regs32.cpp
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id: Regs32.cpp 34 2009-11-07 06:15:58Z jacob $
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

#include "Regs32.h"
#include "MainWindow.h"

#include <iostream>

std::string Regs32::gCommand = "]GREGS=";
std::string Regs32::cCommand = "]CREGS=";
std::string Regs32::fCommand = "]FREGS=";
std::string Regs32::aCommand = "]AREGS=";

Regs32::Regs32(QWidget * parent, std::string command) :
    QWidget::QWidget(parent),
    mParent(parent),
	mLine1(this),
	mLine2(this),
	mLine3(this),
	mLine4(this)
{
    QFont font("monospace", 9);
    font.setStyleHint(QFont::Courier);
    mLine1.setFont(font);
    mLine2.setFont(font);
    mLine3.setFont(font);
    mLine4.setFont(font);
    mLine1.setGeometry(0,10,330,12);
    mLine2.setGeometry(0,26,330,12);
    mLine3.setGeometry(0,42,330,12);
    mLine4.setGeometry(0,58,330,12);

    mCommand = command;
}

Regs32::~Regs32()
{
}

void Regs32::setActive(bool active)
{
    mLine1.setVisible(active);
    mLine2.setVisible(active);
    mLine3.setVisible(active);
    mLine4.setVisible(active);
    std::string command = mCommand + (active ? '1' : '0');
    MainWindow * mainWindow = dynamic_cast<MainWindow *>(mParent);
    mainWindow->issueCommand(command);
}

void Regs32::notify(const std::string& statusLine)
{
	if (statusLine[2] == '0')
		Regs32::mLine1.setText(statusLine.c_str());
	else if (statusLine[2] == '4')
		Regs32::mLine2.setText(statusLine.c_str());
	else if (statusLine[2] == '8')
		Regs32::mLine3.setText(statusLine.c_str());
	else if (statusLine[2] == 'C')
		Regs32::mLine4.setText(statusLine.c_str());
}
