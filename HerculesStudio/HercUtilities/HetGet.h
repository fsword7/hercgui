/*
 *  File:       HetGet.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Sep 3, 2010
 *
 *  Copyright (c) 2009-2013 Jacob Dekel
 *  $Id$
 *
 *  Hetget utility object
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

#ifndef HETGET_H
#define HETGET_H

#include "GenericUtility.h"

#include <QtWidgets/QDialog>
#include "ui_HetGet.h"

class HetGet : public GenericUtility
{
    Q_OBJECT

public:
    HetGet(QWidget *parent = 0);
    ~HetGet();

private:
    Ui::HetGetClass ui;

    virtual void finishedSlot();

private slots:
    void exitClicked();
    void runClicked();
    void browseInFileClicked();
    void browseOutDirClicked();
    void browseOutFileClicked();
	void asciiChanged();
    void stripChanged();
    void unblockChanged();
};

#endif // HETGET_H
