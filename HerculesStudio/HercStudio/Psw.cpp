/*
 *  File:       Psw.cpp
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id$
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
	mFontCourier = new QFont("Mono",10);

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

bool Psw::notify(const std::string& statusLine)
{
	if (!mActive)
		return false;
	if (statusLine.compare(0,4,"SYS=") == 0)
    {
        if (statusLine[4] == '1')
        	mSys->setText("SYS");
        else
        	mSys->setText("   ");
		return true;
    }
	//STATUS=CPU0000 PSW=00000000 00000000 0000000000000000 M.W..... instcount=0
	if (statusLine.compare(0,7,"STATUS=") != 0)
		return false;
	if (mMode == Psw::Docked)
	{
		mLine.replace(0,46, &statusLine.c_str()[7], 46);
		mLine.replace(48,statusLine.length()-56, &statusLine[63]);
		setText(&mLine[0]);
	}
	else if (mActive)
	{
		mCpu->setText(statusLine.substr(7,43).c_str());
		mInstCount->setText(statusLine.substr(62).c_str());
		if (statusLine.c_str()[54] == 'M')
			mMan->setText("MAN");
		else
			mMan->setText("   ");
		if (statusLine.c_str()[56] == 'W')
			mWait->setText("WAIT");
		else
			mWait->setText("    ");
	}
	return true;
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
		mCpu->setMinimumWidth(400);
		mCpu->setFont(*mFontCourier);
		mCpu->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
		mMainWindow->statusBar()->addWidget(mCpu);

		mSys = new QLabel("   ", mMainWindow->statusBar());
		mSys->setFont(*mFontCourier);
		mSys->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
		mMainWindow->statusBar()->addWidget(mSys);

		mWait = new QLabel("    ", mMainWindow->statusBar());
		mWait->setFont(*mFontCourier);
		mWait->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
		mMainWindow->statusBar()->addWidget(mWait);

		mMan = new QLabel("   ", mMainWindow->statusBar());
		mMan->setFont(*mFontCourier);
		mMan->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
		mMainWindow->statusBar()->addWidget(mMan);

		mInstCount = new QLabel("        ", mMainWindow->statusBar());
		mInstCount->setFont(*mFontCourier);
		mInstCount->setFrameStyle(QFrame::StyledPanel | QFrame::NoFrame);
		mMainWindow->statusBar()->addWidget(mInstCount);
	}
}
