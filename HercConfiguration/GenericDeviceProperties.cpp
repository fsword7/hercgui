/*
 *  File: GenericDeviceProperties.cpp
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

#include "GenericDeviceProperties.h"
#include "TerminalProperties.h"
#include "SysgProperties.h"
#include "ConsoleProperties.h"
#include "PrinterProperties.h"
#include "CardReaderProperties.h"
#include "PunchProperties.h"
#include "TapeProperties.h"
#include "CtcProperties.h"
#include "DasdProperties.h"
#include "DevicesWidget.h"
#include "ConfigurationEditor.h"

#include <QMessageBox>

#include <sstream>
#include <iomanip>

GenericDeviceProperties::GenericDeviceProperties(ConfigLine& line, QWidget *parent)
 : QDialog(parent) ,  mLine(line), mParent(parent),
   mOriginalDeviceNumber(ConfigurationEditor::parseNum(mLine.getToken(0),16))
{


}

GenericDeviceProperties::~GenericDeviceProperties()
{

}

GenericDeviceProperties * GenericDeviceProperties::classFactory(DeviceConfigLine& line, QWidget *parent, Devices::Type type)
{
    if (type == 0)
        type = line.getDeviceType();
    outDebug(5, std::cout << "GenericDeviceProperties::classFactory " << type << std::endl);
    switch(type)
    {
    case Devices::Terminal:
        return new TerminalProperties(line,parent);
    case Devices::Console:
        return new ConsoleProperties(line,parent);
    case Devices::Printer:
        return new PrinterProperties(line,parent);
    case Devices::CardReader:
        return new CardReaderProperties(line,parent);
    case Devices::CardPunch:
        return new PunchProperties(line,parent);
    case Devices::Tape:
        return new TapeProperties(line,parent);
    case Devices::CTC:
        return new CtcProperties(line,parent);
    case Devices::DASD:
        return new DasdProperties(line,parent);
    case Devices::Sysg:
        return new SysgProperties(line,parent);
    default:
        throw hsException("Invalid device, "+ line.getToken(1));
    }
}

bool GenericDeviceProperties::deviceNumberValidator(QLineEdit * deviceLine)
{
    // check that field is not empty
    if (deviceLine->text().isEmpty() || !ConfigurationEditor::validateNum(deviceLine->text().toStdString(),16))
    {
        QMessageBox * p = new QMessageBox(this);
        p->setText("Please enter device number in the range 0000-ffff");
        p->exec();
        deviceLine->setFocus();
        return false;
    }

    //make sure device number is new/unique
    DevicesWidget * dev = static_cast<DevicesWidget *>(mParent);
    std::stringstream buff;
    int iDevno = ConfigurationEditor::parseNum(deviceLine->text().toStdString(),16);
    if (iDevno != mOriginalDeviceNumber)
    {
        buff << std::hex << std::setw(4) << std::setfill('0') << std::uppercase
             << iDevno;
        std::string devno = buff.str();
        if (!dev->isUnique(devno))
        {
            QMessageBox * p = new QMessageBox(this);
            p->setText("This device number is already used");
            p->exec();
            return false;
        }
    }
    return true;

}

bool GenericDeviceProperties::ipValidator(QLineEdit * ipLineEdit, bool allowNull)
{
	const std::string ip = ipLineEdit->text().toStdString();
    bool ret = true;
    if (allowNull && ip.compare("...") == 0)
    	return ret;
    QString qip = ip.c_str();
    int pos=0;
    for (int i=0; i<4 && ret; i++)
    {
        outDebug(5, std::cout << "IP:" << ip << " pos:" << pos << " i=" << i << std::endl);
        if (i>0)
        {
            if (qip.toLatin1().data()[pos] != '.')
            {
                ret = false;
                break;
            }
            pos++;
        }
        int num = 0;
        int ppos=pos;
        while(ip[pos] >= '0' && ip[pos] <= '9')
        {
            num = num*10 + ip[pos] - '0';
            pos++;
        }
        if (pos == ppos || num > 255)
        {
            ret = false;
            break;
        }
    }
    if (!ret)
    {
        QString msg = ip.c_str();
        msg += " is illagel IP address";
        QMessageBox::warning(this, "Illegal IP address", msg , QMessageBox::Ok);
        ipLineEdit->setFocus();
        return false;
    }
    return true;
}

