/*
 *  File:       Psw.cpp
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id$
 *
 *	This object managed display of the psw
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

#ifndef PSW_H_
#define PSW_H_

#include <StatusUpdateCollector.h>

#include <QWidget>
#include <QLabel>
#include <QStatusBar>

class Psw: public StatusUpdateCollector, public QLabel
{
public:
	enum PswMode
	{
		Docked = 0,
		StatusBar
	};
	Psw( QStatusBar * statusBar );
	virtual ~Psw();

	virtual bool notify(const QString& );
	void setFont();
	inline bool isActive() const { return mActive; } ;
	void setMode(PswMode mode);
    void standby();
    void setDormant();
    virtual void setVisible(bool visible, bool modern);
    virtual bool visible();

private:
	void setStatusBar();
	// visible - make status bar visible
	// modern - full status (including man/wait/sys) - used in modern panel
    void setStatusVisible(bool visible, bool modern);

	QString  mLine;
	bool mHasStatusBar;
	bool mVisible;
	bool mActive;
	QStatusBar * mStatusBar;
	Psw::PswMode mMode;
	QLabel * mCpu;
	QLabel * mInstCount;
	QLabel * mSys;
	QLabel * mWait;
	QLabel * mMan;
	QFont  * mFontCourier;
};

#endif /* PSW_H_ */
