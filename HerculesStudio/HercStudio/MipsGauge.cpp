/*
 * MipsGauge.cpp
 *
 *  Created on: Jun 16, 2010
 *      Author: yakov
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
	//mMips->move(x,y);
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

return;
	painter.rotate(30.0);
	painter.drawLine(-50,0,0,0);
	painter.rotate(30.0);
	painter.drawLine(-50,0,0,0);
	painter.rotate(30.0);
	painter.drawLine(-50,0,0,0);
	painter.rotate(30.0);
	painter.drawLine(-50,0,0,0);
	painter.rotate(30.0);
	painter.drawLine(-50,0,0,0);
return;
	painter.translate(180.0,45.0);
	painter.rotate(30.0);
	for (int angle=30; angle<=150; angle+=10)
	{
		painter.drawLine(-45,0,-37,0);
		painter.rotate(10.0);
	}
	painter.rotate(-160.0);
   painter.setPen(red);
   painter.drawText(-20,10,"MIPS");
}

void MipsGauge::deleteLater()
{
	//QWidget::deleteLater();
}

bool MipsGauge::isVisible()
{
	return QWidget::isVisible();
}
