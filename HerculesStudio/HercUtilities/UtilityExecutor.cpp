/*
 *  File:       UtilityExecutor.cpp
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

#include "UtilityExecutor.h"
#include "HerculesStudio.h"
#include "UtilityRunner.h"

#include <QProcess>
#include <QString>
#include <QStringList>
#include <cerrno>
#include <cstdio>

UtilityExecutor::UtilityExecutor(QObject *parent) :QObject(parent)
{
    mProcess = NULL;
}

UtilityExecutor::~UtilityExecutor()
{
}

//TODO convert all std::string parameters to QString
int UtilityExecutor::run(const std::string & pCommand, const std::string& pPath, std::vector<std::string> pParameters, UtilityRunner * runner,
        UtilityRunner * errorRunner, const QString& workingDirectory)
{
    QString program = pPath.c_str();
    if (pPath.size() > 0) program += "/";
    program += pCommand.c_str();
    QStringList arguments;
    for (unsigned int i=0; i<pParameters.size(); i++)
    {
        arguments << pParameters[i].c_str();
    }
    mProcess = new QProcess();
    if (runner != NULL)
    {
        runner->start();
        connect(mProcess,
                SIGNAL(readyReadStandardOutput()),
                runner,
                SLOT(readStandardOutput()));
    }
    if (errorRunner != NULL)
    {
        errorRunner->start();
        connect(mProcess,
                SIGNAL(readyReadStandardError()),
                errorRunner,
                SLOT(readStandardError()));
    }
    for (int i=0 ; i<arguments.size(); i++)
        hOutDebug(3, " " << arguments.value(i).toStdString());
    if (!workingDirectory.isEmpty())
        mProcess->setWorkingDirectory(workingDirectory);
    hOutDebug(3, mProcess->workingDirectory().toStdString());
    mProcess->start(program,arguments);
    Q_PID pid = mProcess->pid();

    if (pid != 0)
        return 0;
    else
        return -1;
}

bool UtilityExecutor::running()
{
    if (mProcess != NULL)
        return mProcess->state() == QProcess::Running;
    else return false;

}

void UtilityExecutor::terminate()
{
    if (mProcess != NULL)
	{
        //mProcess->close();
		mProcess = NULL;
	}
}
