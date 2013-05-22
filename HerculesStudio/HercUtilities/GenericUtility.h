/*
 *  File:       GenericUtility.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 26, 2010
 *
 *  Copyright (c) 2010 Jacob Dekel
 *  $Id$
 *
 *	This object is a base class for all utilities
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
#ifndef GENERICUTILITY_H_
#define GENERICUTILITY_H_

#include <QDialog>
#include <QProcess>
#include <string>
#include <vector>

class UtilityExecutor;
class UtilityRunner;

class GenericUtility : public QDialog
{
	Q_OBJECT
public:
	GenericUtility(QString name, QWidget *parent = 0);
	virtual ~GenericUtility();

protected:
	QString mName;
	UtilityExecutor * mExecutor;
	UtilityRunner * mRunner;
	UtilityRunner * mErrorRunner;
	bool mStarted, mFinished, mFinishedOK;
	int mRc;

	bool runOrStopClicked();  // returns false if currently running
    int execute(const std::string & pCommand, const std::string& pPath, std::vector<std::string>& pParameters, const QString &workingDIrectory=QString());
	std::string getFlatCommand(const std::string & command, const std::string& path, std::vector<std::string>& parameters);

protected:
	virtual void finishedSlot() = 0;

private slots:
	void timeout();
	void finished(int result, QProcess::ExitStatus status);
	void errorSlot();
	void runnerError(const QByteArray& line);

signals:
	void error();
	void output(QByteArray line);
};

#endif /* GENERICUTILITY_H_ */
