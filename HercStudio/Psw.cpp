/*
 *  File:       Psw.cpp
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id: Psw.cpp 34 2009-11-07 06:15:58Z jacob $
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


#include "Psw.h"
#include "Preferences.h"

#include <iostream>

Psw::Psw( QWidget * )
{
	setFont();
	mActive = false;
	mLine.reserve(100);
	mLine.assign(100,' ');
	setGeometry(0,0,1200,12);
}

Psw::~Psw()
{

}

void Psw::notify(const std::string& statusLine)
{
	//STATUS=CPU0000 PSW=00000000 00000000 0000000000000000 M....... instcount=0
	if (!mActive || statusLine.compare(0,7,"STATUS=") != 0)
		return;
	mLine.replace(0,46, &statusLine.c_str()[7], 46);
	mLine.replace(48,statusLine.length()-56, &statusLine[63]);
	setText(&mLine[0]);
}

void Psw::setFont()
{
	Preferences& pref = Preferences::getInstance();
	QFont font(pref.fontName(Preferences::PswFontObject).c_str(),
			pref.fontSize(Preferences::PswFontObject),
			(pref.fontIsBold(Preferences::PswFontObject) ? QFont::Bold : QFont::Normal),
			pref.fontIsItalic(Preferences::PswFontObject));

    font.setStyleHint(QFont::Courier);
    QLabel::setFont(font);
}

void Psw::setActive(bool active)
{
	mActive = active;
	setVisible(active);
}
