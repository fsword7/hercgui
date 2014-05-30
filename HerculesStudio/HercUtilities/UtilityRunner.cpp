/*
 *  File:       UtilityRunner.cpp
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

#include "UtilityRunner.h"
#include "HerculesStudio.h"
#include "UtilityExecutor.h"
#include "StringTokenizer.h"
#include "ConfigurationEditor.h"

#include <QProcess>

#include <sstream>
#include <cerrno>
#include <cstdio>

UtilityRunner::UtilityRunner(UtilityExecutor * utilityExecutor)
: mUtilityExecutor(utilityExecutor), mProcess(utilityExecutor->getQProcess())
{

}

UtilityRunner::~UtilityRunner()
{

}

void UtilityRunner::run()
{
    //mUtilityExecutor->getQProcess()->waitForStarted();
    mProcess = mUtilityExecutor->getQProcess();
    return;
}

void UtilityRunner::updateStatus(const std::string& line)
{
    Tokenizer::handle pos, lastPos;
    std::string word = StringTokenizer::getFirstWord(line, pos, lastPos, " \t\n");
    hOutDebug(1,"util runner line:" << line);
    if (line.length() == 0)
        return;

    if (word == "HHCDU044I") // dasdinit
    {
        QByteArray qline(line.c_str());
        emit error(qline);
        int i = line.find(":") + 2;
        int cyls = ConfigurationEditor::parseNum(line.substr(i),10);
        emit maximumChanged(cyls);
        return;
    }
	if (line.compare(0,5,"NMEM=") == 0)  // dasdisup
	{
		int currMem = ConfigurationEditor::parseNum(line.substr(5),10);
		emit maximumChanged(currMem);
		return;
	}
	if (line.compare(0,4,"POS=") == 0)  // dasdcopy
	{
		int currMem = ConfigurationEditor::parseNum(line.substr(5),10);
		emit valueChanged(currMem);
		return;
	}
	if (line.compare(0,5,"IPOS=") == 0)  // hetmap
	{
		int currMem = ConfigurationEditor::parseNum(line.substr(5),10);
		emit valueChanged(currMem);
		return;
	}
	if (line.compare(0,4,"MEM=") == 0)  // dasdisup
    {
        int currMem = ConfigurationEditor::parseNum(line.substr(5),10);
        emit valueChanged(currMem);
        return;
    }
    if (line.compare(0,5,"ETRK=") == 0)  // dasdls, dasdcat
    {
        int currMem = ConfigurationEditor::parseNum(line.substr(5),10);
        emit maximumChanged(currMem);
        return;
    }
    if (line.compare(0,4,"TRK=") == 0)  // dasdcopy
    {
        int currMem = ConfigurationEditor::parseNum(line.substr(4),10);
        emit valueChanged(currMem);
        return;
    }
    if (line.compare(0,5,"CYLS=") == 0)  // dasdconv
    {
        int currMem = ConfigurationEditor::parseNum(line.substr(5),10);
        emit maximumChanged(currMem);
        return;
    }
    if (line.compare(0,4,"CYL=") == 0)  // dasdinit
    {
        int currMem = ConfigurationEditor::parseNum(line.substr(4),10);
        emit valueChanged(currMem);
        return;
    }
    if (line.compare(0,4,"BLK=") == 0)  // dasdinit
    {
        int currMem = ConfigurationEditor::parseNum(line.substr(4),10);
        emit valueChanged(currMem);
        return;
    }
    if (line.compare(0,5,"TRKS=") == 0)  // dasdcopy
    {
        int currMem = ConfigurationEditor::parseNum(line.substr(5),10);
        emit maximumChanged(currMem);
        return;
    }
    if (line.compare(0,5,"CTRK=") == 0)  // dasdls, dasdcat
    {
        int currMem = ConfigurationEditor::parseNum(line.substr(5),10);
        emit valueChanged(currMem);
        hOutDebug(2,"Value changed " << currMem);
        return;
    }
	if (line.compare(0,7,"OUTCYL=") == 0)
	{
		int currCyl = ConfigurationEditor::parseNum(line.substr(7),10);
		emit valueChanged(currCyl);
		return;
	}
	if (line.compare(0,5,"SIZE=") == 0) // dasdcopy
	{
		// swallow this message
		return;
	}
	if (line.length() > 5 && line.compare(2,4,"CYL=") == 0)
    {
        int currCyl = ConfigurationEditor::parseNum(line.substr(4),10);
        hOutDebug(5, "emitting value changed to " << currCyl);
        emit valueChanged(currCyl);
        return;
    }
    if (line.length() > 2 && line.compare(2,8,"REQCYLS=") == 0)
    {
        int currCyl = ConfigurationEditor::parseNum(line.substr(8),10);
        emit valueChanged(currCyl);
        return;
    }
    if (line.length() > 2 && line.compare(2,4,"BLK=") == 0)
    {
        int currCyl = ConfigurationEditor::parseNum(line.substr(4),10);
        emit valueChanged(currCyl);
        return;
    }
    else
    {
        QByteArray qline(line.c_str());
        emit error(qline);
    }
}
void UtilityRunner::readStandardOutput()
{
	if (mProcess == NULL) return;
	mProcess->setReadChannel(QProcess::StandardOutput);
	while (true)
	{
	QByteArray output = mProcess->readLine();
	if (output.length() == 0) break;
	hOutDebug(2,"util runner:" << output.data());
	updateStatus(QString(output).toStdString());
	}
}
void UtilityRunner::readStandardError()
{
	if (mProcess == NULL) return;
	mProcess->setReadChannel(QProcess::StandardError);
	while (true)
	{
	QByteArray output = mProcess->readLine();
	if (output.length() == 0) break;
	output.replace('\n','\0');
	output.replace('\r','\0');
	hOutDebug(2,"util runner err:" << output.data());
	updateStatus(QString(output).toStdString());
	}
}
