/*
 *  File:       Watchdog.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id: Watchdog.h 34 2009-11-07 06:15:58Z jacob $
 *
 *	This object is the watchdog for the hercules process
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

#ifndef WATCHDOG_H_
#define WATCHDOG_H_

#include <QThread>

class Watchdog : public QThread
{
	Q_OBJECT
public:
	Watchdog(int herculesPid, bool child);
	virtual ~Watchdog();

	virtual void run();

signals:
	void HerculesEnded();

private:
	int mHerculesPid;
	bool mChild;

    bool processIsRunning(int pid); // TODO: merge with NamedPipe (same function!)
};

#endif /* WATCHDOG_H_ */
