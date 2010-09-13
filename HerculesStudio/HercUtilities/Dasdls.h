/*
 *  File:       Dasdls.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id$
 *
 *	Dasdls utility object
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

#ifndef DASDLS_H
#define DASDLS_H

#include "GenericUtility.h"

#include <QtGui/QDialog>
#include "ui_Dasdls.h"

class Dasdls : public GenericUtility
{
    Q_OBJECT

public:
    Dasdls(QWidget *parent = 0);
    ~Dasdls();

private:
    Ui::DasdlsClass ui;
    int		mMaximum;

	virtual void finishedSlot();

private slots:
    void exitClicked();
    void runClicked();
    void browseFileClicked();
    void browseSfClicked();
    void runnerMaximumChanged(int maximum);
    void runnerValueChanged(int value);
};

#endif // DASDLS_H
