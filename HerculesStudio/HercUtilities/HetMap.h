/*
 *  File:       HetMap.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Sep 6, 2010
 *
 *  Copyright (c) 2009-2010 Jacob Dekel
 *  $Id$
 *
 *  Hetmap utility object
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

#ifndef HETMAP_H
#define HETMAP_H

#include "GenericUtility.h"

#include <QtGui/QDialog>
#include "ui_HetMap.h"

class HetMap : public GenericUtility
{
    Q_OBJECT

public:
    HetMap(QWidget *parent = 0);
    ~HetMap();

private:
    Ui::HetMapClass ui;
	bool mInternalChange; // check box changed internally - no need to propagate

    virtual void finishedSlot();

private slots:
    void exitClicked();
    void runClicked();
	void allClicked();
	void fileInfoClicked();
	void labelInfoClicked();
	void dataInfoClicked();
	void tapemapClicked();
	void browseFileClicked();
};

#endif // HETMAP_H
