/*
 *  File:       HelpAbout.cpp
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
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

#include "HelpAbout.h"
#include "HerculesStudio.h"
#include "Environment.h"
#include <QPixmap>
#include <QGraphicsView>
#include <QGraphicsItem>

HelpAbout::HelpAbout(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);

	ui.versionLabel->setText(HERCSTUDIO_VERSION);
	ui.stampLabel->setText("Built on: "__DATE__ " " __TIME__);

	static QString icon1 = Environment::getIconsPath().c_str();
	icon1 += "/opensource.png";
    static QPixmap * pixmap1 = new QPixmap(icon1);
    ui.label1->setPixmap(*pixmap1);

    static QString icon2 = Environment::getIconsPath().c_str();
	icon2 += "/tray";
    static QPixmap * pixmap2 = new QPixmap(icon2);
    ui.label2->setPixmap(*pixmap2);
    update();
    connect(ui.okButton, SIGNAL(clicked()), this, SLOT(okPressed()));

}

HelpAbout::~HelpAbout()
{

}

void HelpAbout::okPressed()
{
    close();
}
