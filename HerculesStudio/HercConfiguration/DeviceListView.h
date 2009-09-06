/*
 *  File: DeviceListView.h
 *
 *  Author:     Jacob Dekel
 *  Created on:
 *
 *  Copyright (c) 2009 Jacob Dekel
 *
 *	This object manages the device list presented during run time by Devices
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

#ifndef DEVICELISTVIEW_H_
#define DEVICELISTVIEW_H_
#include "HerculesStudio.h"

#include <QListView>
#include <QStandardItemModel>
#include <QMenu>

class DeviceListView : public QListView
{
    Q_OBJECT
public:
    DeviceListView(QWidget * parent);
    virtual ~DeviceListView();

protected:
    void mousePressEvent ( QMouseEvent * e );

signals:
    void deviceClick(QMouseEvent *);
};

#endif /* DEVICELISTVIEW_H_ */
