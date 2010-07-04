/*
 *  File:       mainwindow.cpp
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id: MainWindow.cpp 34 2009-11-07 06:15:58Z jacob $
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

#include "MainWindow.h"
#include "PreferencesWin.h"
#include "Watchdog.h"
#include "Recovery.h"
#include "HelpAbout.h"
#include "DasdInit.h"
#include "DasdLoad.h"
#include "DasdIsup.h"
#include "Dasdls.h"
#include "Dasdcat.h"
#include "Dasdconv.h"
#include "Dasdcopy.h"
#include "NamedPipe.h"
#include "Environment.h"
#include "Arguments.h"
#include "IplConfig.h"
#include "SystemConfigLine.h"
#include "LogWidget.h"

#include <QFileDialog>
#include <QDockWidget>
#include <QMessageBox>
#include <QFile>

#include <iostream>
#include "cerrno"
#include <err.h>
#include <typeinfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    mTopDock(NULL),
    mDevicesDock(NULL),
    mConfigFile(NULL),
    mCommandLine(NULL),
    mHerculesExecutor(NULL),
    mPreferences(NULL),
    mHerculesActive(false)
{
    ui.setupUi(this);
    ui.actionTapecopy->setVisible(false);
    ui.actionTapemap->setVisible(false);
    ui.actionTapesplit->setVisible(false);
    ui.actionHetget->setVisible(false);
    ui.actionHetinit->setVisible(false);
    ui.actionHetmap->setVisible(false);
    ui.actionHetupd->setVisible(false);

    if (Arguments::getInstance().configFileName().length() > 0)
    {
    	mConfigFile = new ConfigFile(Arguments::getInstance().configFileName());
    }

    QDir d("");
    mCurrentPath = d.absolutePath();

    // left dock (Devices)
    mDevicesDock = new QDockWidget("Devices",this);
    mDevicesDock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    mDevicesPane = new DevicesPane(this);
    mDevicesDock->setWidget(mDevicesPane);
    addDockWidget(Qt::LeftDockWidgetArea, mDevicesDock );

    // top dock (MainPanel)
    mTopDock = new QDockWidget("Controls",this);
    mTopDock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    mMainPanel = new MainPanel(mTopDock);
    mTopDock->setWidget(mMainPanel);
    addDockWidget(Qt::TopDockWidgetArea,mTopDock );

    // gregs32 dock
    mGRegisters32Dock = new QDockWidget("32-bit General Regs",this);
    mGRegisters32Dock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    mGRegisters32 = new Regs32(this,Regs32::gCommand);
    mGRegisters32Dock->setWidget(mGRegisters32);
    addDockWidget(Qt::TopDockWidgetArea,mGRegisters32Dock );
    this->tabifyDockWidget(mTopDock,mGRegisters32Dock);
    mGRegisters32Dock->setVisible(false);

    // cregs32 dock
    mCRegisters32Dock = new QDockWidget("32-bit Control Regs",this);
    mCRegisters32Dock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    mCRegisters32 = new Regs32(this,Regs32::cCommand);
    mCRegisters32Dock->setWidget(mCRegisters32);
    addDockWidget(Qt::TopDockWidgetArea,mCRegisters32Dock );
    this->tabifyDockWidget(mTopDock,mCRegisters32Dock);
    mCRegisters32Dock->setVisible(false);

    // fregs32 dock
    mFRegisters32Dock = new QDockWidget("32-bit Floating-Point Regs",this);
    mFRegisters32Dock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    mFRegisters32 = new Regs32(this,Regs32::fCommand);
    mFRegisters32Dock->setWidget(mFRegisters32);
    addDockWidget(Qt::TopDockWidgetArea,mFRegisters32Dock );
    this->tabifyDockWidget(mTopDock,mFRegisters32Dock);
    mFRegisters32Dock->setVisible(false);

    // aregs32 dock
    mARegisters32Dock = new QDockWidget("Access Registers",this);
    mARegisters32Dock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    mARegisters32 = new Regs32(this,Regs32::aCommand);
    mARegisters32Dock->setWidget(mARegisters32);
    addDockWidget(Qt::TopDockWidgetArea,mARegisters32Dock );
    this->tabifyDockWidget(mTopDock,mARegisters32Dock);
    mARegisters32Dock->setVisible(false);

    // gregs64 dock
    mGRegisters64Dock = new QDockWidget("64-bit General Regs",this);
    mGRegisters64Dock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    mGRegisters64 = new Regs64(this,Regs64::gCommand);
    mGRegisters64Dock->setWidget(mGRegisters64);
    addDockWidget(Qt::TopDockWidgetArea,mGRegisters64Dock );
    this->tabifyDockWidget(mTopDock,mGRegisters64Dock);
    mGRegisters64Dock->setVisible(false);

    // cregs64 dock
    mCRegisters64Dock = new QDockWidget("64-bit Control Regs",this);
    mCRegisters64Dock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    mCRegisters64 = new Regs64(this, Regs64::cCommand);
    mCRegisters64Dock->setWidget(mCRegisters64);
    addDockWidget(Qt::TopDockWidgetArea,mCRegisters64Dock );
    this->tabifyDockWidget(mTopDock,mCRegisters64Dock);
    mCRegisters64Dock->setVisible(false);

    // fregs64 dock
    mFRegisters64Dock = new QDockWidget("64-bit Floating Point Regs",this);
    mFRegisters64Dock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    mFRegisters64 = new Regs64(this, Regs64::fCommand);
    mFRegisters64Dock->setWidget(mFRegisters64);
    addDockWidget(Qt::TopDockWidgetArea,mFRegisters64Dock );
    this->tabifyDockWidget(mTopDock,mFRegisters64Dock);
    mFRegisters64Dock->setVisible(false);

    tabifyDockWidget(mGRegisters32Dock, mTopDock);

    // CommandLine
    mBottomDock = new QDockWidget("Command",this);
    mBottomDock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    mCommandLine = new CommandLine(mBottomDock);
    mCommandLine->setReadOnly(true);
    mBottomDock->setWidget(mCommandLine);
    addDockWidget(Qt::BottomDockWidgetArea,mBottomDock );

    // psw
    mPswDock = new QDockWidget("PSW", this);
    mPswDock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    mPsw = new Psw(Preferences::getInstance().pswMode(), this);
    mPswDock->setWidget(mPsw);
    addDockWidget(Qt::BottomDockWidgetArea,mPswDock );
    this->tabifyDockWidget(mPswDock,mBottomDock);
    mPswDock->setVisible(false);

    if( (Preferences::getInstance().regs(Preferences::ViewGR32)) ) editView32BitGr();
    if( (Preferences::getInstance().regs(Preferences::ViewCR32)) ) editView32BitCr();
    if( (Preferences::getInstance().regs(Preferences::ViewFR32)) ) editView32BitFr();
    if( (Preferences::getInstance().regs(Preferences::ViewAR32)) ) editView32BitAr();
    if( (Preferences::getInstance().regs(Preferences::ViewGR64)) ) editView64BitGr();
    if( (Preferences::getInstance().regs(Preferences::ViewCR64)) ) editView64BitCr();
    if( (Preferences::getInstance().regs(Preferences::ViewFR64)) ) editView64BitFr();
    if( (Preferences::getInstance().regs(Preferences::ViewPsw)) ) editViewPSW();

    QString path = Environment::getIconsPath().c_str();
    outDebug(3,std::cout << "icons=" << path.toStdString() << std::endl);
    QIcon trayIcon(path + "/tray.jpg");
    mSystemTrayIcon = new QSystemTrayIcon(trayIcon);

    connect(ui.actionNew, SIGNAL(triggered()), this , SLOT(newConfig()));
    connect(ui.actionOpen_configuration, SIGNAL(triggered()), this , SLOT(openConfig()));
    connect(ui.actionEditConfig, SIGNAL(triggered()), this , SLOT(config()));
    connect(ui.actionSave, SIGNAL(triggered()), this , SLOT(saveConfig()));
    connect(ui.actionSave_As, SIGNAL(triggered()), this , SLOT(saveConfigAs()));
    connect(ui.actionExit, SIGNAL(triggered()), this , SLOT(exitNow()));
    connect(ui.actionViewControls, SIGNAL(triggered()), this, SLOT(editViewControls()));
    connect(ui.actionViewDevices, SIGNAL(triggered()), this, SLOT(editViewDevices()));
    connect(ui.actionViewCommand, SIGNAL(triggered()), this, SLOT(editViewCommand()));
    connect(ui.actionView_32_bit_General_Registers, SIGNAL(triggered()), this, SLOT(editView32BitGr()));
    connect(ui.actionView_32_bit_Control_Registers, SIGNAL(triggered()), this, SLOT(editView32BitCr()));
    connect(ui.actionView_32_bit_Floating_Point_Registers, SIGNAL(triggered()), this, SLOT(editView32BitFr()));
    connect(ui.actionView_32_bit_Access_Registers, SIGNAL(triggered()), this, SLOT(editView32BitAr()));
    connect(ui.actionView_64_bit_General_Registers, SIGNAL(triggered()), this, SLOT(editView64BitGr()));
    connect(ui.actionView_64_bit_Control_Registers, SIGNAL(triggered()), this, SLOT(editView64BitCr()));
    connect(ui.actionView_64_bit_Floating_Point_Registers, SIGNAL(triggered()), this, SLOT(editView64BitFr()));
    connect(ui.actionView_PSW, SIGNAL(triggered()), this, SLOT(editViewPSW()));
    connect(ui.actionDelete_messages, SIGNAL(triggered()), this, SLOT(deleteMessages()));
    connect(ui.actionSave_messages, SIGNAL(triggered()), this, SLOT(saveMessages()));
    connect(ui.actionPreferences, SIGNAL(triggered()), this, SLOT(preferences()));
    connect(ui.actionPower_on, SIGNAL(triggered()), this, SLOT(powerOn()));
    connect(ui.actionPower_off, SIGNAL(triggered()), this, SLOT(powerOff()));
    connect(ui.actionIPL_Load, SIGNAL(triggered()), this, SLOT(loadCommand()));
    connect(ui.actionStart, SIGNAL(triggered()), this, SLOT(start()));
    connect(ui.actionStop, SIGNAL(triggered()), this, SLOT(stop()));
    connect(ui.actionExtInterrupt, SIGNAL(triggered()), this, SLOT(extInterrupt()));
    connect(ui.actionStore_status, SIGNAL(triggered()), this, SLOT(store()));
    connect(ui.actionPSW_Restart, SIGNAL(triggered()), this, SLOT(restart()));
    connect(ui.actionDevice_Interrupt, SIGNAL(triggered()), this, SLOT(devInterrupt()));
    connect(ui.actionDasdinit, SIGNAL(triggered()), this, SLOT(dasdinit()));
    connect(ui.actionDasdload, SIGNAL(triggered()), this, SLOT(dasdload()));
    connect(ui.actionDasdisup, SIGNAL(triggered()), this, SLOT(dasdisup()));
    connect(ui.actionDasdls, SIGNAL(triggered()), this, SLOT(dasdls()));
    connect(ui.actionDasdcat, SIGNAL(triggered()), this, SLOT(dasdcat()));
    connect(ui.actionDasdconv, SIGNAL(triggered()), this, SLOT(dasdconv()));
    connect(ui.actionDasdcopy, SIGNAL(triggered()), this, SLOT(dasdcopy()));

    connect(ui.actionAbout, SIGNAL(triggered()), this, SLOT(helpAbout()));
    connect(mMainPanel, SIGNAL(powerOnClicked()), this , SLOT(powerOn()));
    connect(mMainPanel, SIGNAL(powerOffClicked()), this , SLOT(powerOff()));
    connect(mMainPanel, SIGNAL(interruptClicked()), this , SLOT(extInterrupt()));
    connect(mMainPanel, SIGNAL(loadClicked()), this , SLOT(load()));
    connect(mMainPanel, SIGNAL(restartClicked()), this , SLOT(restart()));
    connect(mMainPanel, SIGNAL(storeClicked()), this , SLOT(store()));
    connect(mMainPanel, SIGNAL(startClicked()), this , SLOT(start()));
    connect(mMainPanel, SIGNAL(stopClicked()), this , SLOT(stop()));
    connect(mDevicesPane, SIGNAL(restartDevices()), this , SLOT(restartDevices()));
    connect(mCommandLine, SIGNAL(returnPressed()), this , SLOT(newCommand()));
    connect(mSystemTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(systrayClick(QSystemTrayIcon::ActivationReason)));

    if (Preferences::getInstance().splitLog())
    {
		mLogWindow  = new LogWidget(this);
		fontChanged();
		setCentralWidget(static_cast<LogWidget *>(mLogWindow)->tabWidget());
    }
    else
    {
    	mLogWindow  = new PlainLogWidget(this);
    	mLogWindow->setReadOnly(true);
    	fontChanged();
    	setCentralWidget(mLogWindow);
    }

    int recoverRc = NamedPipe::getInstance().recover();
    mRecovery = (recoverRc == 1) ;
    if (mRecovery)
    {
        Recovery * r = new Recovery(this);
        r->exec();
        mRecoveryConfig = r->getHerculesConf().c_str();
        if (mRecoveryConfig == "!")
        {
            exit(0);
        }
        if (mRecoveryConfig == "")
            mRecovery = false;
        else
        {
            powerOn();
            mLogWindow->append("--- successfully attached to Hercules ---");
        }
    }
    if (Arguments::getInstance().configFileName().length() > 0)
    	powerOn();
}

MainWindow::~MainWindow()
{
}

void MainWindow::writeToLog(QString line)
{
    line.remove("\n");
    mLogWindow->append(line);
}

void MainWindow::fontChanged()
{
    std::string fontName = Preferences::getInstance().fontName(Preferences::LogFontObject);
    if (fontName[0] == '(')
    {
    	mLogWindow ->setFont(QFont());
    }
    else
    {
		QFont font(fontName.c_str());
		font.setPointSize(Preferences::getInstance().fontSize(Preferences::LogFontObject));
		font.setBold(Preferences::getInstance().fontIsBold(Preferences::LogFontObject));
		font.setItalic(Preferences::getInstance().fontIsItalic(Preferences::LogFontObject));
		mLogWindow->setFont(font);
    }
    mCRegisters32->setFont();
    mGRegisters32->setFont();
    mFRegisters32->setFont();
    mARegisters32->setFont();
    mCRegisters64->setFont();
    mGRegisters64->setFont();
    mFRegisters64->setFont();

    mPsw->setFont();
    mCommandLine->setFont();
}

void MainWindow::mipsChanged()
{
	mMainPanel->switchMips();
}

void MainWindow::pswChanged()
{
	mPsw->setMode(mPreferences->pswMode());
	mPswDock->setVisible(mPreferences->pswMode() == Psw::Docked);
}

void MainWindow::writeToLogFromQueue()
{

    while (!mLogQueue.empty())
    {
        std::string s = mLogQueue.front();
        mLogQueue.pop_front();
        if (!s.empty())
        {
        	mLogWindow->append(s.c_str());
        }
    }
}

void MainWindow::dispatchStatus()
{
    while (!mStatusQueue.empty())
    {
        std::string statusLine = mStatusQueue.front();
        mStatusQueue.pop_front();
        outDebug(4, std::cout << "writeToDevice:" << statusLine << std::endl);
        if (!statusLine.empty())
        {
            if (statusLine[0] == 'D')
            {
                if (mDevicesRecovery) recoverDevices(statusLine);
                mDevicesPane->notify(statusLine);
            }
            else if (statusLine[0] == 'G')
            	mGRegisters32->notify(statusLine);
            else if (statusLine[0] == 'C')
            	mCRegisters32->notify(statusLine);
            else if (statusLine[0] == 'F')
            	mFRegisters32->notify(statusLine);
            else if (statusLine[0] == 'A')
            	mARegisters32->notify(statusLine);
            else if (statusLine[0] == '6')
            {
            	if (statusLine[3] == 'G')
            		mGRegisters64->notify(statusLine);
            	else if (statusLine[3] == 'C')
            		mCRegisters64->notify(statusLine);
            	else if (statusLine[3] == 'F')
            		mFRegisters64->notify(statusLine);
            }
            else
            {
                mMainPanel->notify(statusLine);
                mPsw->notify(statusLine);
            }
        }
    }
}

void MainWindow::recoverDevices(std::string& statusLine)
{
    if (statusLine.substr(0,5) == "DEVX=") return;
	if (statusLine[4] == 'X')
	{
		if ( mDevicesRecoveryCommenced )
		{
			mDevicesRecoveryCommenced = false;
			mDevicesRecovery = false;
		}
		else
		{
			mDevicesRecoveryCommenced = true;
		}
		return;
	}
	statusLine = "DEVA=" + statusLine.substr(4);
}

void MainWindow::newCommand()
{
    CommandLine *cl = static_cast<CommandLine *>(QObject::sender());
    if (cl == NULL) return;
    outDebug(2, std::cout  << cl->text().toStdString() << std::endl);
    FILE * input = NamedPipe::getInstance().getHerculesCommandsFile();
    if (input)
    {
    	if (!mLogWindow->isOSLog() || cl->text().left(1).compare(".") == 0)
    		fprintf(input,"%s\n",cl->text().toAscii().data());
    	else
    		fprintf(input,".%s\n",cl->text().toAscii().data());

        fflush(input);
    }
    else  err(1, "err");
}


void MainWindow::newConfig()
{
    if (mPreferences == NULL)
        mPreferences = &Preferences::getInstance();
    mConfigFile = new ConfigFile("/usr/share/herculesstudio/template.conf");
    mConfigFile->setNew(true);
    mConfiguration = new Configuration(mConfigFile, true, this);
    mConfiguration->show();
    this->setWindowTitle("(new file) - Hercules Studio");
}

void MainWindow::config()
{
    if (mConfigFile == NULL)
        openConfig();
    if (mConfigFile == NULL)
        return;
    mConfiguration = new Configuration(mConfigFile, false, this);
    mConfiguration->show();
}

void MainWindow::openConfig()
{
    if (mPreferences == NULL)
            mPreferences = &Preferences::getInstance();
    outDebug(5,std::cout << "config at:" << mPreferences->configDir() << std::endl);
    std::string s = QFileDialog::getOpenFileName(this,
            "Open configuration",
            mPreferences->configDir().c_str(),
            tr("Hercules configuration files (*.conf *.cnf);;All files(*)")).toUtf8().data();
    if (s.length() == 0)
        return;

    mConfigFile = new ConfigFile(s);
    QFileInfo f(s.c_str());
    mPreferences->setVolatileConfigDir(f.dir().absolutePath().toStdString().c_str());
    this->setWindowTitle((mConfigFile->getFileName() + " - Hercules Studio").c_str());
}

void MainWindow::saveConfig()
{
	if (mConfigFile == NULL) return;
    if (mConfigFile->isNew())
        saveConfigAs();
    else
    {
        mConfigFile->appendNewLines();
        mConfigFile->write();
    }
}

void MainWindow::saveConfigAs()
{
	if (mConfigFile == NULL) return;
	if (mPreferences == NULL)
	        mPreferences = &Preferences::getInstance();
    std::string s = QFileDialog::getSaveFileName(this,
            "Save configuration",
            mPreferences->configDir().c_str(),
            tr("Hercules configuration files (*.conf *.cnf);;All files(*)")).toUtf8().data();
    if (s.length() == 0)
        return;
    int pos = s.length()-5;
    if ( (s.substr(pos,5) != ".conf") && (s.substr(pos+1,4) != ".cnf") )
        s += ".conf";
    mConfigFile->appendNewLines();
    mConfigFile->write(s);
    this->setWindowTitle((mConfigFile->getFileName() + " - Hercules Studio").c_str());
}

void MainWindow::exitNow()
{
    powerOff();
    close();
}

void MainWindow::editViewControls()
{
    mTopDock->setVisible(!mTopDock->isVisible());
    ui.actionViewControls->setChecked(mTopDock->isVisible());
}

void MainWindow::editViewDevices()
{
    mDevicesDock->setVisible(!mDevicesDock->isVisible());
    ui.actionViewDevices->setChecked(mDevicesDock->isVisible());
}

void MainWindow::editViewCommand()
{
    mBottomDock->setVisible(!mBottomDock->isVisible());
    ui.actionViewCommand->setChecked(mBottomDock->isVisible());
}

void MainWindow::editView32BitGr()
{
	bool newVal = !mGRegisters32Dock->isVisible();
    mGRegisters32Dock->setVisible(newVal);
    mGRegisters32->setActive(newVal);
    ui.actionView_32_bit_General_Registers->setChecked(newVal);
    Preferences::getInstance().setRegs(Preferences::ViewGR32, newVal);
}

void MainWindow::editView32BitCr()
{
	bool newVal = !mCRegisters32Dock->isVisible();
    mCRegisters32Dock->setVisible(newVal);
    mCRegisters32->setActive(newVal);
    ui.actionView_32_bit_Control_Registers->setChecked(newVal);
    Preferences::getInstance().setRegs(Preferences::ViewCR32, newVal);
}

void MainWindow::editView32BitFr()
{
	bool newVal = !mFRegisters32Dock->isVisible();
    mFRegisters32Dock->setVisible(newVal);
    mFRegisters32->setActive(newVal);
    ui.actionView_32_bit_Floating_Point_Registers->setChecked(newVal);
    Preferences::getInstance().setRegs(Preferences::ViewFR32, newVal);
}

void MainWindow::editView32BitAr()
{
	bool newVal = !mARegisters32Dock->isVisible();
	mARegisters32Dock->setVisible(newVal);
	mARegisters32->setActive(newVal);
    ui.actionView_32_bit_Access_Registers->setChecked(newVal);
    Preferences::getInstance().setRegs(Preferences::ViewAR32, newVal);
}

void MainWindow::editView64BitGr()
{
	bool newVal = !mGRegisters64Dock->isVisible();
    mGRegisters64Dock->setVisible(newVal);
    mGRegisters64->setActive(newVal);
    ui.actionView_64_bit_General_Registers->setChecked(newVal);
    Preferences::getInstance().setRegs(Preferences::ViewGR64, newVal);
}

void MainWindow::editView64BitCr()
{
	bool newVal = !mCRegisters64Dock->isVisible();
    mCRegisters64Dock->setVisible(newVal);
    mCRegisters64->setActive(newVal);
    ui.actionView_64_bit_Control_Registers->setChecked(newVal);
    Preferences::getInstance().setRegs(Preferences::ViewCR64, newVal);
}

void MainWindow::editView64BitFr()
{
	bool newVal = !mFRegisters64Dock->isVisible();
    mFRegisters64Dock->setVisible(newVal);
    mFRegisters64->setActive(newVal);
    ui.actionView_64_bit_Floating_Point_Registers->setChecked(newVal);
    Preferences::getInstance().setRegs(Preferences::ViewFR64, newVal);
}

void MainWindow::editViewPSW()
{
	bool newVal = !mPsw->isActive();
	mPsw->setActive(newVal);
	ui.actionView_PSW->setChecked(newVal);
	Preferences::getInstance().setRegs(Preferences::ViewPsw, newVal);
	if (Preferences::getInstance().pswMode() == Psw::Docked)
	{
		mPswDock->setVisible(newVal);
	}



 }

void MainWindow::deleteMessages()
{
    if (mHerculesActive)
    {
        QMessageBox msgBox;
        msgBox.setText("Messages belong to to a running Hercules.");
        msgBox.setInformativeText("Are you sure you want to delete history messages?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        msgBox.setIcon(QMessageBox::Warning);
        int ret = msgBox.exec();
        if (ret != QMessageBox::Yes)
            return ;
    }
    mLogWindow->clear();

}


void MainWindow::saveMessages(bool autosave)
{
	//TODO: allow splitting to two separate files when in split log mode
    if (mLogWindow->empty())
        return;
    QString s;
    if (!autosave)
    {
		 s = QFileDialog::getSaveFileName(this,
				tr("Save Messages"),
				mPreferences->logsDir().c_str(),
				tr("Hercules log files (*.log)")).toUtf8().data();
		if (!s.endsWith(".log"))
			s+= ".log";
    }
    else
    {
    	s = mPreferences->logsDir().c_str();
    	s += "/hercules";
    	int i=0;
    	QString backS = s;
    	while (QFile::exists(s+".log"))
    	{
    		s = backS + "." + QString().setNum(++i);
    	}
    	if (i==0)
    	{
    		s=backS;
    	}
    	s+=".log";
    }
    QFile file(s);

    QString data = mLogWindow->toPlainText();
    file.open(QIODevice::WriteOnly);
    file.write(data.toStdString().c_str(),data.length());
    file.close();
}

void MainWindow::preferences()
{
    if (mPreferences == NULL)
        mPreferences = &Preferences::getInstance();
    PreferencesWin * pw = new PreferencesWin(mCurrentPath.toStdString(), mPreferences, this);
    connect(pw, SIGNAL(fontChanged()), this, SLOT(fontChanged()));
    connect(pw, SIGNAL(mipsChanged()), this, SLOT(mipsChanged()));
    connect(pw, SIGNAL(pswChanged()), this, SLOT(pswChanged()));
    pw->show();
}

ConfigFile * MainWindow::getConfigurationFile()
{
	return mConfigFile;
}


void MainWindow::powerOn()
{
    std::string configName;
    if (!mRecovery)
    {
        if (mConfigFile == NULL) openConfig();
        if (mConfigFile == NULL) return;
        configName = mConfigFile->getFileName();
        mPreferences = &Preferences::getInstance();
    }
    else
    {
        configName = mRecoveryConfig.toStdString();
        mConfigFile = new ConfigFile(configName);
    }

    if (configName.empty()) return;
    if (mConfigFile->changed())
    {
        int ret = QMessageBox::warning(this, "Configuration",
                                            "Configuration file was updated since last saved\n"
                                            "Do you want to save it now?",
                                            QMessageBox::Save | QMessageBox::Cancel,
                                            QMessageBox::NoButton);
        if (ret == QMessageBox::Save)
        {
            mConfigFile->write();
        }
        else return;
    }

    if (mHerculesExecutor != NULL) return;
    ui.menuFile->setEnabled(false);
    int herculesPid = 0;
    if (!mRecovery)
    {
        outDebug(5,std::cout << "not recovery" << std::endl);
        mHerculesExecutor = new HerculesExecutor(*this);
        herculesPid = mHerculesExecutor->run(configName, mPreferences->hercDir());
        mDevicesRecovery = false;
    }
    else
    {
        outDebug(2,std::cout << "recovery" << std::endl);
        herculesPid = NamedPipe::getInstance().getHerculesPid();
        mHerculesExecutor = new HerculesExecutor(*this);
        mDevicesRecovery = true;
    }

    NamedPipe::getInstance().generatePid(getpid(), herculesPid);

    mLogRunner = new LogRunner(mLogQueue);
    connect(mLogRunner, SIGNAL(newData()), this , SLOT(writeToLogFromQueue()));
    mLogRunner->start();

    mStatusRunner = new StatusRunner(mStatusQueue);
    connect(mStatusRunner, SIGNAL(newData()), this , SLOT(dispatchStatus()));
    mStatusRunner->start();

    mWatchdog = new Watchdog(herculesPid, !mRecovery);
    connect(mWatchdog, SIGNAL(HerculesEnded()), this, SLOT(herculesEndedSlot()));
    mWatchdog->start();

    mHerculesActive = true;
    mMinimizeOnClose=true;
    mCommandLine->setReadOnly(false);

    mMainPanel->standby();
    this->setWindowTitle((mConfigFile->getFileName() + " - Hercules Studio").c_str());
    if (mRecovery)
    {
    	// refresh devices list
        issueCommand("]DEVLIST=1");
        usleep(500000);
        issueCommand("]NEWDEVLIST=1");

    }

    mRecovery = false;
    mSystemTrayIcon->show();

    if (ui.actionView_32_bit_General_Registers->isChecked())
    	this->mGRegisters32->setActive(true);
    if (ui.actionView_32_bit_Control_Registers->isChecked())
    	this->mCRegisters32->setActive(true);
    if (ui.actionView_32_bit_Floating_Point_Registers->isChecked())
    	this->mFRegisters32->setActive(true);
    if (ui.actionView_32_bit_Access_Registers->isChecked())
    	this->mARegisters32->setActive(true);
    if (ui.actionView_64_bit_General_Registers->isChecked())
    	this->mGRegisters64->setActive(true);
    if (ui.actionView_64_bit_Control_Registers->isChecked())
    	this->mCRegisters64->setActive(true);
    if (ui.actionView_64_bit_Floating_Point_Registers->isChecked())
    	this->mFRegisters64->setActive(true);
}

void MainWindow::powerOff()
{
    if (!mHerculesActive) return ;
    mMinimizeOnClose=false;
    printf("Goodbye!\n");
    FILE * input = NamedPipe::getInstance().getHerculesCommandsFile();
    if (input)
    {
            fprintf(input,"quit\n\r");
            fflush(input);
            outDebug(2, std::cout << "hercules closed" << std::endl);
            usleep(100000);
    }
    mDevicesPane->clear();
    mMainPanel->setDormant();
}

void MainWindow::load()
{
    if (!mHerculesActive) return;
    int addr = mMainPanel->getLoadAddress();
    outDebug(1, std::cout << "going to load " << addr << std::endl);
    FILE * input = NamedPipe::getInstance().getHerculesCommandsFile();
    if (input)
    {
            fprintf(input,"IPL %X\n",addr);
            fflush(input);
    }
}

void MainWindow::loadCommand()
{
	if (!mHerculesActive) return;
	int lpIndex = -1;
	if (mAdHocLoadParm != "")
	{
		QString loadParm = mAdHocLoadParm;
	}
	else
	{
		hOutDebug(5, "lastSYS=" << mConfigFile->getLastSys())
		for (int i=0; i<= mConfigFile->getLastSys(); i++)
		{
			if ((*mConfigFile)[i]->getLowercaseToken(0) == "loadparm")
			{
				lpIndex = i;
				mAdHocLoadParm = (*mConfigFile)[i]->getToken(1).c_str();
				break;
			}
			else hOutDebug(5,"skipping " << i << ": " << (*mConfigFile)[i]->getLowercaseToken(0));
		}
	}
	if (lpIndex == -1) mAdHocLoadParm = "......";
	IplConfig * iplConfig = new IplConfig(mMainPanel->getLoadAddress(),mAdHocLoadParm.toAscii(), this);
	iplConfig->show();
	connect(iplConfig, SIGNAL(doIpl(QString, QString)), this, SLOT(loadCommandDoIpl(QString, QString)));
}

void MainWindow::loadCommandDoIpl(const QString& devNo, const QString& loadParm)
{
	mAdHocLoadParm = loadParm;
	QString loadCommand = "LOADPARM " + mAdHocLoadParm;
	issueCommand(loadCommand.toStdString());
	mMainPanel->setLoadAddress(devNo.toStdString().c_str());
	std::string iplCommand = "IPL " + devNo.toStdString();
	issueCommand(iplCommand);
}

void MainWindow::extInterrupt()
{
    if (!mHerculesActive) return;
    outDebug(1, std::cout << "going to externally interrupt " << std::endl);
    FILE * input = NamedPipe::getInstance().getHerculesCommandsFile();
    if (input)
    {
            fprintf(input,"ext\n");
            fflush(input);
    }
}

void MainWindow::devInterrupt()
{
    if (!mHerculesActive) return;
    int addr = mMainPanel->getLoadAddress();
    outDebug(1, std::cout << "going to interrupt " << addr << std::endl);
    FILE * input = NamedPipe::getInstance().getHerculesCommandsFile();
    if (input)
    {
            fprintf(input,"I %X\n", addr);
            fflush(input);
    }
}

void MainWindow::restart()
{
    if (!mHerculesActive) return;
    FILE * input = NamedPipe::getInstance().getHerculesCommandsFile();
    if (input)
    {
            fprintf(input,"restart\n");
            fflush(input);
    }
}

void MainWindow::store()
{
    if (!mHerculesActive) return;
    FILE * input = NamedPipe::getInstance().getHerculesCommandsFile();
    if (input)
    {
            fprintf(input,"store\n");
            fflush(input);
    }
}

void MainWindow::start()
{
    if (!mHerculesActive) return;
    FILE * input = NamedPipe::getInstance().getHerculesCommandsFile();
    if (input)
    {
            fprintf(input,"startall\n");
            fflush(input);
    }
}

void MainWindow::stop()
{
    if (!mHerculesActive) return;
    FILE * input = NamedPipe::getInstance().getHerculesCommandsFile();
    if (input)
    {
            fprintf(input,"stopall\n");
            fflush(input);
    }
}

void MainWindow::dasdinit()
{
    DasdInit * dasdinit = new DasdInit(this);
    connect(dasdinit, SIGNAL(output(QString)), this , SLOT(writeToLog(QString)));
    dasdinit->show();
}

void MainWindow::dasdload()
{
    DasdLoad * dasdload = new DasdLoad(this);
    connect(dasdload, SIGNAL(output(QString)), this , SLOT(writeToLog(QString)));
    dasdload->show();
}

void MainWindow::dasdisup()
{
    DasdIsup * dasdisup = new DasdIsup(this);
    connect(dasdisup, SIGNAL(output(QString)), this , SLOT(writeToLog(QString)));
    dasdisup->show();
}

void MainWindow::dasdls()
{
    Dasdls * dasdls = new Dasdls(this);
    connect(dasdls, SIGNAL(output(QString)), this , SLOT(writeToLog(QString)));
    dasdls->show();
}

void MainWindow::dasdcat()
{
    Dasdcat * dasdcat = new Dasdcat(this);
    connect(dasdcat, SIGNAL(output(QString)), this , SLOT(writeToLog(QString)));
    dasdcat->show();
}

void MainWindow::dasdconv()
{
    Dasdconv * dasdconv = new Dasdconv(this);
    connect(dasdconv, SIGNAL(output(QString)), this , SLOT(writeToLog(QString)));
    dasdconv->show();
}

void MainWindow::dasdcopy()
{
    Dasdcopy * dasdcopy = new Dasdcopy(this);
    connect(dasdcopy, SIGNAL(output(QString)), this , SLOT(writeToLog(QString)));
    dasdcopy->show();
}

void MainWindow::herculesEndedSlot()
{
    std::cerr << "############ ended ##############" << std::endl;
    if (Preferences::getInstance().autosaveLog())
    {
    	saveMessages(true);
    }
    mLogRunner->terminate();
    mStatusRunner->terminate();
    mWatchdog->terminate();
    mHerculesActive = false;
    mCommandLine->setReadOnly(true);
    mHerculesExecutor = NULL;
    mMainPanel->setDormant();
    ui.menuFile->setEnabled(true);
    mSystemTrayIcon->setVisible(false);
    mMinimizeOnClose=false;
}

void MainWindow::helpAbout()
{
    HelpAbout *ha = new HelpAbout(this);
    ha->show();
}

void MainWindow::systrayHint()
{
	if (!mSystrayHintEjected)
	{
		mSystemTrayIcon->showMessage("Hercules is still active","Click this icon to restore the main window",QSystemTrayIcon::Information,5000);
        mSystemTrayIcon->setToolTip("Click on this icon to restore the main window");
		mSystrayHintEjected = true;
	}
}

void MainWindow::closeEvent(QCloseEvent *event)
 {
     if (mHerculesActive && mMinimizeOnClose)
     {
    	 hOutDebug(5,"systray");
         mSystemTrayIcon->setVisible(true);
         systrayHint();
         setVisible(false);

         event->ignore();
     }
     else
     {
    	 hOutDebug(5,"real Close");
         if (mHerculesActive)
         {
        	 QMessageBox::warning(this,"Hercules is still running",
        			 "please wait until Hercules finishes\n"
        			 "Press OK to wait until hercules finishes\n"
        			 "Or press Abort to force exit",
        			 QMessageBox::Ok, QMessageBox::Abort);
        	 event->ignore(); // TODO - deleteLater debug
         }
         //else deleteLater();
     }
 }

void MainWindow::systrayClick(QSystemTrayIcon::ActivationReason)
{
	if (isVisible())
	{
		setVisible(false);
		systrayHint();
	}
	else
	{
		setVisible(true);
	}
}

void MainWindow::restartDevices()
{
    hOutDebug(3,"MainWindow::restartRecovery");
    mDevicesRecovery = true;
    issueCommand("]DEVLIST=1");
    usleep(500000);
    issueCommand("]NEWDEVLIST=1");
}

bool MainWindow::issueCommand(const std::string& command)
{
    if (!mHerculesActive)
        return false;

    outDebug(2, std::cout << "issue command:" << command << std::endl);
    FILE * input = NamedPipe::getInstance().getHerculesCommandsFile();
    if (input)
    {
            fprintf(input,"%s\n", command.c_str());
            fflush(input);
            return true;
    }
    else
    {
        outDebug(3, std::cout << "input=" << input << std::endl);
        return false;
    }
}
