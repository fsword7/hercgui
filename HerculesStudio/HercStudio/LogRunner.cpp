/*
 *  File:       LogRunner.cpp
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

#include "LogRunner.h"
#include "HerculesStudio.h"
#include "NamedPipe.h"
#include "Preferences.h"

LogRunner::LogRunner(SynchronizedQueue& logQueue, HerculesExecutor * herculesExecutor, int maxQueueSize)
: Runner(logQueue), mHerculesExecutor(herculesExecutor), mMaxQueueSize(maxQueueSize)
{
}

LogRunner::~LogRunner()
{

}

void LogRunner::run()
{
    mQueue.push_back("logger started");
    emit newData();

#ifdef hFramework
    mProcess = mHerculesExecutor->getQProcess();
    connect(mProcess,
            SIGNAL(readyReadStandardOutput()),
            this,
            SLOT(readStandardOutput()));
    return;
#else
	QFile& logFile = NamedPipe::getInstance().getHerculesLogfile();
	QByteArray buff;
    mRunning = true;

    while(mRunning)
    {
		buff = logFile.readLine(512);
		if (buff.isEmpty())
		{
            mQueue.push_back("logger ended");
            emit newData();
            QThread::sleep(100);
            break;
        }
		buff[buff.size()-1] = '\0';
        mQueue.push_back(buff);
        emit newData();
        while (mQueue.size() > mMaxQueueSize) // do not flood the queue
        	QThread::msleep(100); // yield
    }
#endif
}

#ifdef hFramework
void LogRunner::readStandardOutput()
{
    mProcess->setReadChannel(QProcess::StandardOutput);
    while (true)
    {
        QByteArray output = mProcess->readLine();
        if (output.length() == 0) break;
        output.replace("\n","\0");
        output.replace("\r","\0");
        mQueue.push_back(output.data());
        emit newData();
    }
}
#endif
