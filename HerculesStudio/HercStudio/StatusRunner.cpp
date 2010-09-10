/*
 *  File:       StatusRunner.cpp
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id: StatusRunner.cpp 34 2009-11-07 06:15:58Z jacob $
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
	FILE * statusFile = NamedPipe::getInstance().getHerculesStatus();
	char buff[512];
	mRunning = true;
	while(mRunning)
	{
		if (fgets(buff,512,statusFile) == NULL)
		{
			emit newData();
			break;
		}
	if (buff[0]) buff[strlen(buff)-1] = '\0'; // remove CR
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
		output.replace("\n"," ");
		if (output.length() == 0) break;
		mQueue.push_back(output.data());
		emit newData();
	}
}

