/*
 *  File: ConfigLine.cpp
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id: ConfigLine.cpp 34 2009-11-07 06:15:58Z jacob $
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
#include "ConfigLine.h"
#include "HerculesStudio.h"

#include <iostream>
#include <algorithm>

ConfigLine::ConfigLine(const char *line) :
  mLine(line), mRemark(false), mDeleted(false), mUpdated(false), mNew(false), mTokens()
{
    if (mLine.find_first_of("\n") == std::string::npos)
        mLine = mLine + "\n";
    parseLine();
}

ConfigLine::~ConfigLine()
{

}

const std::string& ConfigLine::getLine() const
{
   return mLine;
}

std::string ConfigLine::getToken(int i) const
{
    if (i >= mSize) return "";
    std::string result = mLine.substr(mTokens[i].first, mTokens[i].second ) ;
    return result;
}

std::string ConfigLine::getLowercaseToken(int i) const
{
    std::string result = getToken(i);
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

std::string ConfigLine::getUppercaseToken(int i) const
{
    std::string result = getToken(i);
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

std::string ConfigLine::getMultiToken(int from, int to) const
{
    if (from >= mSize) return "";
    if (to >= mSize || to == 0) to = mSize-1;

    return mLine.substr(mTokens[from].first, mTokens[to].first+mTokens[to].second-mTokens[from].first ) ;
}

void ConfigLine::replaceLine(const std::string& newLine)
{
  outDebug(5, std::cout  << "configLine: new Line '" << newLine << "'" << std::endl);
  outDebug(5, std::cout  << "configLine: old Line  '" << mLine <<     "'" << std::endl);
  mLine = newLine;
  mUpdated = true;
  parseLine();
}

void ConfigLine::replaceParameters(const std::string& newParm)
{
  std::string newString = "  " + getToken(0) + "    " + newParm + "                   #edited By Hercules Studio\n";
  outDebug(5, std::cout  << "configLine: line is now '" << newString << "'" << std::endl);
  outDebug(5, std::cout  << "configLine: old string  '" << mLine <<     "'" << std::endl);
  mLine = newString;
  mUpdated = true;
  parseLine();
}

void ConfigLine::parseLine()
{
  tokenize(" \t\n\r");
  mRemark = false;
  if (mLine[0] == '#')
  {
    mRemark = true;
    return;
  }

  mKeyStart=mLine.find_first_not_of(" \t\n\r");
  if (mKeyStart == std::string::npos) // blank line
  {
    mRemark = true;
    return;
  }
}

void ConfigLine::tokenize(const std::string& delimiters)
{
    std::string::size_type lastPos = mLine.find_first_not_of(delimiters, 0);
    std::string::size_type pos     = mLine.find_first_of(delimiters, lastPos);
    mTokens.clear();
    mSize = 0;

    while (std::string::npos != pos || std::string::npos != lastPos)
    {
        if (pos == std::string::npos) pos = mLine.length() - lastPos;
        TokenPair newToken(lastPos, pos - lastPos);
        mTokens.push_back(newToken);
        mSize++;
        lastPos = mLine.find_first_not_of(delimiters, pos);
        pos = mLine.find_first_of(delimiters, lastPos);
        if (mLine[lastPos] == '#') break;
    }
}

void ConfigLine::setDeleted(bool parm)
{
    mDeleted = parm;
}