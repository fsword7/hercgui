/*
 *  File:       Mips.cpp
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id: Mips.cpp 34 2009-11-07 06:15:58Z jacob $
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

#include "Mips.h"

#include <QLCDNumber>

Mips::Mips(QWidget * parent) :
    QWidget(parent),
    mNum1(1, this),
    mNum2(1, this),
    mNum3(1, this),
    mNum(6, this)
{
    QPalette palette;
    palette.setColor(QPalette::Foreground, QColor(255,0,0));

    mNum1.setSegmentStyle(QLCDNumber::Flat);
    mNum1.setFrameShape( QLCDNumber::NoFrame );
    mNum1.setPalette(palette);
    mNum1.display(0);
    mNum1.move(0,0);

    mNum2.setPalette(palette);
    mNum2.setSegmentStyle(QLCDNumber::Flat);
    mNum2.setFrameShape( QLCDNumber::NoFrame );
    mNum2.display(0);
    mNum2.move(15,0);

    mNum3.setPalette(palette);
    mNum3.setSegmentStyle(QLCDNumber::Flat);
    mNum3.setFrameShape( QLCDNumber::NoFrame );
    mNum3.display(0);
    mNum3.move(30,0);

    mNum.setPalette(palette);
    mNum.setSegmentStyle(QLCDNumber::Flat);
    mNum.setFrameShape( QLCDNumber::NoFrame );
    mNum.display(0);
    mNum.move(45,0);

    setVisible(true);
}

Mips::~Mips()
{
}

void Mips::setValue(uint value)
{
    mNum.display(12.34);
    if (value > 999) // :)
    {
        mNum1.display(9);
        mNum2.display(9);
        mNum3.display(9);
        return;
    }
    int val = value;
    mNum3.display(val%10);
    val /= 10;
    mNum2.display(val%10);
    val /= 10;
    mNum1.display(val);

}


