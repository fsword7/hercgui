/*
 *  File:       IplConfig.cpp
 *
 *  Author:     Jacob Dekel
 *  Created on: June 22, 2010
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
#include "IplConfig.h"
#include "HerculesStudio.h"

#include <sstream>
#include <cstdio>


IplConfig::IplConfig(QString &devno, QString& loadParm, QWidget *parent)
    : QDialog(parent), mDevno(devno), mLoadParm(loadParm)
{
	ui.setupUi(this);
	ui.devNo->setText(mDevno);
	ui.loadParm->setText(mLoadParm);

	connect(ui.okButton, SIGNAL(clicked()), this, SLOT(okPressed()));
	connect(ui.cancelButton, SIGNAL(clicked()), this, SLOT(cancelPressed()));
}

IplConfig::~IplConfig()
{

}

void IplConfig::okPressed()
{
	mDevno = ui.devNo->text();
	mLoadParm = ui.loadParm->text();
	accept();
}

void IplConfig::cancelPressed()
{
	deleteLater();
}

