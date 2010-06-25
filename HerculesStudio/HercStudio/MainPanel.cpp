/*
 *  File:       MainPanel.cpp
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id: MainPanel.cpp 34 2009-11-07 06:15:58Z jacob $
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

#include "HerculesStudio.h"
#include "MainPanel.h"
#include "PanelButton.h"
#include "Environment.h"
#include "MipsLed.h"
#include "MipsGauge.h"
#include "Preferences.h"
#include "ConfigurationEditor.h"

#include <QResizeEvent>
#include <QColor>
#include <iostream>
#include <sstream>

MainPanel::MainPanel(QWidget *parent)
    :QWidget(parent),
    mMipsHWM(0)
{
    QString iconsPath = Environment::getIconsPath().c_str();
    mYellowLow = new QPixmap(iconsPath + "/yellow.png");
    mYellowHigh = new QPixmap(iconsPath + "/yellowhigh.png");
    setupUi(parent);
}

MainPanel::~MainPanel()
{
	mMips->close();
	delete mMips;
	delete mYellowLow;
	delete mYellowHigh;
}

void MainPanel::setupUi(QWidget *)
{
	const QColor black(50,50,50);
    const QColor white(255,255,20);
    QBrush blackBrush(black);
    QPalette blackPalette(white, black);
    this->setPalette(blackPalette);
    this->setAutoFillBackground(true);
    this->setMinimumHeight(100);

    QString iconsPath = Environment::getIconsPath().c_str();
    mPowerOnButton = new PanelButton(iconsPath + "/poweronoffu.gif", iconsPath + "/poweronoffd.gif", this);
    mPowerOffButton = new PanelButton(iconsPath + "/poweroffu.gif", iconsPath + "/poweroffd.gif", this);
    mInterruptButton = new PanelButton(iconsPath + "/interruptu.gif", iconsPath + "/interruptd.gif", this);
    mLoadButton = new PanelButton(iconsPath + "/loadu.gif", iconsPath + "/loadd.gif", this);

    mRestartButton = new PanelButton(iconsPath + "/restartu.gif", iconsPath + "/restartd.gif", this);
    mStoreButton = new PanelButton(iconsPath + "/storeu.gif", iconsPath + "/stored.gif", this);
    mStopButton = new PanelButton(iconsPath + "/stopu.gif", iconsPath + "/stopd.gif", this);
    mStartButton = new PanelButton(iconsPath + "/startu.gif", iconsPath + "/startd.gif", this);

    mPowerOnButton->setToolTip("Power On");
    mPowerOffButton->setToolTip("Power Off");
    mInterruptButton->setToolTip("Interrupt");
    mLoadButton->setToolTip("Load (IPL)");
    mRestartButton->setToolTip("Restart");
    mStoreButton->setToolTip("Store");
    mStopButton->setToolTip("Stop all CPUs");
    mStartButton->setToolTip("Start all CPUs");

    QFont font;
    font.setPointSize(6);
    mSys = new QLabel(this);
    mSys->setPixmap(*mYellowLow);
    mSysText = new QLabel("SYS",this);
    mSysText->setFont(font);
    mMan = new QLabel(this);
    mMan->setPixmap(*mYellowLow);
    mManText = new QLabel("MAN",this);
    mManText->setFont(font);
    mWait = new QLabel(this);
    mWait->setPixmap(*mYellowLow);    //path = iconsPath + "/yellowhigh.png";

    mPSW = new QLabel(this);
    QFont pswFont("Mono", 10);
    mPSW->setFont(pswFont);

    mWaitText = new QLabel("WAIT",this);
    mWaitText->setFont(font);
    mLoad = new QLabel(this);
    mLoad->setPixmap(*mYellowLow);
    mLoadText = new QLabel("LOAD",this);
    mLoadText->setFont(font);
    mLcd0 = new QLCDNumber(1,this);;
    mLcd0->setSegmentStyle(QLCDNumber::Flat);
    mLcd0->setFrameStyle(QFrame::NoFrame);
    mLcd0->resize(20,20);
    mLcd0->setMode(QLCDNumber::Hex);
    mLcd1 = new QLCDNumber(1,this);
    mLcd1->setSegmentStyle(QLCDNumber::Flat);
    mLcd1->setFrameStyle(QFrame::NoFrame);
    mLcd1->resize(20,20);
    mLcd1->setMode(QLCDNumber::Hex);
    mLcd2 = new QLCDNumber(1,this);
    mLcd2->setSegmentStyle(QLCDNumber::Flat);
    mLcd2->setFrameStyle(QFrame::NoFrame);
    mLcd2->resize(20,20);
    mLcd2->setMode(QLCDNumber::Hex);
    mLcd3 = new QLCDNumber(1,this);
    mLcd3->setSegmentStyle(QLCDNumber::Flat);
    mLcd3->setFrameStyle(QFrame::NoFrame);
    mLcd3->resize(20,20);
    mLcd3->setMode(QLCDNumber::Hex);

    QString path = Environment::getIconsPath().c_str();
    QPixmap pixMap(path + "/dial1.gif");
    mDial0 = new ClickLabel(this,mLcd0);
    mDial0->setPixmap(pixMap);
    mDial1 = new ClickLabel(this,mLcd1);
    mDial1->setPixmap(pixMap);
    mDial2 = new ClickLabel(this,mLcd2);
    mDial2->setPixmap(pixMap);
    mDial3 = new ClickLabel(this,mLcd3);
    mDial3->setPixmap(pixMap);

    mDial0->setToolTip("Click to select IPL address");
    mDial1->setToolTip("Click to select IPL address");
    mDial2->setToolTip("Click to select IPL address");
    mDial3->setToolTip("Click to select IPL address");

    if (Preferences::getInstance().mipsAsGauge())
    	mMips = new MipsGauge(this);
    else
    	mMips = new MipsLed(this);
    mMips->setVisible(false);

    connect(mPowerOnButton, SIGNAL(clicked()), this , SLOT(powerOnClickedSlot()));
    connect(mPowerOffButton, SIGNAL(clicked()), this , SLOT(powerOffClickedSlot()));
    connect(mInterruptButton, SIGNAL(clicked()), this , SLOT(interruptClickedSlot()));
    connect(mLoadButton, SIGNAL(clicked()), this , SLOT(loadClickedSlot()));
    connect(mRestartButton, SIGNAL(clicked()), this , SLOT(restartClickedSlot()));
    connect(mStoreButton, SIGNAL(clicked()), this , SLOT(storeClickedSlot()));
    connect(mStartButton, SIGNAL(clicked()), this , SLOT(startClickedSlot()));
    connect(mStopButton, SIGNAL(clicked()), this , SLOT(stopClickedSlot()));
}

void MainPanel::resizeEvent(QResizeEvent * )
{
    int width = this->size().rwidth();
    mPowerOnButton->move(width-90,15);
    mPowerOffButton->move(width-50,15);
    mInterruptButton->move(width-90,45);
    mLoadButton->move(width-50,45);
    mRestartButton->move(width-510,15);
    mStoreButton->move(width-550,15);
    mStopButton->move(width-510,45);
    mStartButton->move(width-550,45);

    mSys->move(width-200,60);
    mSysText->move(width-200,50);
    mMan->move(width-172,60);
    mManText->move(width-172,50);
    mWait->move(width-144,60);
    mWaitText->move(width-144,50);
    mLoad->move(width-116,60);
    mLoadText->move(width-116,50);

    mLcd0->move(width-250,5);
    mLcd1->move(width-310,5);
    mLcd2->move(width-370,5);
    mLcd3->move(width-430,5);

    mDial0->move(width-270,30);
    mDial1->move(width-330,30);
    mDial2->move(width-390,30);
    mDial3->move(width-450,30);

    mMips->move(width-680,20);

    mPSW->setGeometry(width-550,110,48*12,12);

}

QSize MainPanel::sizeHint() const
{
    return QSize(100, 100);
}

void MainPanel::powerOnClickedSlot()
{
    emit powerOnClicked();
}
void MainPanel::powerOffClickedSlot()
{
    emit powerOffClicked();
}

void MainPanel::loadClickedSlot()
{
    emit loadClicked();
}

void MainPanel::restartClickedSlot()
{
    emit restartClicked();
}

void MainPanel::storeClickedSlot()
{
    emit storeClicked();
}

void MainPanel::startClickedSlot()
{
    emit startClicked();
}

void MainPanel::stopClickedSlot()
{
    emit stopClicked();
}

void MainPanel::interruptClickedSlot()
{
    emit interruptClicked();
}

void MainPanel::updateLcd(QLCDNumber * lcd, int inc)
{
    lcd->display(lcd->intValue()+inc);
    if (lcd->intValue() > 15)
        lcd->display(0);
    if (lcd->intValue() < 0)
        lcd->display(15);
    outDebug(5, std::cout << "update " << lcd->intValue() << std::endl);
}

int MainPanel::getLoadAddress()
{
    return mLcd0->intValue() + 16*mLcd1->intValue() +
        (16*16)*mLcd2->intValue() + (16*16*16)*mLcd3->intValue();
}

void MainPanel::setLoadAddress(const char *devNo)
{
	int addr = ConfigurationEditor::parseNum(devNo,16);
	QLCDNumber *nums[4] = {mLcd0, mLcd1, mLcd2, mLcd3};
	for (int i=0; i< 4; i++)
	{
		int dig = addr%16;
		addr /= 16;
		nums[i]->display(dig);
	}
}

void MainPanel::setDormant()
{
    mMips->setVisible(false);
}

void MainPanel::standby()
{
    mMips->setVisible(true);
}

void MainPanel::notify(const std::string& statusLine)
{
    if (statusLine.compare(0,4,"SYS=") == 0)
    {
        char sysVal = statusLine[4];
        if (sysVal == '1')
            mSys->setPixmap(*mYellowHigh);
        else
            mSys->setPixmap(*mYellowLow);

    }
    else if (statusLine.compare(0,4,"MAN=") == 0)
    {
        char sysVal = statusLine[4];
        if (sysVal == '1')
            mMan->setPixmap(*mYellowHigh);
        else
            mMan->setPixmap(*mYellowLow);

    }
    else if (statusLine.compare(0,5,"LOAD=") == 0)
    {
        char sysVal = statusLine[4];
        if (sysVal == '1')
            mLoad->setPixmap(*mYellowHigh);
        else
            mLoad->setPixmap(*mYellowLow);

    }
    else if (statusLine.compare(0,5,"MIPS=") == 0)
    {
        double d = atof(statusLine.c_str()+5);
        if (d > mMipsHWM)
        {
            mMipsHWM = d;
            std::stringstream ss;
            ss << "High Water Mark:" << d;
            mMips->setToolTip(ss.str().c_str());
        }
        mMips->display(d);
    }
    //STATUS=CPU0000 PSW=00000000 00000000 0000000000000000 M....... instcount=0
    else if (statusLine.compare(0,7,"STATUS=") == 0)
    {
        if ( (statusLine[54] == 'M') ||
             (statusLine[56] == 'W') )
            mMips->display(0);
    }
}

void MainPanel::switchMips()
{
	bool updated = false;
	bool visible = mMips->isVisible();
	if (Preferences::getInstance().mipsAsGauge())
	{
		MipsLed * oldMips = dynamic_cast<MipsLed *>(mMips);
		if (oldMips != NULL)
		{
			Mips * gauge = new MipsGauge(this);
			mMips = gauge;
			oldMips->deleteLater();
			updated = true;
		}
	}
	else
	{
		MipsGauge * oldMips = dynamic_cast<MipsGauge *>(mMips);
		if (oldMips != NULL)
		{
			Mips * lcd = new MipsLed(this);
			mMips = lcd;
			oldMips->deleteLater();
			updated = true;
		}
	}
	if (updated)
	{
		mMips->setVisible(visible);
		int width = this->size().rwidth();
		mMips->move(width-680,20);
	}

}

ClickLabel::ClickLabel(QWidget * parent, QLCDNumber * lcd)
: QLabel(parent), mLcd(lcd)
{
}

void ClickLabel::mousePressEvent(QMouseEvent * event)
{
    MainPanel::updateLcd(this->mLcd,event->x() > 30 ? 1 : -1);
}
