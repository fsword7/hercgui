/*
 *  File:       MainPanel.cpp
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

#include "HerculesStudio.h"
#include "MainPanel.h"
#include "MainPanelClassic.h"
#include "Preferences.h"
#include "Environment.h"

#include <QResizeEvent>
#include <QColor>
#include <iostream>

MainPanel::MainPanel(QWidget *parent)
    :QWidget(parent),
    mMipsHWM(0)
{
    QString iconsPath = Environment::getIconsPath().c_str();
    mYellowLow = new QPixmap(iconsPath + "/yellow.png");
    mYellowHigh = new QPixmap(iconsPath + "/yellowhigh.png");

}

MainPanel::~MainPanel()
{
}

void MainPanel::doConnect()
{
    connect(powerOnButton(), SIGNAL(clicked()), this , SLOT(powerOnClickedSlot()));
    connect(powerOffButton(), SIGNAL(clicked()), this , SLOT(powerOffClickedSlot()));
    connect(interruptButton(), SIGNAL(clicked()), this , SLOT(interruptClickedSlot()));
    connect(loadButton(), SIGNAL(clicked()), this , SLOT(loadClickedSlot()));
    connect(stopButton(), SIGNAL(clicked()), this , SLOT(stopClickedSlot()));

    checkedConnect(mRestartButton,restartClickedSlot());
    checkedConnect(mStoreButton,storeClickedSlot());
    checkedConnect(mStartButton,startClickedSlot());
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



ClickLabel::ClickLabel(MainPanel * parent, QObject * lcd)
: QLabel(parent), mPanel(parent), mLcd(lcd)
{
}

void ClickLabel::mousePressEvent(QMouseEvent * event)
{
    mPanel->updateLcd((QLCDNumber*)(mLcd), event->x() > 30 ? 1 : -1); 
}
