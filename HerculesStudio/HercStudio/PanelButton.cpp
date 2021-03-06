/*
 *  File:       PanelButton.cpp
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009-2013 Jacob Dekel
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

#include "PanelButton.h"
#include "HerculesStudio.h"
#include "Environment.h"

#include <iostream>
#include <QPainter>
#include <QPixmap>


PanelButton::PanelButton(const QString& upButtonName, const QString&  downButtonName, QWidget * parent) :
    QPushButton(parent), mDown(false), mUpButtonName(upButtonName), mDownButtonName(downButtonName)

{
    connect(this,SIGNAL(pressed()), this, SLOT(pressedSignal()));
    connect(this,SIGNAL(released()), this, SLOT(releasedSignal()));
}

PanelButton::~PanelButton()
{
  
}

void PanelButton::replaceButton(const QString& upButtonName, const QString&  downButtonName)
{
	mUpButtonName = upButtonName;
	mDownButtonName = downButtonName;
	repaint();
}
void PanelButton::paintEvent(QPaintEvent *)
{
    QString  filename = mUpButtonName;
    if (mDown)
        filename = mDownButtonName;
    QPixmap newPix(filename);
    QPainter painter(this);
    painter.drawPixmap(0, 0, newPix);
    resize(33,25);
}

void PanelButton::pressedSignal()
{
    mDown = true;
    repaint();
}

void PanelButton::releasedSignal()
{
    mDown = false;
    repaint();
}


