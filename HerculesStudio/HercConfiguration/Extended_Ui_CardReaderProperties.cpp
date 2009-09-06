/*
 *  File: Extended_Ui_CardReaderProperties.cpp
 *
 *  Author:     Jacob Dekel
 *  Created on:
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

#include "Extended_Ui_CardReaderProperties.h"

Extended_Ui_CardReaderProperties::Extended_Ui_CardReaderProperties()
{
}

Extended_Ui_CardReaderProperties::~Extended_Ui_CardReaderProperties()
{
}

void Extended_Ui_CardReaderProperties::setupUi(QWidget *widget)
{
    CardReaderPropertiesClass::setupUi(widget);
    fileNamesListView = new ReaderFileListView(groupBox_2);
    fileNamesListView->setObjectName(QString::fromUtf8("fileNamesListView"));
    fileNamesListView->setGeometry(QRect(90, 90, 331, 168));
    fileNamesListView->setMovement(QListView::Snap);
}
