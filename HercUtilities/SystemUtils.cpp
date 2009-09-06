/*
 * SystemUtils.cpp
 *
 *  Created on: Aug 24, 2009
 *      Author: yakovd
 */

#include "SystemUtils.h"

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
