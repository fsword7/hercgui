/*
 *  File:       UtilityRunner.cpp
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

#include "UtilityRunner.h"
#include "HerculesStudio.h"
#include "StringTokenizer.h"
#include "ConfigurationEditor.h"


#include <sstream>
#include <errno.h>
#include <cstdio>

UtilityRunner::UtilityRunner(FILE * file)
: mFile(file)
{

}

UtilityRunner::~UtilityRunner()
{

}

void UtilityRunner::run()
{
    if (mFile==NULL)
    {
        std::stringstream s;
        s << "dasdinit open failed " << errno <<  std::endl;
        emit error(s.str().c_str());
        return;
    }

    char buff[512];
    while(true)
    {
        if (fgets(buff,512,mFile) == NULL)
        {
            emit valueChanged(-1);
            return;
        }
        updateStatus(buff);
    }

}

void UtilityRunner::updateStatus(const std::string& line)
{
    Tokenizer::handle pos, lastPos;
    std::string word = StringTokenizer::getFirstWord(line, pos, lastPos, " \t\n");
    outDebug(1, std::cout << "util runner line:" << line << std::endl);
    if (line.length() == 0)
    	return;

    if (word == "HHCDU044I") // dasdinit
    {
        QString qline(line.c_str());
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
    if (line.compare(0,4,"MEM=") == 0)  // dasdisup
    {
        int currMem = ConfigurationEditor::parseNum(line.substr(5),10);
        emit valueChanged(currMem);
        return;
    }
    if (line.compare(0,5,"ETRK=") == 0)  // dasdls
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
        outDebug(2, std::cout << "Value changed " << currMem << std::endl);
        return;
    }
    if (line.compare(0,5,"ETRK=") == 0)  // dasdcat
    {
        int currMem = ConfigurationEditor::parseNum(line.substr(5),10);
        emit valueChanged(currMem);
        outDebug(2, std::cout << "Value changed " << currMem << std::endl);
        return;
    }
    if (line.compare(0,7,"OUTCYL=") == 0)
    {
        int currCyl = ConfigurationEditor::parseNum(line.substr(7),10);
        emit valueChanged(currCyl);
        return;
    }
    if (line.length() > 5 && line.compare(2,4,"CYL=") == 0)
    {
        int currCyl = ConfigurationEditor::parseNum(line.substr(4),10);
        outDebug(5, std::cout << "emitting value changed to " << currCyl << std::endl);
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
        QString qline(line.c_str());
        emit error(qline);
    }
}
