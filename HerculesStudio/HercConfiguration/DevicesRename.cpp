/*
 *  File: DevicesRename.cpp
 *
 *  Author:     Jacob Dekel
 *  Created on: Nov 1, 2009
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
#include "DevicesRename.h"

DevicesRename::DevicesRename(int value, int newValue, QWidget *parent)
    : QDialog(parent), mInitialDevNum(value)
{
	ui.setupUi(this);

	mSpinBox = new Configuration::HexSpinBox(this);
	mSpinBox->move(180,40);
	mSpinBox->setValue(newValue);

	QString from = mSpinBox->textFromValue(value);
	ui.fromValue->setText(from);

	connect(ui.buttonBox,SIGNAL(accepted()), this, SLOT(ok()));
	connect(ui.buttonBox,SIGNAL(rejected()), this, SLOT(notok()));
}

DevicesRename::~DevicesRename()
{

}

void DevicesRename::ok()
{
	emit accepted(mSpinBox->textFromValue(mInitialDevNum), mSpinBox->textFromValue(mSpinBox->value()));
}

void DevicesRename::notok()
{
	emit rejected();
}
