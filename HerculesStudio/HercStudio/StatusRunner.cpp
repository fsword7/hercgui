/*
 *  File:       StatusRunner.cpp
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

#include "StatusRunner.h"
#include "NamedPipe.h"

#include <fcntl.h>

StatusRunner::StatusRunner(SynchronizedQueue& statusQueue)
: Runner(statusQueue)
{

}

StatusRunner::~StatusRunner()
{
}

void StatusRunner::run()
{
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
}

