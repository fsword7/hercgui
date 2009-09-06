/*
 *  File: DevicesWidget.cpp
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

#include "DevicesWidget.h"
#include "Configuration.h"
#include "devices.h"
#include "DeviceConfigLine.h"
#include "GenericDeviceProperties.h"
#include "Environment.h"

#include <QMouseEvent>
#include <QList>
#include <QMessageBox>
#include <QHeaderView>

#include <iostream>
#include <sstream>

DevicesWidget::DevicesWidget(ConfigFile * configFile, QWidget * parent) :
    QWidget(parent) , mConfigFile(configFile), mConfigView(), mLastClick(-1)
{
    mConfigView = new DeviceConfigView(this);
    mConfigView->setAutoScroll(false);
    mConfigView->setGeometry(0,0, 680,420);
    mConfigView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);

    QString iconPath = Environment::getIconsPath().c_str();
    mCardIcon     = new QIcon(iconPath + "/card.jpg");
    mConsoleIcon  = new QIcon(iconPath + "/card.jpg");
    mDasdIcon     = new QIcon(iconPath + "/dasd.jpg");
    mNetIcon      = new QIcon(iconPath + "/net.jpg");
    mPrinterIcon  = new QIcon(iconPath + "/printer.jpg");
    mTapeIcon     = new QIcon(iconPath + "/tape.jpg");
    mTerminalIcon = new QIcon(iconPath + "/terminal.jpg");
    mUnknownIcon  = new QIcon(iconPath + "/unknown.jpg");

    initialize();
    QPushButton * okButton = new QPushButton(this);
    okButton->setText("OK");
    okButton->move(10,630);

    QPushButton * cancelButton = new QPushButton(this);
    cancelButton->setText("Cancel");
    cancelButton->move(710,630);

    mConfigView->setVisible(true);
    connect(mConfigView, SIGNAL(pressed(QModelIndex)),this,SLOT(mousePressed(QModelIndex)));
    connect(mConfigView, SIGNAL(deviceClick(QMouseEvent *)),this,SLOT(deviceClick(QMouseEvent *)));

}

DevicesWidget::~DevicesWidget()
{
}

void DevicesWidget::mousePressed(const QModelIndex index)
{
    outDebug(2, std::cout << "pressed " << index.row() << std::endl);
    mLastClick = index.row();
}

void DevicesWidget::deviceClick(QMouseEvent * event)
{
    outDebug(2, std::cout << "click "  << std::endl);
    outDebug(2, std::cout << "mouse pressed(" << mLastClick << "): " <<
        (event->button() == Qt::RightButton ? "Right " : " ?" )
        << " at" << event->x()
        <<  std::endl);

    bool canAddSysg;
    if (mConfigFile->findFirstDeviceByType("SYSG") != NULL)
    	canAddSysg = false;
    else
    	canAddSysg = true;

    if  ( event->button() == Qt::RightButton )
    {
        QMenu * mMenuAdd;
        if (mLastClick >=0)
        {
            mMenu.clear();
            mMenuAdd = mMenu.addMenu("Add");
            mMenu.addAction("Delete", this, SLOT(menuDelete()));
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
            if (canAddSysg)
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

void DevicesWidget::menuAddItem(Devices::Type type)
{
    outDebug(5, std::cout << "add Item" << mLastClick << std::endl);
    mCandidateLine = new DeviceConfigLine("\n");
    mCandidateLine->setNew(true);

    GenericDeviceProperties * mProp = NULL;

    try
    {
        mProp = GenericDeviceProperties::classFactory(*mCandidateLine,this,type);
    }
    catch (...)
    {
        outDebug(2, std::cout << "exception caught in class factory (add)" << std::endl);
    }

    connect(mProp, SIGNAL(updateLine(bool)), this, SLOT(updateLine(bool)));
    mProp->setVisible(true);

    mLastClick=-1;
}

void DevicesWidget::menuAddTerminal()
{
    menuAddItem(Devices::Terminal);
}

void DevicesWidget::menuAddConsole()
{
    menuAddItem(Devices::Console);
}

void DevicesWidget::menuAddPrinter()
{
    menuAddItem(Devices::Printer);
}

void DevicesWidget::menuAddCardReader()
{
    menuAddItem(Devices::CardReader);
}

void DevicesWidget::menuAddCardPunch()
{
    menuAddItem(Devices::CardPunch);
}

void DevicesWidget::menuAddTape()
{
    menuAddItem(Devices::Tape);
}

void DevicesWidget::menuAddCTC()
{
    menuAddItem(Devices::CTC);
}

void DevicesWidget::menuAddDasd()
{
    menuAddItem(Devices::DASD);
}

void DevicesWidget::menuAddSYSG()
{
    menuAddItem(Devices::Sysg);
}

void DevicesWidget::menuDelete()
{
    outDebug(5, std::cout << "delete " << mLastClick << std::endl);
    mConfigFile->deleteLine(mDialogToFile[mLastClick]);
    mLastClick=-1;
    initialize();
}

void DevicesWidget::menuProperties()
{
    outDebug(5, std::cout << "properties " << mLastClick << std::endl);
    mCandidateLine = mConfigFile->getDevice(mDialogToFile[mLastClick]);
    mCandidateLine->setNew(false);
    GenericDeviceProperties * mProp = NULL;

    try
    {
        mProp = GenericDeviceProperties::classFactory(*mCandidateLine, this);
    }
    catch (hsException e)
    {
        std::string msg = e.what();
        QMessageBox::critical(this, "Invalid Device Line", msg.c_str() , QMessageBox::Ok);
    }
    catch (...)
    {
        outDebug(2, std::cout << "exception caught in class factory (properties) " << std::endl);
    }

    if (mProp != NULL)
    {
        connect(mProp, SIGNAL(updateLine(bool)), this, SLOT(updateLine(bool)));
        mProp->setVisible(true);
    }

    mLastClick=-1;

}

void DevicesWidget::updateLine(bool keep)
{
    outDebug(5, std::cout << "Update Line " << std::endl);

    std::stringstream newLine;
    newLine << ConfigurationEditor::normalizeNum(mCandidateLine->getToken(0),4,16);
    newLine <<  " " << mCandidateLine->getMultiToken(1,0) << "\n";
    mCandidateLine->replaceLine(newLine.str());

    if (mCandidateLine != NULL)
    {
        if (keep && mCandidateLine->isNew())
            mConfigFile->addDev(mCandidateLine);
        mCandidateLine = NULL;
    }
    initialize();
}

bool DevicesWidget::isUnique(std::string devno)
{
    return  (mConfigFile->getDevice(devno) == NULL);
}

void DevicesWidget::initialize()
{
    int lastSys = mConfigFile->getLastSys();
    QStandardItemModel * model = new QStandardItemModel(0,1);
    model->clear();
    QStringList labels = (QStringList() << "Device");
    model->setHorizontalHeaderLabels(labels);
    QStandardItem *parentItem = model->invisibleRootItem();
    mDialogToFile.clear();

    outDebug(5, std::cout << "DeviceDlg::initilize from " <<lastSys+1 << " to " << mConfigFile->size() << std::endl);
    int lineNum=-1;
    for (int i=lastSys+1; i<mConfigFile->size(); i++)
    {
        const DeviceConfigLine * line = mConfigFile->getDevice(i);
        outDebug(5,std::cout <<"line=" << line->getLine() << line->isRemark() << std::endl);
        if (line->isRemark()) continue;

        std::string strLine = line->getToken(0);
        strLine.resize(10,' ');
        strLine += line->getToken(1).c_str();
        strLine.resize(20,' ');
        strLine += line->getMultiToken(2,0).c_str();
        strLine.resize(133,' ');
        QStandardItem * item = new QStandardItem(strLine.c_str());
        item->setIcon(Devices::getIcon(line->getDeviceType()));

        parentItem->appendRow(item);
        ++lineNum;
        mDialogToFile[lineNum] = i;
    }
    mConfigView->setModel(model);
    mConfigView->setColumnWidth(0,800);
    QFont font("Mono", 10);
    mConfigView->setFont(font);
    for (int i=0; i<model->rowCount(); i++)
        mConfigView->setRowHeight(i,18);
    mConfigView->setGridStyle(Qt::NoPen);
    mConfigView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}
