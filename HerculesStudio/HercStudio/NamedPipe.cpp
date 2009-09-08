/*
 *  File:       NamedPipe.cpp
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
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

#include "HerculesStudio.h"
#include "NamedPipe.h"

#include <QMutexLocker>

#include <string>
#include <sstream>
#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <cstdlib>
#include <errno.h>


#define MAXIMUM_SETS 20

NamedPipe NamedPipe::sInstance;

NamedPipe::NamedPipe()
{

}

NamedPipe& NamedPipe::getInstance()
{
    return sInstance;
}

NamedPipe::~NamedPipe()
{

}

int NamedPipe::recover()
{
    int set = 0;
    mRecovery = false;

    char * home = getenv("HOME");
    std::string hercstudioDir = home;
    hercstudioDir += "/.hercstudio";
    std::string setPath;

    int rc = mkdir(hercstudioDir.c_str(), 0777);
    if (rc<0 && errno != EEXIST)
    {
        perror(hercstudioDir.c_str());
        outDebug(1,std::cout << "rc=" << errno << std::endl);
        return -1;
    }

    for (int pipeSet=0; pipeSet<MAXIMUM_SETS; pipeSet++)
    {
        std::stringstream ss;
        ss << hercstudioDir << "/run" << pipeSet;
        setPath = ss.str();

        if (!fileExists(setPath))
        {
            outDebug(2,std::cout << "setPath=" << setPath << ",run=" << pipeSet << std::endl);
            set = pipeSet;
            break;  // set does not exist - we can use this number
        }

        int hercPid = 0;
        int studioPid = 0;
        std::string pidPath = setPath + "/pid";
        FILE * fstr = fopen(pidPath.c_str(),"r");
        if (fstr != NULL)
        {
            rc = fscanf(fstr,"%d",&hercPid);
            fscanf(fstr,"%d",&studioPid);
            fclose(fstr);
        }

        if (!processIsRunning(hercPid))
        {
            delDir(setPath);
            set = pipeSet;
            break; // set was cleared
        }

        // set exists, hercules process is running
        outDebug(1,std::cout<< "Hercules process " << hercPid << " is running" << std::endl);

        if (!processIsRunning(studioPid))
        {
            // recovery - takeover
            mRecovery = true;
            mSetPath = setPath;
            mHerculesPid = hercPid;
            break;
        }
        else
        {
            mRecovery = false;
        }

    }

    outDebug(2, std::cout << "going to work with set " << set << std::endl);
    mSetPath = setPath;
    if (!mRecovery && mkdir(mSetPath.c_str(), 0777)<0)
    {
        perror(hercstudioDir.c_str());
        outDebug(2,std::cout << "rc=" << errno << std::endl);
        return -1;
    }

    umask(0);

    mFifo0 = mSetPath + "/fifo0";
    outDebug(5,std::cout << "fifo0=" << mFifo0 << std::endl);
    if (!mRecovery && mknod(mFifo0.c_str(), S_IFIFO|0666, 0) != 0)
    {
        perror("fifo0");
        return -1;
    }

    mFifo1 = mSetPath + "/fifo1";
    if (!mRecovery && mknod(mFifo1.c_str(), S_IFIFO|0666, 0) != 0)
    {
        perror("fifo1");// TODO: merge with NamedPipe (same function!)
        return -1;
    }

    mFifo2 = mSetPath + "/fifo2";
    if (!mRecovery && mknod(mFifo2.c_str(), S_IFIFO|0666, 0) != 0)
    {
        perror("fifo2");
        return -1;
    }
    return mRecovery ? 1 : 0;
}

int NamedPipe::delDir(std::string& dir)
{
    std::string command = "rm -rf " + dir;
    return system(command.c_str()); // TODO: temp
}

bool NamedPipe::processIsRunning(int pid)
{
    if (pid <= 1) return false;
    std::stringstream procPath;// TODO: merge with Watchdog (same function!)
    procPath << "/proc/" << pid;
    return (fileExists(procPath.str()));
}

bool NamedPipe::fileExists(const std::string & fileName)
{
    static struct stat dstat;
    return (stat(fileName.c_str(), &dstat) == 0);
}

void NamedPipe::generatePid(int studioPid, int herculesPid)
{
    std::string pidPath = mSetPath + "/pid";
    outDebug(3,std::cout << "generatePid " << studioPid << "," << herculesPid << std::endl);
    FILE * fPid = fopen(pidPath.c_str(),"w+");
    if (fPid == NULL)
    {
        perror(pidPath.c_str());
        return;
    }
    fprintf(fPid,"%d\n",herculesPid);
    fprintf(fPid,"%d\n",studioPid);
    fclose(fPid);
}

int NamedPipe::getHerculesPid()
{
    return mHerculesPid;
}

FILE * NamedPipe::getHerculesStdin()
{
    int fd = open(mFifo0.c_str(),
             O_RDONLY | O_NONBLOCK);
    if (fd == -1) perror("fifo0 fd");
    return fdopen(fd,"r");
}

FILE * NamedPipe::getHerculesCommandsFile()
{
    if (mStdinOutput == NULL)
    {
        if (mStdinOutput == NULL)
            mStdinOutput = fopen(mFifo0.c_str(),"w");
    }
    return mStdinOutput;
}

FILE * NamedPipe::getHerculesStdout()
{
    if (mStdout == NULL)
    {
        if (mStdout == NULL)
            mStdout = fopen(mFifo1.c_str(),"w");
    }
    return mStdout;
}

FILE * NamedPipe::getHerculesLogfile()
{
    if (mStdoutInput == NULL)
    {
        if (mStdoutInput == NULL)
            mStdoutInput = fopen(mFifo1.c_str(),"r");
    }
    return mStdoutInput;
}

FILE * NamedPipe::getHerculesStderr()
{
    if (mStderr == NULL)
    {
    	mStderr = fopen(mFifo2.c_str(),"w");
    }
    return mStderr;
}

FILE * NamedPipe::getHerculesStatus()
{
    if (mStderrInput == NULL)
    {
    	mStderrInput = fopen(mFifo2.c_str(),"r");
    }

    return mStderrInput;
}

std::string NamedPipe::getHerculesCommandline()
{
    std::stringstream procPath;
    procPath << "/proc/" << mHerculesPid << "/cmdline";
    FILE * f = fopen(procPath.str().c_str(),"r");
    if (f==NULL) return "";
    char buff[255];
    int len = fread(buff,sizeof(char), 255,f);
    if (len == -1)
    {
        fclose(f);
        return "";
    }
    for (int i=0; i<len; i++)
        if (buff[i] == '\0') buff[i] = ' ';
    std::string ret = buff;
    return ret;
}
