/*
 *  File: VisualizedDeviceEntry.cpp
 *
 *  Author:     Jacob Dekel
 *  Created on:
 *
 *  Copyright (c) 2009 Jacob Dekel
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

#include "VisualizedDeviceEntry.h"

VisualizedDeviceEntry::VisualizedDeviceEntry(int number, Devices::Type type, const std::string& definition)
: mNumber(number), mType(type), mDefinition(definition), mPixmap(NULL), mIcon(NULL), mItem(NULL)
{
}

VisualizedDeviceEntry::VisualizedDeviceEntry()
{}

VisualizedDeviceEntry::~VisualizedDeviceEntry()
{

}
