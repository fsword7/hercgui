/*
 *  File: GenericDeviceProperties.h
 *
 *  Author:     Jacob Dekel
 *  Created on:
 *
 *  Copyright (c) 2009 Jacob Dekel
 *
 *	This is the base class for all devices properties dialogs
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

#ifndef GENERICDEVICEPROPERTIES_H_
#define GENERICDEVICEPROPERTIES_H_

#include "DeviceConfigLine.h"
#include <QtGui/QDialog>

class GenericDeviceProperties : public QDialog
{
    Q_OBJECT
public:
    GenericDeviceProperties(ConfigLine& line, QWidget *parent);
    virtual ~GenericDeviceProperties();

    static GenericDeviceProperties * classFactory(DeviceConfigLine& line, QWidget *parent, Devices::Type type=Devices::Other);

protected:
    mutable ConfigLine & mLine;
    QWidget *mParent;

    bool  deviceNumberValidator(QLineEdit * deviceLine);
    bool ipValidator(QLineEdit * ipLineEdit, bool allowNull);
    int   mOriginalDeviceNumber;

private slots:
    virtual void ok()=0;
    virtual void cancel()=0;

signals:
    void updateLine(bool keep);

};

#endif /* GENERICDEVICEPROPERTIES_H_ */
