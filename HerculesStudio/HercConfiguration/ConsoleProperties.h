/*
 *  File: ConsoleProperties.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009-2013 Jacob Dekel
 *  $Id$
 *
 *  This object handles the configuration of the console
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

#ifndef CONSOLEPROPERTIES_H
#define CONSOLEPROPERTIES_H

#include "ConfigLine.h"
#include "GenericDeviceProperties.h"

#include "ui_ConsoleProperties.h"

class ConsoleProperties : public GenericDeviceProperties
{
    Q_OBJECT

public:
    ConsoleProperties(ConfigLine & line, QWidget *parent = 0);
    virtual ~ConsoleProperties();

private:
    Ui::ConsolePropertiesClass ui;
    ConfigLine & mLine;

    void setIntegrated(bool integrated);

signals:
    void updateLine(bool);

private slots:
    void ok();
    void cancel();
    void deviceTypeChanged(int index);
};

#endif // CONSOLEPROPERTIES_H
