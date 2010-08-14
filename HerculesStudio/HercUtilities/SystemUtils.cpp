/*
 *  File:       SystemUtils.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 24, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id:
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


#include "SystemUtils.h"

#include <QFile>

#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


SystemUtils::SystemUtils()
{
}

SystemUtils::~SystemUtils()
{
}

bool SystemUtils::processIsRunning(int pid)
{
    if (pid <= 1) return false;
    std::stringstream procPath;// TODO: merge with Watchdog (same function!)
    procPath << "/proc/" << pid;
    return (fileExists(procPath.str()));
}

bool SystemUtils::fileExists(const std::string & fileName)
{
    static struct stat dstat;
    return (stat(fileName.c_str(), &dstat) == 0);
}

bool SystemUtils::fileExists(const QString & fileName)
{
    return (QFile::exists(fileName));
}
