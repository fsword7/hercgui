/*
 *  File:       TapeMap.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 26, 2010
 *
 *  Copyright (c) 2009-2010 Jacob Dekel
 *  $Id$
 *
 *  tapemap utility object
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
#ifndef TAPEMAP_H
#define TAPEMAP_H

#include "GenericUtility.h"

#include <QtGui/QDialog>
#include "ui_TapeMap.h"

#include <cstdio>
class UtilityExecutor;

class TapeMap : public GenericUtility
{
    Q_OBJECT

public:
    TapeMap(QWidget *parent = 0);
    ~TapeMap();

private slots:
    void exitClicked();
    void runClicked();
    void browseFileClicked();

private:
    Ui::TapeMapClass ui;
	void finishedSlot();

};

#endif // TAPEMAP_H
