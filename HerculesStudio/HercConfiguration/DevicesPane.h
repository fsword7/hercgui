/*
 *  File: DevicesPane.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id$
 *
 *	This object displays the devices pane during hercules run time
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

#ifndef DEVICESPANE_H
#define DEVICESPANE_H

#include "DeviceTypes.h"
#include "DeviceListView.h"
#include "StatusUpdateCollector.h"
#include "DeviceMenuProcessor.h"

#include <QtGui/QWidget>
#include "ui_Devices.h"
#include <QGridLayout>
#include <QScrollArea>
#include <QLabel>

#include <utility>
#include <vector>
#include <map>

typedef std::pair<int,int> TokenPair;

class VisualizedDeviceEntry;

/*
NOTE: look for BEGIN_DEVICE_CLASS_QUERY in hercules source

note: DEVA=0180 3390 DASD 1001 /media/Data/Hercules/zLinux/CentOS-4.7/180.cckd [3339 cyls]
*/

struct DynDeviceLine
{
    char    DEV[3];
    char    action;
    char    eq;
    char    devNo[4];
    char    pad1;
    char    devTypeN[4];
    char    pad2;
    char    devType[4];
    char    pid3;
    char    statusBytes[4];
};

class MainWindow;
class DeviceConfigLine;
class DevicesRename;
class DevicesPane : public DeviceMenuProcessor, public StatusUpdateCollector
{
	Q_OBJECT
public:
    DevicesPane(QWidget *parent = 0);
    virtual ~DevicesPane();

    virtual bool	notify(const QString& statusLine);
    void 			clear();

public slots:
    void mousePressed(QModelIndex index);
    void menuRename();
    void menuDelete();
    void doAddDevice(bool);
    void updateDevice(bool);
    void menuProperties();
    void doRename(QString oldDevNum, QString newDevNum);
    void menuInterrupt();
    void menuStatus();
    void menuTraceCCW();
    void rejected();

protected:
    virtual bool 				isRealDev(int lineNumber);
    virtual bool 				canAddSYSG();
    virtual bool 				isConfig() { return false; }
    virtual bool 				realDevice();
    virtual DeviceTypes::Type 	getType(int lineNumber);
    virtual void 				doLoadTape(QString& tapeFileName);
    virtual bool 				hasConfig();
    virtual bool				traced();

private:
    Ui::DevicesClass ui;
    MainWindow 				* mMainWindow;
    QGridLayout 			* mLayout;
    QScrollArea 			* mScrollArea;
    DeviceListView 			* mListView;
    std::vector<TokenPair> 	mTokens;
    std::map<int, VisualizedDeviceEntry> mDevices;
    QStandardItemModel 		* mModel;
    DevicesRename 			* mRenameDlg;
    int 					mOldDevNum;

    QSize 					sizeHint() const;
    void 					clearDevices(QWidget * parent);
    void 					resizeEvent ( QResizeEvent * event );
    void 					adjustSize();
    VisualizedDeviceEntry	* getDeviceEntry();
    QString 				textFromValue(int value) const;
    int                     getNextDev(std::map<int, VisualizedDeviceEntry>::iterator it) const;
    QIcon 					* mYellowIcon;
    QIcon 					* mYellowHighIcon;

signals:
    void restartDevices();
};

#endif // DEVICES_H
