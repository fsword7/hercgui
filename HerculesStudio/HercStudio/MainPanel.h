/*
 *  File:       MainPanel.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id$
 *
 *	This object presents the main panel
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

#ifndef MAINPANEL_H_
#define MAINPANEL_H_

#include "StatusUpdateCollector.h"
#include "Mips.h"

#include <QtGui/QWidget>
#include "ui_Devices.h"
#include <QPushButton>
#include <QToolButton>
#include <QLCDNumber>
#include <QLabel>
#include <QMouseEvent>

// base class for "clickable" objects
class ClickLabel : public QLabel
{
  Q_OBJECT
public:
  ClickLabel(QWidget *parent, QLCDNumber * lcd);
  virtual void mousePressEvent(QMouseEvent * event );
signals:
  void clicked(const QPoint &);
private:
  QLCDNumber * mLcd;

};

class MainPanel : public QWidget, public StatusUpdateCollector
{
  Q_OBJECT

public:
  MainPanel(QWidget *parent = 0);
  virtual ~MainPanel();
  static void updateLcd(QLCDNumber * lcd, int inc);
  int getLoadAddress();
  void setLoadAddress(const char * devNo);
  void standby();
  void setDormant();
  bool notify(const std::string& statusLine);
  void switchMips();

signals:
  void powerOnClicked();
  void powerOffClicked();
  void loadClicked();
  void interruptClicked();
  void restartClicked();
  void storeClicked();
  void startClicked();
  void stopClicked();

protected:
    void resizeEvent(QResizeEvent * event);
    QSize sizeHint() const;

private:

  QPushButton *mPowerOnButton;
  QPushButton *mPowerOffButton;
  QPushButton *mInterruptButton;
  QPushButton *mLoadButton;

    QLabel      *mSys;
    QLabel      *mSysText;
    QLabel      *mMan;
    QLabel      *mManText;
    QLabel      *mWait;
    QLabel      *mWaitText;
    QLabel      *mLoad;
    QLabel      *mLoadText;
    QLabel      *mPSW;

  QPixmap     *mYellowLow;
  QPixmap     *mYellowHigh;

  QPushButton *mRestartButton;
  QPushButton *mStoreButton;
  QPushButton *mStopButton;
  QPushButton *mStartButton;

  QLCDNumber *mLcd0, *mLcd1, *mLcd2, *mLcd3;
  ClickLabel *mDial0, *mDial1, *mDial2, *mDial3;

  Mips *mMips;

  double 	mMipsHWM;

  void setupUi(QWidget * parent);

private slots:
void powerOnClickedSlot();
void powerOffClickedSlot();
void loadClickedSlot();
void interruptClickedSlot();
void restartClickedSlot();
void storeClickedSlot();
void startClickedSlot();
void stopClickedSlot();
};

#endif /* MAINPANEL_H_ */
