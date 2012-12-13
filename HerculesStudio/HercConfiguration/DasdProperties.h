/*
 *  File: DasdProperties.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009-2013 Jacob Dekel
 *  $Id$
 *
 *
 *  This object handles the configuration of DASDs
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

#ifndef DASDPROPERTIES_H
#define DASDPROPERTIES_H

#include "ConfigLine.h"
#include "GenericDeviceProperties.h"

#include <QtGui/QWidget>
#include "ui_DasdProperties.h"

typedef std::string stringArray[3];

class DasdProperties : public GenericDeviceProperties
{
    Q_OBJECT

public:
    DasdProperties(ConfigLine& configLine, QWidget *parent = 0);
    virtual ~DasdProperties();

private:
    Ui::DasdPropertiesClass ui;
    static std::string ckd[];
    static std::string fba[];

    void initialize(ConfigLine& line);
    bool isInet(const std::string& fileName, stringArray & vec);
    bool isFba(const std::string& dev);
    void setInet(bool set);
    void setLocal(bool set);
    void setFba(bool set);
    void setCkd(bool set);

private slots:
    virtual void ok();
    virtual void cancel();
    void browseClicked();
    void browseShadowClicked();
    void typeChanged(const QString &);
    void localInetChanged();
};

#endif // DASDPROPERTIES_H
