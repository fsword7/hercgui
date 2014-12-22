/*
 *  File:       NamedPipe.cpp
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009-2013 Jacob Dekel
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

#include "HerculesStudio.h"
#include "SystemUtils.h"
#include "NamedPipe.h"

#include <QMutexLocker>

#include <string>
#include <sstream>
#include <iostream>

#include <cstdio>
#include <fcntl.h>
#include <cstdlib>
#include <cerrno>
#ifndef hFramework
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

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
    #ifndef hFramework
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
        hOutDebug(0,"rc=" << errno);
        return -1;
    }

    for (int pipeSet=0; pipeSet<MAXIMUM_SETS; pipeSet++)
    {
        std::stringstream ss;
        ss << hercstudioDir << "/run" << pipeSet;
        setPath = ss.str();

        if (!SystemUtils::fileExists(setPath))
        {
            hOutDebug(2,"setPath=" << setPath << ",run=" << pipeSet);
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
            if (fscanf(fstr,"%d",&studioPid))
                fclose(fstr);
        }

        if (!SystemUtils::processIsRunning(studioPid))
        {
            if (!SystemUtils::processIsRunning(hercPid))
            {
                delDir(setPath);
                set = pipeSet;
                break; // set was cleared - use it
            }
            else
            {
                // recovery - takeover
                hOutDebug(0,"Hercules process " << hercPid << " is running");
                mRecovery = true;
                mSetPath = setPath;
                mHerculesPid = hercPid;
                break;
            }
        }
    }

    hOutDebug(0, "going to work with set " << set);
    mSetPath = setPath;
    if (!mRecovery && mkdir(mSetPath.c_str(), 0777)<0)
    {
        perror(hercstudioDir.c_str());
        hOutDebug(2,"rc=" << errno);
        return -1;
    }

    generatePid(getpid(),0);
    umask(0);

    mFifo0 = mSetPath + "/fifo0";
    hOutDebug(5,"fifo0=" << mFifo0);
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
    #else
    return 0;
    #endif
}

int NamedPipe::delDir(std::string& dir)
{
    std::string command = "rm -rf " + dir;
    return system(command.c_str()); // TODO: temp
}

void NamedPipe::generatePid(int studioPid, int herculesPid)
{
    std::string pidPath = mSetPath + "/pid";
    hOutDebug(0,"generatePid " << studioPid << "," << herculesPid);
    FILE * fPid = fopen(pidPath.c_str(),"w");
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

#ifndef hFramework
FILE * NamedPipe::getHerculesStdin()
{
    int fd = open(mFifo0.c_str(),
             O_RDONLY | O_NONBLOCK);
    if (fd == -1) perror("fifo0 fd");
    return fdopen(fd,"r");
}
#endif
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

QFile& NamedPipe::getHerculesLogfile()
{
	if (mStdoutInput == NULL)
	{
		mStdoutInput = new QFile(mFifo1.c_str());
		mStdoutInput->open(QIODevice::ReadOnly);
	}
	return *mStdoutInput;
}

FILE * NamedPipe::getHerculesStderr()
{
    if (mStderr == NULL)
    {
        mStderr = fopen(mFifo2.c_str(),"w");
    }
    return mStderr;
}

QFile&  NamedPipe::getHerculesStatus()
{
    if (mStderrInput == NULL)
    {
		mStderrInput =  new QFile(mFifo2.c_str());
		mStderrInput->open(QIODevice::ReadOnly);
    }

	return *mStderrInput;
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
