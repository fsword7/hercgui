/*
 *  File: TapeProperties.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009-2013 Jacob Dekel
 *  $Id$
 *
 *	This object manages the tape configuration properties
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

#ifndef TAPEPROPERTIES_H
#define TAPEPROPERTIES_H

#include "ConfigLine.h"
#include "GenericDeviceProperties.h"

#include <QtWidgets/QWidget>
#include "ui_TapeProperties.h"

class TapeProperties : public GenericDeviceProperties
{
    Q_OBJECT

public:
    TapeProperties(ConfigLine& line,QWidget *parent = 0);
    virtual ~TapeProperties();

private:
    Ui::TapePropertiesClass ui;
    static const std::string types[];

    void initialize(ConfigLine & line);

private slots:
    void ok();
    void cancel();
    void standard(bool set=true);
    void scsi(bool set=true);
    void fileBrowse();
};

#endif // TAPEPROPERTIES_H
