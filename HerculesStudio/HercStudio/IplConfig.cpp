/*
 *  File:       IplConfig.cpp
 *
 *  Author:     Jacob Dekel
 *  Created on: June 22, 2010
 *
 *  Copyright (c) 2009-2010 Jacob Dekel
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
#include "IplConfig.h"
#include "HerculesStudio.h"

#include <sstream>
#include <cstdio>


IplConfig::IplConfig(int loadAddr, const char * loadParm, QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);
	ui.devNo->setText(textFromValue(loadAddr));
	ui.loadParm->setText(loadParm);

	connect(ui.okButton, SIGNAL(clicked()), this, SLOT(okPressed()));
	connect(ui.cancelButton, SIGNAL(clicked()), this, SLOT(cancelPressed()));
}

IplConfig::~IplConfig()
{

}

void IplConfig::okPressed()
{
	emit doIpl(ui.devNo->text(), ui.loadParm->text());
	deleteLater();
}

void IplConfig::cancelPressed()
{
	deleteLater();
}

QString IplConfig::textFromValue(int value) const
{
	std::stringstream ss;
	ss << std::hex << value;
	char formatted[5];
	if (value <= 0xffff)
		sprintf(formatted,"%4.4X",(value));
	else
		strcpy(formatted,"0000");
	QString ret(formatted);
	outDebug(5,std::cout << "textFromValue:" << value << "='" << ret.toStdString() << std::endl;)
	return ret;
}

