/*
 *  File: DevicesWidget.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id: DevicesWidget.h 34 2009-11-07 06:15:58Z jacob $
 *
 *	This object presents the devices section in the configuration
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

#ifndef DEVICESDLG_H_
#define DEVICESDLG_H_

#include "ConfigFile.h"
#include "DeviceConfigView.h"
#include "DeviceMenuProcessor.h"

#include <QWidget>
#include <QFrame>
#include <QIcon>

class DevicesWidget : public DeviceMenuProcessor
{
    Q_OBJECT
public:
    DevicesWidget(ConfigFile * configFile, QWidget * parent);
    virtual ~DevicesWidget();
    bool isUnique(std::string devno);

private:
    ConfigFile * mConfigFile;
    DeviceConfigView * mConfigView;
    QMenu mMenu;
    std::map<int,int> mDialogToFile;
    QIcon *mCardIcon;
    QIcon *mConsoleIcon;
    QIcon *mDasdIcon;
    QIcon *mNetIcon;
    QIcon *mPrinterIcon;
    QIcon *mTapeIcon;
    QIcon *mTerminalIcon;
    QIcon *mUnknownIcon;

    void initialize();
    virtual bool isRealDev(int) { return true;}
    bool canAddSYSG();
    bool isConfig() { return true; };
    bool realDevice();
    DeviceTypes::Type getType(int lineNumber);
    virtual void doLoadTape(QString&) { return; }
    virtual bool hasConfig() { return true; };
    virtual bool traced() { return false; }

private slots:
    void mousePressed(QModelIndex index);
    void doAddDevice(bool keep);
    void updateDevice(bool);

public slots:

    void menuDelete();
    void menuProperties();
};

#endif /* DEVICESDLG_H_ */
