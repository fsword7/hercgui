/*
 *  File:       MainPanelClassic.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Oct 28, 2010
 *
 *  Copyright (c) 2009-2010 Jacob Dekel
 *  $Id: MainPanel.h 67 2010-09-13 14:06:31Z dekeljacob $
 *
 *  This object presents the main panel in "classic" mode
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

#ifndef MAINPANELCLASSIC_H_
#define MAINPANELCLASSIC_H_

#include "MainPanel.h"
#include "StatusUpdateCollector.h"
#include "Mips.h"

#include <QtGui/QWidget>
#include "ui_Devices.h"
#include <QToolButton>
#include <QMouseEvent>

class MainPanelClassic : public MainPanel
{
  Q_OBJECT

public:
  MainPanelClassic(QWidget *parent = 0);
  virtual ~MainPanelClassic();
  void standby();
  void setDormant();
  bool notify(const QByteArray& statusLine);
  void switchMips();
  bool isStopped() const;
  virtual void updateLcd(QLCDNumber * lcd, int inc);


protected:
    virtual void resizeEvent(QResizeEvent * event);
    virtual QSize sizeHint() const;
    virtual void setupUi(QWidget * parent);

private:


    QLabel      *mSys;
    QLabel      *mSysText;
    QLabel      *mMan;
    QLabel      *mManText;
    QLabel      *mWait;
    QLabel      *mWaitText;
    QLabel      *mLoad;
    QLabel      *mLoadText;
    QLabel      *mPSW;

  ClickLabel *mDial0, *mDial1, *mDial2, *mDial3;

  Mips *mMips;

};

#endif /* MAINPANELCLASSIC_H_ */
