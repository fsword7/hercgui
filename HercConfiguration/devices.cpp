/*
 *  File: Devices.cpp
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

#include "devices.h"
#include "VisualizedDeviceEntry.h"
#include "Environment.h"
#include "HerculesStudio.h"

#include <QBitmap>
#include <QString>
#include <QMouseEvent>

#include <iostream>
#include <sstream>
#include <cstdio>

#define NUM_DEVICES 12

const char *Devices::sDeviceNames[NUM_DEVICES] =
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
    "Network"
};

const char *Devices::sDeviceIconNames[NUM_DEVICES] =
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

std::vector<QIcon *> Devices::sDeviceIcons;

Devices::Devices(QWidget *parent)
    : QWidget(parent),
    mLayout(NULL),
    mListView(NULL),
    mModel(NULL)
{
    ui.setupUi(this);
    std::string iconsPath = Environment::getIconsPath() + "/yellow.png";
    mYellowIcon = new QIcon(iconsPath.c_str());
    iconsPath = Environment::getIconsPath() + "/yellowhigh.png";
    mYellowHighIcon = new QIcon(iconsPath.c_str());
}

Devices::~Devices()
{

}

QIcon & Devices::getIcon(Devices::Type type)
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

void Devices::mousePressed(const QModelIndex index)
{
    outDebug(2, std::cout << "pressed " << index.row() << std::endl);
    mLastClick = index.row();
}

void Devices::devClick(QMouseEvent * event)
{
    outDebug(2, std::cout << "click "  << std::endl);
    outDebug(2, std::cout << "mouse pressed(" << mLastClick << "): " <<
    (event->button() == Qt::RightButton ? "Right " : " ?" )
    << " at" << event->x()
    <<  std::endl);

    if (false) //TODO pending implementation ( event->button() == Qt::RightButton )
    {
        QMenu * mMenuAdd;
        if (mLastClick >=0)
        {
            mMenu.clear();
            mMenuAdd = mMenu.addMenu("Add...");
            mMenu.addAction("Delete...", this, SLOT(menuDelete()));
            mMenu.addSeparator();
            mMenu.addAction("Properties...", this, SLOT(menuProperties()));
            mMenu.setVisible(true);
            mMenu.move(event->globalX(), event->globalY());
        }
        else
        {
            mMenu.clear();
            mMenuAdd = mMenu.addMenu("Add");
            mMenu.setVisible(true);
            mMenu.move(event->globalX(), event->globalY());
        }
        if (mMenuAdd != NULL)
        {
            mMenuAdd->clear();
            mMenuAdd->addAction("Terminal", this, SLOT(menuAddTerminal()));
            mMenuAdd->addAction("SYSG", this, SLOT(menuAddSYSG()));
            mMenuAdd->addAction("Console", this, SLOT(menuAddConsole()));
            mMenuAdd->addAction("Printer", this, SLOT(menuAddPrinter()));
            mMenuAdd->addAction("Card Reader", this, SLOT(menuAddCardReader()));
            mMenuAdd->addAction("Card Punch", this, SLOT(menuAddCardPunch()));
            mMenuAdd->addAction("Tape Drives", this, SLOT(menuAddTape()));
            mMenuAdd->addAction("CTC", this, SLOT(menuAddCTC()));
            mMenuAdd->addAction("Dasd", this, SLOT(menuAddDasd()));
            //mMenuAdd->addAction("Comm", this, SLOT(menuAddComm()));
        }
    }
}

void Devices::notify(const std::string& statusLine)
{
    outDebug(3, std::cout << statusLine << std::endl);
    struct DynDeviceLine * line = (DynDeviceLine *)statusLine.c_str();
    int devNo;
    VisualizedDeviceEntry deviceEntry;
    bool deviceAdded = false;
    if (strncmp(line->DEV,"DEV",3) == 0)
    {
        Devices::Type devType = Devices::Other;
        switch (line->action)
        {
            case('A'):
                if (strncmp(line->devType,"DASD",4)==0)
                    devType= Devices::DASD;
                else if (strncmp(line->devType,"CTC",3)==0)
                    devType= Devices::CTC;
                else if (strncmp(line->devType,"QETH",4)==0)
                    devType= Devices::Comm;
                else if (strncmp(line->devType,"RDR ",4)==0)
                    devType= Devices::CardReader;
                else if (strncmp(line->devType,"PCH ",4)==0)
                    devType= Devices::CardReader;
                else if (strncmp(line->devType,"PRT ",4)==0)
                    devType= Devices::Printer;
                else if (strncmp(line->devType,"DSP ",4)==0)
                    devType= Devices::Terminal;
                else if (strncmp(line->devType,"TAPE",4)==0)
                    devType= Devices::Tape;
                else if (strncmp(line->devType,"CON",3)==0)
                    devType= Devices::Console;

                    devNo =  strtol(line->devNo, NULL, 16);
                    {
                    VisualizedDeviceEntry& deviceEntryPtr = *new VisualizedDeviceEntry(devNo, devType, statusLine);
                    std::pair<int,VisualizedDeviceEntry> toinsert(devNo, deviceEntryPtr);
                    mDevices.insert(toinsert);
                    deviceAdded = true;
                    }

                break;
            case('C'):
                devNo = strtol(line->devNo, NULL, 16);
                if (mDevices.find(devNo) != mDevices.end())
                {
                    deviceEntry = mDevices[devNo];

                    if (deviceEntry.getItem() != NULL)
                    {
                        if (line->statusBytes[1] == '1')
                        {
                            deviceEntry.getItem()->setIcon(*mYellowHighIcon);
                        }
                        else
                        {
                            deviceEntry.getItem()->setIcon(*mYellowIcon);
                        }
                    }
                }
                break;
            case('D'):
            {
                devNo = strtol(line->devNo, NULL, 16);
                std::map <int,VisualizedDeviceEntry>::iterator it = mDevices.find(devNo);
                if (it != mDevices.end())
                {
                    mDevices.erase(it);
                    deviceAdded = true;
                }
            }
                break;
            default:
                break;
        }
    }

    if (deviceAdded)
    {
        outDebug(3,std::cout << mDevices.size() << " Devices." << std::endl);
        if (mModel == NULL) mModel = new QStandardItemModel();
        mModel->clear();
        QStandardItem *parentItem = mModel->invisibleRootItem();

        std::map<int, VisualizedDeviceEntry>::iterator it;
        for (int curDev=0; curDev<=Devices::Comm; curDev++ )
        {
            outDebug(5,std::cout << "curDev:" << curDev<< std::endl);
            bool titleDevAdded = false;
            for (it =  mDevices.begin(); it != mDevices.end(); it++)
            {
                VisualizedDeviceEntry& ent = it->second;
                if (ent.getType() == curDev)
                {
                    outDebug(5,std::cout << "ent:" << ent.getType()<<" "<<sDeviceNames[ent.getType()]<<std::endl);
                    if (!titleDevAdded)
                    {
                    	std::string iconPath = Environment::getIconsPath() +
                    			sDeviceIconNames[ent.getType()];
                        const QIcon& ic = getIcon(ent.getType());
                        QStandardItem *item = new QStandardItem(ic, sDeviceNames[ent.getType()]);
                        item->setEditable(false);
                        parentItem->appendRow(item);
                        titleDevAdded=true;
                    }
                    char ss[255];
                    sprintf(ss,"%4.4X %s",ent.getNumber(), ent.getDefinition().substr(25).c_str());
                    if (ent.getIcon() == NULL)
                    {
                        ent.setIcon(mYellowIcon);
                    }
                    ent.setItem(new QStandardItem(*ent.getIcon(), ss));
                    ent.getItem()->setEditable(false);
                    std::string tt = ent.getDefinition().substr(10,10).c_str() ;
                    tt += ent.getDefinition().substr(25).c_str();
                    ent.getItem()->setToolTip(tt.c_str());
                    parentItem->appendRow(ent.getItem());
                }
            }
        }
        if (mListView == NULL) mListView = new DeviceListView(this);
        mListView->setModel(mModel);
        mListView->setVisible(true);
        adjustSize();
        connect(mListView, SIGNAL(pressed(QModelIndex)),this,SLOT(mousePressed(QModelIndex)));
        connect(mListView, SIGNAL(deviceClick(QMouseEvent *)), this, SLOT(devClick(QMouseEvent *)));
    }
}

void Devices::adjustSize()
{
    if (mListView != NULL)
        mListView->setGeometry(0,0,this->size().width()-10,this->size().height()-10);
}

void Devices::resizeEvent ( QResizeEvent * )
{
    adjustSize();
}

QSize Devices::sizeHint() const
{
    return QSize(160, 100);
}

Devices::Type Devices::getType(const std::string& strType)
{
    if (strType.compare("CON")==0)
        return Devices::Terminal;
    else if(strType.compare("DASD")==0)
        return Devices::DASD;
    else return Devices::Other;
}

void Devices::clear()
{
    if (mModel != NULL)
        mModel->clear();
    mDevices.clear();
    if (mListView == NULL)
        return;
    mListView->setModel(mModel);
    mListView->setVisible(true);
    adjustSize();

}

void Devices::menuDelete()
{
    outDebug(2, std::cout << "delete " << mLastClick << std::endl);
    mLastClick=-1;
}

