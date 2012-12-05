/*
 *  File:       MipsLcd.cpp
 *
 *  Author:     Jacob Dekel
 *  Created on: Jun 15, 2010
 *
 *  Copyright (c) 2009-2010 Jacob Dekel
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

#include "MipsLed.h"
#include "HerculesStudio.h"
#include "Preferences.h"

#include <QLCDNumber>

MipsLed::MipsLed(QWidget * parent)
: Mips()
{
	mMipsPalette.setColor(QPalette::Foreground, QColor(255,0,0));
    mMips = new QLCDNumber(6,parent);
	mMips->setPalette(mMipsPalette);
    mMips->setSegmentStyle(QLCDNumber::Flat);
    mMips->setFrameShape( QLCDNumber::NoFrame );
	mMips->setVisible(true);
}

MipsLed::~MipsLed()
{
}

void MipsLed::move(int x, int y)
{
	mMips->move(x,y);
}

void MipsLed::display(double value)
{
	mMips->display(value);
}

void MipsLed::setActive(bool active)
{
	if (active)
	{
		if (Preferences::getInstance().greenLed())
			mMipsPalette.setColor(QPalette::Foreground, QColor(0,255,0));
		else
			mMipsPalette.setColor(QPalette::Foreground, QColor(255,0,0));
	}
	else
		mMipsPalette.setColor(QPalette::Foreground, QColor(120,100,100));
	mMips->setPalette(mMipsPalette);
}

void MipsLed::setToolTip(const QString & tip)
{
	mMips->setToolTip(tip);
}

void MipsLed::deleteLater()
{
	mMips->deleteLater();
}

bool MipsLed::isVisible()
{
	return mMips->isVisible();
}
