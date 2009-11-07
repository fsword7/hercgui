/*
 *  File: Extended_Ui_CardReaderProperties.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id: Extended_Ui_CardReaderProperties.h 34 2009-11-07 06:15:58Z jacob $
 *
 *	This object extends the card reader gui class to include ReaderFileListView
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

#ifndef EXTENDED_UI_CARDREADERPROPERTIES_H_
#define EXTENDED_UI_CARDREADERPROPERTIES_H_

#include "ui_CardReaderProperties.h"
#include "ReaderFileListView.h"

class Extended_Ui_CardReaderProperties: public Ui::CardReaderPropertiesClass
{
public:
    Extended_Ui_CardReaderProperties();
    virtual ~Extended_Ui_CardReaderProperties();

    void setupUi(QWidget *CardReaderPropertiesClass);

    ReaderFileListView *fileNamesListView;
};

#endif /* EXTENDED_UI_CARDREADERPROPERTIES_H_ */
