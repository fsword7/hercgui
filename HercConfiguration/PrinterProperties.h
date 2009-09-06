/*
 *  File: PrinterProperties.h
 *
 *  Author:     Jacob Dekel
 *  Created on:
 *
 *  Copyright (c) 2009 Jacob Dekel
 *
 *	This object manages the printer configuration properties
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

#ifndef PRINTERPROPERTIES_H
#define PRINTERPROPERTIES_H

#include "ConfigLine.h"
#include "GenericDeviceProperties.h"
#include "HerculesStudio.h"

#include "ui_PrinterProperties.h"

class PrinterProperties : public GenericDeviceProperties
{
    Q_OBJECT

public:
    PrinterProperties(ConfigLine& line,QWidget *parent = 0);
    ~PrinterProperties();
    void setCrlfEnabled(bool enabled);

private:
    Ui::PrinterPropertiesClass ui;

private slots:
    void ok();
    void cancel();
    void fileNameBrowse();
};

class PrinterFileNameValidator : public QValidator
{
public:
    PrinterFileNameValidator(QObject * parent);
    virtual QValidator::State validate(QString&, int&) const;

private:
    PrinterProperties * mParent;
};

#endif // PRINTERPROPERTIES_H
