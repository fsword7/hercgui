/*
 *  File: CtcProperties.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009-2013 Jacob Dekel
 *
 *  This object handles the configuration of the CTC
 *  $Id$
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

#ifndef CTCPROPERTIES_H
#define CTCPROPERTIES_H

#include "ConfigLine.h"
#include "GenericDeviceProperties.h"

#include <QtWidgets/QWidget>
#include "ui_CtcProperties.h"

class CtcProperties : public GenericDeviceProperties
{
    Q_OBJECT

public:
    CtcProperties(ConfigLine& configLine, QWidget *parent = 0);
    virtual ~CtcProperties();

private:
    Ui::CtcPropertiesClass ui;

    bool macValidator(const std::string& mac);
    void arrangeQeth(bool set);
    void arrangeLcs(bool set);
    void arrangeCtc(bool set);
    void arrangeCtct(bool set);
    void arrangeByType();
    void initialize(ConfigLine & configLine);

private slots:
    virtual void ok();
    virtual void cancel();
    virtual void oatBrowse();
    void typeChanged(const QString &);
    void tun0Check(int);
};

#endif // CTCPROPERTIES_H
