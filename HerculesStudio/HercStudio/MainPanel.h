/*
 *  File:       MainPanel.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id$
 *
 *  This object presents the main panel
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
#include <QLabel>
#include <QPushButton>
#include <QToolButton>
#include <QLCDNumber>
#include "ui_Devices.h"

class MainPanel;
// base class for "clickable" objects
class ClickLabel : public QLabel
{
  Q_OBJECT
public:
  ClickLabel(MainPanel *parent, QObject * lcd);
  virtual void mousePressEvent(QMouseEvent * event );
signals:
  void clicked(const QPoint &);
private:
  MainPanel * mPanel;
  QObject * mLcd;

};

class MainPanel : public QWidget, public StatusUpdateCollector
{
  Q_OBJECT

public:
  MainPanel(QWidget *parent = 0);
  virtual ~MainPanel();
  virtual void doConnect();

  virtual void standby()=0;
  virtual void setDormant()=0;
  virtual bool notify(const std::string& statusLine)=0;
  virtual void switchMips()=0;
  virtual int getLoadAddress()=0;
  virtual void setLoadAddress(const char * devNo)=0;
  virtual bool isStopped()const = 0;
  virtual void updateLcd(QLCDNumber * lcd, int inc)=0;

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
    virtual void resizeEvent(QResizeEvent * event)=0;
    virtual QSize sizeHint() const=0;
    virtual QAbstractButton* powerOnButton() {return mPowerOnButton;};
    virtual QAbstractButton* powerOffButton() {return mPowerOffButton;};
    virtual QAbstractButton* interruptButton() {return mInterruptButton;};
    virtual QAbstractButton* stopButton() {return mStopButton;};
    virtual QAbstractButton* loadButton() {return mLoadButton;};


    double  mMipsHWM;
    QPushButton *mPowerOnButton;
    QPushButton *mPowerOffButton;
    QPushButton *mInterruptButton;
    QPushButton *mLoadButton;
    QPushButton *mRestartButton;
    QPushButton *mStoreButton;
    QPushButton *mStopButton;
    QPushButton *mStartButton;

    QPixmap     *mYellowLow;
    QPixmap     *mYellowHigh;

public slots:
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
