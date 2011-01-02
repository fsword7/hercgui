/*
 *  File:       MainPanelModern.cpp
 *
 *  Author:     Jacob Dekel
 *  Created on: Oct 28, 2010
 *
 *  Copyright (c) 2009-2010 Jacob Dekel
 *  $Id: MainPanel.cpp 67 2010-09-13 14:06:31Z dekeljacob $
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
#include "MainPanelModern.h"
#include "Environment.h"
#include "PanelButton.h"
#include "Preferences.h"
#include "MipsLed.h"
#include "MipsGauge.h"
#include "ConfigurationEditor.h"

#include <sstream>


MainPanelModern::MainPanelModern(QWidget *parent)
: MainPanel(parent)
{
	setupUi(parent);
	mStopped=true;
	doConnect();
}

MainPanelModern::~MainPanelModern()
{
	delete mMips;
	delete mYellowLow;
	delete mYellowHigh;
}

void MainPanelModern::setupUi(QWidget *)
{
	const QColor black(50,50,50);
    const QColor white(255,255,20);
    QBrush blackBrush(black);
    QPalette blackPalette(white, black);
    this->setPalette(blackPalette);
    this->setAutoFillBackground(true);

    QString iconsPath = Environment::getIconsPath().c_str() ;
    hOutDebug(3,"icons path:'" << iconsPath.toStdString() << "'");

    setButton(mPowerOnButton, QString("Power On"), QIcon(iconsPath + "/modern/Play-icon.png"));
    setButton(mPowerOffButton, QString("Power Off"), QIcon(iconsPath + "/modern/Stop-icon.png"));
    setButton(mStopButton, QString("Stop"), QIcon(iconsPath + "/modern/Pause.png"));
    setButton(mInterruptButton, QString("Interrupt"), QIcon(iconsPath + "/modern/interrupt.png"));
    setButton(mLoadButton, QString("Load"), QIcon(iconsPath + "/modern/load.png"));

    mRestartButton = NULL;
    mStoreButton = NULL;
    mStartButton = NULL;

    mPowerOnButton->setToolTip("Power On");
    mPowerOffButton->setToolTip("Power Off");
    mInterruptButton->setToolTip("Interrupt");
    mLoadButton->setToolTip("Load (IPL)");
    mStopButton->setToolTip("Stop all CPUs");

    QFont font;
    font.setPointSize(6);

    mPSW = new QLabel(this);
    QFont pswFont("Mono", 10);
    mPSW->setFont(pswFont);

    if (Preferences::getInstance().mipsAsGauge())
    	mMips = new MipsGauge(this);
    else
    	mMips = new MipsLed(this);
    mMips->setVisible(false);
}

void MainPanelModern::setButton(QToolButton*& button, QString text, QIcon icon)
{
    button = new QToolButton(this);
    button->resize(60,70);
    button->setText(text);
    button->setIcon(icon);
    button->setStyleSheet("QToolButton {background: rgb(50,50,50); color: white; }");
    button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
}

void MainPanelModern::resizeEvent(QResizeEvent * )
{
    int width = this->size().rwidth();
    mPowerOnButton->move(10,20);
    mPowerOffButton->move(70,20);
    mStopButton->move(130,20);
    mInterruptButton->move(190,20);
    mLoadButton->move(250,20);

    mMips->move(width-680,20);

    mPSW->setGeometry(width-550,110,48*12,12);

}

int MainPanelModern::getLoadAddress()
{
    return mLcd0->intValue() + 16*mLcd1->intValue() +
        (16*16)*mLcd2->intValue() + (16*16*16)*mLcd3->intValue();
}

void MainPanelModern::setLoadAddress(const char *devNo)
{
	if (Preferences::getInstance().theme() == Preferences::Modern)
		return;

	int addr = ConfigurationEditor::parseNum(devNo,16);
	QLCDNumber *nums[4] = {mLcd0, mLcd1, mLcd2, mLcd3};
	for (int i=0; i< 4; i++)
	{
		int dig = addr%16;
		addr /= 16;
		nums[i]->display(dig);
	}
}

void MainPanelModern::setDormant()
{
    if (!mStopButton->text().contains("Stop", Qt::CaseSensitive))
    {
    	stopClickedSlot();
    }

    mMips->setVisible(false);  // must be last - indicates hercules is not active to other functions
}

void MainPanelModern::standby()
{
    mMips->setVisible(true);   // must be last - indicates hercules is active to other functions
}

bool MainPanelModern::notify(const std::string& statusLine)
{
	bool ret = true;
    if (statusLine.compare(0,5,"MIPS=") == 0)
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
    //3.07 :STATUS=CPU0000 PSW=00000000 00000000 0000000000000000 M....... instcount=0
    //3.08?:STATUS=CP00 PSW=00000000 00000000 0000000000000000 M......Z instcount=0
    else if (statusLine.compare(0,7,"STATUS=") == 0)
    {
        if ( (statusLine[54] == 'M') ||
             (statusLine[56] == 'W') )
            mMips->display(0);
    }
    else ret = false;

    return ret;
}

void MainPanelModern::switchMips()
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
			mMips->setToolTip(oldMips->toolTip());
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
			mMips->setToolTip(oldMips->toolTip());
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

QSize MainPanelModern::sizeHint() const
{
    return QSize(100, 100);
}

void MainPanelModern::stopClickedSlot()
{
	if (!mMips->isVisible())  // actually: is hercules running
		return;
    if (mStopButton->text().contains("Stop", Qt::CaseSensitive))
    {
    	delete mStopButton;
    	setButton(mStopButton, QString("Resume"), QIcon(QString(Environment::getIconsPath().c_str()) + "/modern/Resume.png"));
    	mStopped=true;
    }
    else
    {
    	delete mStopButton;
        setButton(mStopButton, QString("Stop"), QIcon(QString(Environment::getIconsPath().c_str()) + "/modern/Pause.png"));
        mStopped=false;
    }
    resizeEvent(NULL);
    connect(stopButton(), SIGNAL(clicked()), this , SLOT(stopClickedSlot()));
    mStopButton->setVisible(true);
    emit stopClicked();
}

