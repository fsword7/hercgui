/*
 *  File:       MipsLcd.h
 *
 *  Author:     Jacob Dekel
 *  Created on: June 15, 2010
 *
 *  Copyright (c) 2009-2010 Jacob Dekel
 *  $Id: Mips.h 34 2009-11-07 06:15:58Z jacob $
 *
 *	This object presents the current mips reading in LCD format
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

#ifndef MIPSLCD_H_
#define MIPSLCD_H_

#include "Mips.h"

#include <QLCDNumber>

class MipsLed : public Mips
{
    Q_OBJECT
public:
    MipsLed(QWidget * parent = 0);
    virtual ~MipsLed();

    virtual void display(double);
    virtual void setVisible(bool visible);
    virtual void move(int, int);
    virtual void setToolTip(const QString & tip);
    virtual void deleteLater(void);
    virtual bool isVisible();

private:
    QLCDNumber *mMips;
};

#endif /* MIPSLCD_H_ */
