/*
 *  File:       GenericUtility.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 26, 2010
 *
 *  Copyright (c) 2010 Jacob Dekel
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

#include "GenericUtility.h"
#include "UtilityExecutor.h"

#include <QMessageBox>
#include <QTimer>

GenericUtility::GenericUtility(QString name, QWidget *parent) :
QDialog(parent),
mName(name),
mExecutor(NULL), 
mRunner(NULL),
mErrorRunner(NULL)
{
	
}

GenericUtility::~GenericUtility()
{
	delete mRunner;
	delete mErrorRunner;
	delete mExecutor;
}

bool GenericUtility::runOrStopClicked()
{
    if (mExecutor != NULL)
    {
		mExecutor->terminate();
		mExecutor = NULL;
        QMessageBox::warning(this, mName,
                                            mName +" operation was aborted at user's request",
                                            QMessageBox::Ok,
                                            QMessageBox::NoButton);
        return false;
    }
    else
    {
    	return true;
    }
}

int GenericUtility::execute(const std::string & command, const std::string& path, std::vector<std::string>& parameters)
{
	mStarted = true;
	mFinished = false;
    parameters.push_back("EXTERNALGUI");

    mExecutor = new UtilityExecutor(this);
    mRunner = new UtilityRunner(mExecutor);
    mErrorRunner = new UtilityRunner(mExecutor);
    emit output(getFlatCommand(command,path,parameters).c_str());
    int ret = mExecutor->run(command, path, parameters, mRunner, mErrorRunner);

    // do not wait forever
    //QTimer::singleShot(2000, this, SLOT(timeout()));

    // we should know if finished
    connect(mExecutor->getQProcess(),SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(finished(int, QProcess::ExitStatus)));

    // connect to default handlers (superclass can override these slots)
    connect(mRunner, SIGNAL(error(QString)), this, SLOT(runnerError(QString)));
    connect(mErrorRunner, SIGNAL(error(QString)), this, SLOT(runnerError(QString)));

    // general error status handler
    connect(this, SIGNAL(error()), this, SLOT(errorSlot()));

    return ret;
}

void GenericUtility::timeout()
{
}

void GenericUtility::finished(int result, QProcess::ExitStatus status)
{
	mFinished = true;
	mRc = result;
	mFinishedOK = ( (status==QProcess::NormalExit) && (mRc == 0) );
	QString rcLine = mName + " ended; rc=";
	rcLine += QString::number(result);
	emit output(rcLine);
	finishedSlot();
}

void GenericUtility::errorSlot()
{
    QMessageBox::warning(this, mName,
                                mName + " failed. Please check the log for details",
                                QMessageBox::Ok,
                                QMessageBox::NoButton);
    deleteLater();
}

void GenericUtility::runnerError(const QString& line)
{
	hOutDebug(0,line.toStdString());
    emit output(line);
}

std::string GenericUtility::getFlatCommand(const std::string & command, const std::string& path, std::vector<std::string>& parameters)
{
	std::string result;
	result = path;
	if (path.size() > 0 ) result += "/";
	result+=command;
	for (unsigned int i = 0;
			i < parameters.size()-1;
			i++)
	{
		result += " ";
		result+=parameters[i];
	}
	return result;
}
