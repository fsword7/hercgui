/*
 *  File:       mainwindow.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id$
 *
 *	Main window object
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "HerculesStudio.h"
#include "ui_MainWindow.h"
#include "MainPanel.h"
#include "DevicesPane.h"
#include "CommandLine.h"
#include "Psw.h"
#include "Regs32.h"
#include "Regs64.h"
#include "HerculesExecutor.h"
#include "LogRunner.h"
#include "StatusRunner.h"
#include "Preferences.h"
#include "Watchdog.h"

#include "ConfigFile.h"
#include "Configuration.h"
#include <QTextEdit>
#include <QSystemTrayIcon>
#include <QWaitCondition>

class LogRunner;
class HerculesExecutor;
class PlainLogWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

    bool  			issueCommand(const std::string& command);
    ConfigFile      * getConfigurationFile();

private:
    Ui::MainWindowClass ui;
    QDockWidget         * mTopDock;
    QDockWidget         * mDevicesDock;
    QDockWidget         * mBottomDock;
    QDockWidget         * mGRegisters32Dock;
    QDockWidget         * mCRegisters32Dock;
    QDockWidget         * mFRegisters32Dock;
    QDockWidget         * mARegisters32Dock;
    QDockWidget         * mGRegisters64Dock;
    QDockWidget         * mCRegisters64Dock;
    QDockWidget         * mFRegisters64Dock;
    QDockWidget			* mPswDock;

    Configuration       * mConfiguration;
    ConfigFile          * mConfigFile;
    PlainLogWidget     * mLogWindow;
    QSystemTrayIcon     * mSystemTrayIcon;
    MainPanel           * mMainPanel;
    DevicesPane         * mDevicesPane;
    CommandLine         * mCommandLine;
    Psw					* mPsw;
    Regs32              * mGRegisters32;
    Regs32		 		* mCRegisters32;
    Regs32				* mFRegisters32;
    Regs32				* mARegisters32;
    Regs64  			* mGRegisters64;
    Regs64              * mCRegisters64;
    Regs64              * mFRegisters64;
    SynchronizedQueue   mLogQueue;
    SynchronizedQueue   mStatusQueue;
    HerculesExecutor    * mHerculesExecutor;
    LogRunner *         mLogRunner;
    StatusRunner        * mStatusRunner;
    Preferences         * mPreferences;
    Watchdog            * mWatchdog;
    bool                mHerculesActive;
    bool				mMinimizeOnClose;
    bool                mRecovery;
    bool 				mDevicesRecovery;
    bool 				mDevicesRecoveryCommenced;
    bool                mSystrayHintEjected;
    bool				mStatusReceived;
    QString             mRecoveryConfig;
    QString             mCurrentPath;
    QString             mAdHocLoadParm;
    QString             mIplDevno;

    void 				testGui();
    void  				connectMainPanel();

public slots:
    void powerOn();
    void powerOff();
    void extInterrupt();
    void devInterrupt();
    void load();
    void restart();
    void store();
    void start();
    void stop();
    void restartDevices();

    // menu-related slots
    void newConfig();
    void config();
    void saveConfig();
    void saveConfigAs();
    void openConfig();
    void exitNow();
    void loadCommand();
    void editViewControls();
    void editViewDevices();
    void editViewCommand();
    void editView32BitGr();
    void editView32BitCr();
    void editView32BitFr();
    void editView32BitAr();
    void editView64BitGr();
    void editView64BitCr();
    void editView64BitFr();
    void editViewPSW();
    void dasdinit();
    void dasdload();
    void dasdisup();
    void dasdls();
    void dasdcat();
    void dasdconv();
    void dasdcopy();
    void tapemap();
    void tapecopy();
	void tapesplit();
	void hetinit();
	void hetget();
	void hetupd();
	void hetmap();

    void writeToLogFromQueue();
    void writeToLog(QString line);
    void fontChanged();
    void mipsChanged();
    void pswChanged();
    void themeChanged();
    void dispatchStatus();
    void newCommand();
    void herculesEndedSlot();
    void deleteMessages();
    void saveMessages(bool autosave=false);
    void preferences();
    void helpAbout();
    void closeEvent(QCloseEvent * event);
	void tryAbort();
    void systrayClick(QSystemTrayIcon::ActivationReason);
    void systrayHint();
    void recoverDevices(std::string& statusLine);
};

#endif // MAINWINDOW_H
