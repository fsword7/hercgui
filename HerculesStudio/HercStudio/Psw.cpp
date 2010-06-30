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
#include "HerculesStudio.h"
#include "Preferences.h"

#include <QStatusBar>
#include <iostream>

Psw::Psw( Psw::PswMode mode, QMainWindow * mainWindow)

{
	mMainWindow = mainWindow;
	mMode = mode;
	mFontCourier = new QFont("Courier");

	setFont();
	setStatusBar();
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
	if (mMode == Psw::Docked)
	{
		mLine.replace(0,46, &statusLine.c_str()[7], 46);
		mLine.replace(48,statusLine.length()-56, &statusLine[63]);
		setText(&mLine[0]);
	}
	else if (mActive)
	{
		mCpu->setText(statusLine.substr(7,39).c_str());
		mInstCount->setText(statusLine.substr(60).c_str());
	}
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
	if (mMode == Psw::StatusBar)
		setStatusBar();
	setVisible(active);
}

void Psw::setMode(Psw::PswMode mode)
{
	mMode = mode;
	setStatusBar();
}

void Psw::setStatusBar()
{
	mMainWindow->setStatusBar(0);
	mMainWindow->setStatusBar(new QStatusBar());

	if (mMode == Psw::StatusBar && mActive)
	{
		mCpu = new QLabel("        ", mMainWindow->statusBar());
		mCpu->setMinimumWidth(320);
		mCpu->setFont(*mFontCourier);
		mCpu->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
		mMainWindow->statusBar()->addWidget(mCpu);

		mInstCount = new QLabel("        ", mMainWindow->statusBar());
		mCpu->setFont(*mFontCourier);
		mInstCount->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
		mMainWindow->statusBar()->addWidget(mInstCount);
	}
}
