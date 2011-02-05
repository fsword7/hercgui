/*
 *  File: DeviceTypes.cpp
 *
 *  Author:     Jacob Dekel
 *  Created on: Oct 15, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id$
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

#include "HerculesStudio.h"
#include "DeviceTypes.h"
#include "Environment.h"


#define NUM_DEVICES 12
std::vector<QIcon *> DeviceTypes::sDeviceIcons;

const char *DeviceTypes::sDeviceNames[NUM_DEVICES] =
{
    "Unknown",
    "Display Terminal",
    "SYSG",
    "Consoles",
    "Printers",
    "Card Readers",
    "Card Punch",
    "Line Printers",
    "Tape Drives",
    "CTC",
    "Dasd",
    "OSA"
};

const char *DeviceTypes::sDeviceIconNames[NUM_DEVICES] =
{
    "/unknown.jpg",   //0
    "/terminal.jpg",  //1
    "/terminal.jpg",  //2
    "/console.jpg",   //3
    "/printer.jpg",   //4
    "/card.jpg",   //5
    "/unknown.jpg",   //6
    "/printer.jpg",   //7
    "/tape.jpg",      //8
    "/net.jpg",       //9
    "/dasd.jpg",      //10
    "/net.jpg"        //11
};


DeviceTypes::DeviceTypes()
{
}

DeviceTypes::~DeviceTypes()
{
}

QIcon & DeviceTypes::getIcon(DeviceTypes::Type type)
{
    if  (sDeviceIcons.empty())
    {
        std::string iconsPath = Environment::getIconsPath();
        for (int i=0; i<NUM_DEVICES; i++)
        {
            QString iPath = iconsPath.c_str();
            iPath += sDeviceIconNames[i];
            hOutDebug(5,i <<" ipath:"<<iPath.toStdString());
            sDeviceIcons.push_back(new QIcon(iPath));
        }
    }
    return *sDeviceIcons.at(type);
}

const char *DeviceTypes::getName(Type type)
{
    if  (type < 0 || type > NUM_DEVICES) return NULL;
    return sDeviceNames[type];
}
