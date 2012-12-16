/*
 *  File: DeviceConfigLine.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009-2013 Jacob Dekel
 *  $Id$
 *
 *	This object manages one configuration line belonging to devices
 *	(see also ConfigLine.h)
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

#ifndef DEVICECONFIGLINE_H_
#define DEVICECONFIGLINE_H_

#include "ConfigLine.h"
#include "DeviceTypes.h"

#include <map>

class DeviceConfigLine: public ConfigLine
{
public:
    typedef std::map<std::string, DeviceTypes::Type> DeviceTypesMap;
    DeviceConfigLine(const char *line);
    virtual ~DeviceConfigLine();
	static void clear();

    int getDeviceNumber() const;
    DeviceTypes::Type getDeviceType() const;
    std::string toString() const;
    int getMultiCount();

private:
    static std::map<std::string,DeviceTypes::Type> * mDeviceTypes;
    static std::map<std::string,DeviceTypes::BaseType> * mDeviceBaseTypes;
    int mMultiCount;

    void initilize();
    void handleMultiLine();
};

#endif /* DEVICECONFIGLINE_H_ */
