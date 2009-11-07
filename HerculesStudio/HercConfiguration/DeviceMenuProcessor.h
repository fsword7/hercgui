 /*
 *  File: DeviceMenuProcessor.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Oct 14, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id: DeviceMenuProcessor.h 34 2009-11-07 06:15:58Z jacob $
 *
 *	This object is an interface for driving devices through menus
 *	(used by both the devices pane and the devices configuration tab)
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
#ifndef DEVICEMENUPROCESSOR_H_
#define DEVICEMENUPROCESSOR_H_

#include "DeviceConfigLine.h"
#include "DeviceTypes.h"

#include <QtGui/QWidget>
#include <QMenu>

class DeviceMenuProcessor : public QWidget
{
	Q_OBJECT
public:
	DeviceMenuProcessor(QWidget *parent = 0);
	virtual ~DeviceMenuProcessor();

public slots:
    void devicesClick(QMouseEvent * event);
	void menuAddTerminal();
    void menuAddConsole();
    void menuAddPrinter();
    void menuAddCardReader();
    void menuAddCardPunch();
    void menuAddTape();
    void menuAddCTC();
    void menuAddDasd();
    void menuAddSYSG();
    void menuLoad();
    void newTapeFileOK(QString& tapeFile);

protected:
    virtual bool isRealDev(int lineNumber) = 0;
    virtual bool canAddSYSG() = 0;
    virtual bool isConfig() = 0;
    virtual bool addMode() = 0;
    virtual DeviceTypes::Type getType(int lineNumber) = 0;
    virtual void doLoadTape(QString& tapeFileName) = 0;
    virtual bool hasConfig() = 0;
    virtual bool traced() = 0;

    int mClickRow;
    int mLastClick;
    QMenu mMenu;
    DeviceConfigLine * mCandidateLine;

private:
    void menuAddItem(int);
};

#endif /* DEVICEMENUPROCESSOR_H_ */
