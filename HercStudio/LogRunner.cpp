/*
 *  File:       LogRunner.cpp
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id: LogRunner.cpp 34 2009-11-07 06:15:58Z jacob $
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

#include "LogRunner.h"
#include "NamedPipe.h"
#include "Preferences.h"

#include <fcntl.h>
#include <sys/time.h>


LogRunner::LogRunner(SynchronizedQueue& logQueue)
: Runner(logQueue)
{
}

LogRunner::~LogRunner()
{

}

void LogRunner::run()
{
    FILE * logFile = NamedPipe::getInstance().getHerculesLogfile();

    bool addTimeStamp = Preferences::getInstance().logTimestamp();
    char buff[512];
    char * buffPtr;
    mRunning = true;
    mQueue.push_back("logger started");
    emit newData();
    if (addTimeStamp)
    	buffPtr = buff+10;
    else
    	buffPtr = buff;
    while(mRunning)
    {
        if (fgets(buffPtr,500,logFile) == NULL)
        {
            mQueue.push_back("logger ended");
            emit newData();
            QThread::sleep(100);
            break;
        }
        if (addTimeStamp)
        	getTimeStamp(buff);
        if (buffPtr[0]) buff[strlen(buff)-1] = '\0'; // remove CR
        mQueue.push_back(buff);
        emit newData();
    }
}

void LogRunner::getTimeStamp(char * addr)
{
	static struct timeval  now;
    static time_t          tt;

	gettimeofday( &now, NULL );
	tt = now.tv_sec;
	strncpy( addr, ctime(&tt)+11, 8 );
	memcpy(addr+8, "  ", 2);
}
