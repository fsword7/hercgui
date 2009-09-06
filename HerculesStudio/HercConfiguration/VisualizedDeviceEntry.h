/*
 *  File: VisualizedDeviceEntry.h
 *
 *  Author:     Jacob Dekel
 *  Created on:
 *
 *  Copyright (c) 2009 Jacob Dekel
 *
 *	This is the base class for all configuration device entries
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

#ifndef DEVICEENTRY_H_
#define DEVICEENTRY_H_

#include "devices.h"

class VisualizedDeviceEntry
{
public:
    VisualizedDeviceEntry(int number, Devices::Type type, const std::string& definition);
    VisualizedDeviceEntry();
    virtual ~VisualizedDeviceEntry();

    inline int getNumber(){ return mNumber; }
    inline Devices::Type getType() { return mType; }
    inline QPixmap * getPixmap() { return mPixmap; }
    inline QIcon * getIcon() { return mIcon; }
    inline QStandardItem * getItem() { return mItem; }
    inline std::string getDefinition() { return mDefinition; }
    inline void setPixmap(QPixmap * w) { mPixmap=w; }
    inline void setIcon(QIcon * w) { mIcon=w; }
    inline void setItem(QStandardItem * w) { mItem=w; }

private:
    int mNumber;
    Devices::Type mType;
    std::string mDefinition;
    QPixmap * mPixmap;
    QIcon * mIcon;
    QStandardItem * mItem;
};
#endif /* DEVICEENTRY_H_ */
