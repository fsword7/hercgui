/*
 *  File: DeviceConfigLine.cpp
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

#include "DeviceConfigLine.h"
#include "ConfigurationEditor.h"

#include <cstdlib>
#include <sstream>


DeviceConfigLine::DeviceTypes *DeviceConfigLine::mDeviceTypes = NULL;
std::map<std::string,Devices::BaseType> *DeviceConfigLine::mDeviceBaseTypes = NULL;

DeviceConfigLine::DeviceConfigLine(const char *line):
    ConfigLine(line), mMultiCount(0)
{
    initilize();
    handleMultiLine();
}

DeviceConfigLine::~DeviceConfigLine()
{
    outDebug(5,std::cout << "~DeviceConfigLine" << std::endl);
}

int DeviceConfigLine::getDeviceNumber() const
{
    return std::strtol(getToken(0).c_str(),NULL, 16);
}

Devices::Type DeviceConfigLine::getDeviceType() const
{
    QString token = getToken(1).c_str();
    DeviceTypes::iterator it = (*mDeviceTypes).find(token.toUpper().toStdString());
    if (it == (*mDeviceTypes).end())
        return (*mDeviceTypes)["0000"];
    outDebug(5, std::cout << "getDeviceType " << token.toStdString() << std::endl);
    return it->second;
}

void DeviceConfigLine::handleMultiLine()
{
    std::string devno = getToken(0);
    int first = -1, last = -1;
    size_t dash = devno.find_first_of("-");
    if (dash != std::string::npos)
    {
        first = ConfigurationEditor::parseNum(devno.substr(0,dash), 16);
        last = ConfigurationEditor::parseNum(devno.substr(dash+1), 16);
    }
    size_t  dot = devno.find_first_of(".");
    if (dot != std::string::npos)
    {
        first = ConfigurationEditor::parseNum(devno.substr(0,dot), 16);
        last = first+ConfigurationEditor::parseNum(devno.substr(dot+1), 10)-1;
    }

    if (first > -1)
    {
        mMultiCount = last-first+1;
        outDebug(5, std::cout << devno << " multiCount:" << mMultiCount << std::endl);
        std::stringstream line;
        line << std::hex << first ;
        line << " " + getMultiToken(1,0) + "\n";
        replaceLine(line.str());
    }
    else
    {
        first = ConfigurationEditor::parseNum(devno, 16);
    }
}

int DeviceConfigLine::getMultiCount()
{
    outDebug(5, std::cout << "get multiCount:" << mMultiCount << std::endl);
    return mMultiCount;
}

void DeviceConfigLine::initilize()
{
    if (mDeviceTypes != NULL)
        return;

    mDeviceTypes = new std::map<std::string,Devices::Type>;
    mDeviceBaseTypes = new std::map<std::string,Devices::BaseType>;

    mDeviceTypes->insert(std::pair<std::string,Devices::Type>("0000",Devices::Other));
    mDeviceTypes->insert(std::pair<std::string,Devices::Type>("3270",Devices::Terminal));
    mDeviceTypes->insert(std::pair<std::string,Devices::Type>("3287",Devices::Terminal));
    mDeviceTypes->insert(std::pair<std::string,Devices::Type>("SYSG",Devices::Sysg));
    mDeviceTypes->insert(std::pair<std::string,Devices::Type>("1052",Devices::Console));
    mDeviceTypes->insert(std::pair<std::string,Devices::Type>("3215",Devices::Console));
    mDeviceTypes->insert(std::pair<std::string,Devices::Type>("1052-C",Devices::Console));
    mDeviceTypes->insert(std::pair<std::string,Devices::Type>("3215-C",Devices::Console));
    mDeviceTypes->insert(std::pair<std::string,Devices::Type>("1442",Devices::CardReader));
    mDeviceTypes->insert(std::pair<std::string,Devices::Type>("2501",Devices::CardReader));
    mDeviceTypes->insert(std::pair<std::string,Devices::Type>("3505",Devices::CardReader));
    mDeviceTypes->insert(std::pair<std::string,Devices::Type>("3525",Devices::CardPunch));
    mDeviceTypes->insert(std::pair<std::string,Devices::Type>("1403",Devices::Printer));
    mDeviceTypes->insert(std::pair<std::string,Devices::Type>("3211",Devices::Printer));
    mDeviceTypes->insert(std::pair<std::string,Devices::Type>("3410",Devices::Tape));
    mDeviceTypes->insert(std::pair<std::string,Devices::Type>("3420",Devices::Tape));
    mDeviceTypes->insert(std::pair<std::string,Devices::Type>("3422",Devices::Tape));
    mDeviceTypes->insert(std::pair<std::string,Devices::Type>("3430",Devices::Tape));
    mDeviceTypes->insert(std::pair<std::string,Devices::Type>("3480",Devices::Tape));
    mDeviceTypes->insert(std::pair<std::string,Devices::Type>("3490",Devices::Tape));
    mDeviceTypes->insert(std::pair<std::string,Devices::Type>("3590",Devices::Tape));
    mDeviceTypes->insert(std::pair<std::string,Devices::Type>("9347",Devices::Tape));
    mDeviceTypes->insert(std::pair<std::string,Devices::Type>("8809",Devices::Tape));
    mDeviceTypes->insert(std::pair<std::string,Devices::Type>("3088",Devices::CTC));
    mDeviceTypes->insert(std::pair<std::string,Devices::Type>("CTCI",Devices::CTC));
    mDeviceTypes->insert(std::pair<std::string,Devices::Type>("LCS", Devices::CTC));
    mDeviceTypes->insert(std::pair<std::string,Devices::Type>("CTCT", Devices::CTC));
    mDeviceTypes->insert(std::pair<std::string,Devices::Type>("2305",Devices::DASD));
    mDeviceTypes->insert(std::pair<std::string,Devices::Type>("2311",Devices::DASD));
    mDeviceTypes->insert(std::pair<std::string,Devices::Type>("2314",Devices::DASD));
    mDeviceTypes->insert(std::pair<std::string,Devices::Type>("3330",Devices::DASD));
    mDeviceTypes->insert(std::pair<std::string,Devices::Type>("3340",Devices::DASD));
    mDeviceTypes->insert(std::pair<std::string,Devices::Type>("3350",Devices::DASD));
    mDeviceTypes->insert(std::pair<std::string,Devices::Type>("3375",Devices::DASD));
    mDeviceTypes->insert(std::pair<std::string,Devices::Type>("3380",Devices::DASD));
    mDeviceTypes->insert(std::pair<std::string,Devices::Type>("3390",Devices::DASD));
    mDeviceTypes->insert(std::pair<std::string,Devices::Type>("9345",Devices::DASD));
    mDeviceTypes->insert(std::pair<std::string,Devices::Type>("3310",Devices::DASD));
    mDeviceTypes->insert(std::pair<std::string,Devices::Type>("3370",Devices::DASD));
    mDeviceTypes->insert(std::pair<std::string,Devices::Type>("9332",Devices::DASD));
    mDeviceTypes->insert(std::pair<std::string,Devices::Type>("9335",Devices::DASD));
    mDeviceTypes->insert(std::pair<std::string,Devices::Type>("9336",Devices::DASD));
    mDeviceTypes->insert(std::pair<std::string,Devices::Type>("0671",Devices::DASD));
    mDeviceTypes->insert(std::pair<std::string,Devices::Type>("2703",Devices::Comm));

    mDeviceBaseTypes->insert(std::pair<std::string,Devices::BaseType>("3088",Devices::CTCT));
    mDeviceBaseTypes->insert(std::pair<std::string,Devices::BaseType>("CTCI",Devices::CTCI));
    mDeviceBaseTypes->insert(std::pair<std::string,Devices::BaseType>("LCS", Devices::LCS));
    mDeviceBaseTypes->insert(std::pair<std::string,Devices::BaseType>("2305",Devices::CKD));
    mDeviceBaseTypes->insert(std::pair<std::string,Devices::BaseType>("2311",Devices::CKD));
    mDeviceBaseTypes->insert(std::pair<std::string,Devices::BaseType>("2314",Devices::CKD));
    mDeviceBaseTypes->insert(std::pair<std::string,Devices::BaseType>("3330",Devices::CKD));
    mDeviceBaseTypes->insert(std::pair<std::string,Devices::BaseType>("3340",Devices::CKD));
    mDeviceBaseTypes->insert(std::pair<std::string,Devices::BaseType>("3350",Devices::CKD));
    mDeviceBaseTypes->insert(std::pair<std::string,Devices::BaseType>("3375",Devices::CKD));
    mDeviceBaseTypes->insert(std::pair<std::string,Devices::BaseType>("3380",Devices::CKD));
    mDeviceBaseTypes->insert(std::pair<std::string,Devices::BaseType>("3390",Devices::CKD));
    mDeviceBaseTypes->insert(std::pair<std::string,Devices::BaseType>("9345",Devices::CKD));
    mDeviceBaseTypes->insert(std::pair<std::string,Devices::BaseType>("3310",Devices::FBA));
    mDeviceBaseTypes->insert(std::pair<std::string,Devices::BaseType>("3370",Devices::FBA));
    mDeviceBaseTypes->insert(std::pair<std::string,Devices::BaseType>("9332",Devices::FBA));
    mDeviceBaseTypes->insert(std::pair<std::string,Devices::BaseType>("9335",Devices::FBA));
    mDeviceBaseTypes->insert(std::pair<std::string,Devices::BaseType>("9336",Devices::FBA));
    mDeviceBaseTypes->insert(std::pair<std::string,Devices::BaseType>("9336",Devices::FBA));

}

std::string DeviceConfigLine::toString() const
{
    int i=mLine.length()-3;
    while (i>=0)
    {
        if ( (mLine[i] != ' ') && (mLine[i] != '\t') && (mLine[i] != '\n') && (mLine[i] != '\r') )
            break;
        --i;
    }
    return mLine.substr(0,i);
}
