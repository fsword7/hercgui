/*
 *  File:       MipsGauge.h
 *
 *  Author:     Jacob Dekel
 *  Created on: June 15, 2010
 *
 *  Copyright (c) 2009-2013 Jacob Dekel
 *  $Id$
 *
 *	This object presents the current mips reading as gauge needle
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


#ifndef MIPSGAUGE_H_
#define MIPSGAUGE_H_

#include "Mips.h"

class MipsGauge: public Mips
{
	Q_OBJECT
public:
	MipsGauge(QWidget * parent = 0);
	virtual ~MipsGauge();

    virtual void display(double);
	virtual void setActive(bool visible);
    virtual void move(int, int);
    virtual void setToolTip(const QString & tip);
    virtual void deleteLater(void);
    virtual bool isVisible();

protected:
    void paintEvent(QPaintEvent *);
	double  mPrevAngle;

private:
    double mValue;
    double mHwm;
	QColor mPenColor;
};

#endif /* MIPSGAUGE_H_ */
