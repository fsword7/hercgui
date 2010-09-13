/*
 *  File:       MipsGauge.cpp
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

#include "MipsGauge.h"
#include "HerculesStudio.h"

#include <QPaintEvent>
#include <QPainter>

MipsGauge::MipsGauge(QWidget * parent)
: Mips(parent)
{
	mMips = new QWidget(parent);
	mHwm = 0.0;
	resize(120,120);
}

MipsGauge::~MipsGauge()
{
}

void MipsGauge::move(int x, int y)
{
	QWidget::move(x,y);
}

void MipsGauge::display(double value)
{
	mValue = value;
	hOutDebug(5,"mValue=" << mValue);
	if (mValue > mHwm)
		mHwm = mValue;
	static QRect rect(0,0,120,120);
	update(rect);
}

void MipsGauge::setVisible(bool visible)
{
	QWidget::setVisible(visible);
}

void MipsGauge::setToolTip(const QString & tip)
{
	QWidget::setToolTip(tip);
}

void MipsGauge::paintEvent(QPaintEvent *)
{
	hOutDebug(5,"Gauge paint" << std::endl);
	QPainter painter(this);

	QColor red(240,30,30,255);
	QColor black(0,0,0,255);

	painter.drawArc(0,10,120,120,30*16,120*16);
	painter.translate(60,70);

	painter.rotate(30.0);
	for (int angle=30; angle<=150; angle+=10)
	{
		painter.drawLine(-65,0,-60,0);
		painter.rotate(10.0);
	}
	painter.rotate(-160.0);

	double angle = 30.0 ;
	if (mHwm > 0.0)
		angle = 30 + (mValue/mHwm*120);
	hOutDebug(5,"angle="<<angle << " hwm=" << mHwm << std::endl);
	painter.rotate(angle);
	painter.drawLine(-50,0,0,0);
}

void MipsGauge::deleteLater()
{
	QWidget::deleteLater();
}

bool MipsGauge::isVisible()
{
	return QWidget::isVisible();
}
