/*
 *  File:       Watchdog.cpp
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id$
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

#include "Watchdog.h"
#include "HerculesStudio.h"
#include "SystemUtils.h"

#include <iostream>
#include <sstream>
#ifndef hFramework
#include <sys/wait.h>
#endif
#include <cerrno>
#include <sys/stat.h>

Watchdog::Watchdog(int herculesPid, bool child)
{
    mHerculesPid = herculesPid;
    mChild = child;
}

Watchdog::Watchdog(QProcess * herculesProcess)
{
	mHerculesPid = -1;
	mChild = false;
	mProcess = herculesProcess;
}

Watchdog::~Watchdog()
{
}

void Watchdog::run()
{
    if (mChild)
    {
#ifndef hFramework
        int stat_loc;
        int rc = waitpid(mHerculesPid, &stat_loc, 0);
        outDebug(1, std::cout << "watchdog ended for pid:" << rc << std::endl);
#endif
    }
    else
    {
      if (mHerculesPid > 0)
      {
        while(SystemUtils::processIsRunning(mHerculesPid))
          sleep(2);
      }
      else
      {
        while (mProcess->state() != QProcess::NotRunning)
          sleep(2);
      }
    }
    emit HerculesEnded();
}
