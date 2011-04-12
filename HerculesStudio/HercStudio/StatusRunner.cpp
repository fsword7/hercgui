/*
 *  File:       StatusRunner.cpp
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

#include "StatusRunner.h"
#include "HerculesStudio.h"
#include "NamedPipe.h"

#include <fcntl.h>

StatusRunner::StatusRunner(SynchronizedQueue& statusQueue, HerculesExecutor * herculesExecutor)
: Runner(statusQueue), mHerculesExecutor(herculesExecutor)
{

}

StatusRunner::~StatusRunner()
{
}

void StatusRunner::run()
{
#ifdef hFramework
	mProcess = mHerculesExecutor->getQProcess();
	connect(mHerculesExecutor->getQProcess(),
			SIGNAL(readyReadStandardError()),
			this,
			SLOT(readStandardError()));
	return;
#else
	QFile& statusFile = NamedPipe::getInstance().getHerculesStatus();
	QByteArray buff;
	buff.resize(512);
	mRunning = true;
	while(mRunning)
	{
		int size;
		if ((size = statusFile.readLine(buff.data(),512)) <= 0)
		{
			emit newData();
			break;
		}
		buff[size-1] = '\0';
		mQueue.push_back(buff);
		emit newData();
	}
#endif
}


void StatusRunner::readStandardError()
{
	mProcess->setReadChannel(QProcess::StandardError);
	while (true)
	{
		QByteArray output = mProcess->readLine();
		if (output.length() == 0) break;
		output[output.length()-1]='\0';
		mQueue.push_back(output.data());
		emit newData();
	}
}

