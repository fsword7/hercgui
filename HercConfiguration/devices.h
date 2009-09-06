/*
 *  File: Devices.h
 *
 *  Author:     Jacob Dekel
 *  Created on:
 *
 *  Copyright (c) 2009 Jacob Dekel
 *
 *	This objects displays the devices pane during hercules run time
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

#ifndef DEVICES_H
#define DEVICES_H

#include "DeviceListView.h"
#include "StatusUpdateCollector.h"

#include <QtGui/QWidget>
#include "ui_devices.h"
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

class Devices : public QWidget, public StatusUpdateCollector
{
    Q_OBJECT

public:
    enum Type { Other = 0,
            Terminal, Sysg, Console, Printer, CardReader, CardPunch,
            LinePrinter, Tape, CTC, DASD , Comm };
    enum BaseType { CTCT=100 , CTCI, LCS, CKD, FBA};


const static char *sDeviceNames[];
const static char *sDeviceIconNames[];
static std::vector<QIcon *> sDeviceIcons;

    Devices(QWidget *parent = 0);
    virtual ~Devices();

    void notify(const std::string& statusLine);
    void clear();
    static QIcon & getIcon(Devices::Type type) ;

public slots:
    void mousePressed(QModelIndex index);
    void devClick(QMouseEvent * event);
    void menuDelete();

private:
    Ui::DevicesClass ui;
    QGridLayout *mLayout;
    QScrollArea *mScrollArea;
    DeviceListView *mListView;
    std::vector<TokenPair> mTokens;
    std::map<int, VisualizedDeviceEntry> mDevices;
    QStandardItemModel *mModel;
    QMenu mMenu;
    int mLastClick;

    QSize sizeHint() const;
    Type getType(const std::string& dev);
    void clearDevices(QWidget * parent);
    void resizeEvent ( QResizeEvent * event );
    void adjustSize();

    QIcon * mYellowIcon;
    QIcon * mYellowHighIcon;
};

#endif // DEVICES_H
