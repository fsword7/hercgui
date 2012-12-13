/*
 *  File:       TapeCopy.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 27, 2010
 *
 *  Copyright (c) 2009-2013 Jacob Dekel
 *  $Id$
 *
 *  tapecopy utility object
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
#ifndef TAPECOPY_H
#define TAPECOPY_H

#include "GenericUtility.h"

#include <QtGui/QDialog>
#include "ui_TapeCopy.h"

class TapeCopy : public GenericUtility
{
    Q_OBJECT

public:
    TapeCopy(QWidget *parent = 0);
    ~TapeCopy();

private:
    Ui::TapeCopyClass ui;
    QRect mUpperGeometry, mLowerGeometry;

	virtual void finishedSlot();

public slots:
	void runClicked();
	void cancelClicked();
	void browseFileClicked();
	void tapeToScsi();
	void scsiToTape();

};

#endif // TAPECOPY_H
