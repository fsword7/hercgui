/*
 *  File:       HetUpd.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Sep 3, 2010
 *
 *  Copyright (c) 2009-2010 Jacob Dekel
 *  $Id$
 *
 *  Hetupd utility object
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

#ifndef HETUPD_H
#define HETUPD_H

#include "GenericUtility.h"

#include <QtGui/QDialog>
#include "ui_HetUpd.h"

class HetUpd : public GenericUtility
{
    Q_OBJECT

public:
    HetUpd(QWidget *parent = 0);
    ~HetUpd();

private:
    Ui::HetUpdClass ui;

    virtual void finishedSlot();

private slots:
    void exitClicked();
    void runClicked();
    void browseInFileClicked();
	void browseOutDirClicked();
	void browseOutFileClicked();
};

#endif // HETUPD_H
