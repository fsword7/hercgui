/*
 *  File:       Dasdcopy.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009-2013 Jacob Dekel
 *  $Id$
 *
 *	Dasdcopy utility object
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

#ifndef DASDCOPY_H
#define DASDCOPY_H

#include "GenericUtility.h"

#include <QtGui/QDialog>
#include "ui_Dasdcopy.h"

class UtilityExecutor;
class Dasdcopy : public GenericUtility
{
    Q_OBJECT

public:
    Dasdcopy(QWidget *parent = 0);
    ~Dasdcopy();

private:
    Ui::DasdcopyClass ui;
    bool mFirstEndReceived;
    
	virtual void finishedSlot();

private slots:
    void runClicked();
    void exitClicked();
    void browseFileClicked();
    void browseShadowClicked();
    void browseOutClicked();
    void runnerMaximumChanged(int maximum);
    void runnerValueChanged(int value);

};

#endif // DASDCOPY_H
