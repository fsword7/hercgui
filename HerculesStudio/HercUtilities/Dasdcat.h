/*
 *  File:       Dasdcat.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id$
 *
 *	Dasdcat utility object
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

#ifndef DASDCAT_H
#define DASDCAT_H

#include "GenericUtility.h"

#include <QtGui/QDialog>
#include "ui_Dasdcat.h"

class UtilityExecutor;

class Dasdcat : public GenericUtility
{
    Q_OBJECT

public:
    Dasdcat(QWidget *parent = 0);
    ~Dasdcat();

private:
    Ui::DasdcatClass ui;

    virtual void finishedSlot();

private slots:
    void exitClicked();
    void runClicked();
    void browseFileClicked();
    void browseSfClicked();
    void specificToggeled(bool);
};

#endif // DASDCAT_H
