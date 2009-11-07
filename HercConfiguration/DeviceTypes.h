/*
 *  File: DeviceTypes.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Oct 15, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id: DeviceTypes.h 34 2009-11-07 06:15:58Z jacob $
 *
 *  This object is an interface for driving devices through menus
 *  (used by both the devices pane and the devices configuration tab)
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


#ifndef DEVICETYPES_H_
#define DEVICETYPES_H_

#include <QIcon>

class DeviceTypes
{
public:
    DeviceTypes();
    virtual ~DeviceTypes();
    enum Type { Other = 0,
            Terminal, Sysg, Console, Printer, CardReader, CardPunch,
            LinePrinter, Tape, CTC, DASD , Comm };
    enum BaseType { CTCT=100 , CTCI, LCS, CKD, FBA};
    static QIcon & getIcon(DeviceTypes::Type type) ;
    static const char * getName(Type type);
private:
    const static char *sDeviceNames[];
    const static char *sDeviceIconNames[];
    static std::vector<QIcon *> sDeviceIcons;
};

#endif /* DEVICETYPES_H_ */
