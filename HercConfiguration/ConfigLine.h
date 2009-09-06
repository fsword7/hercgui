/*
 *  File: ConfigLine.h
 *
 *  Author:     Jacob Dekel
 *  Created on:
 *
 *  Copyright (c) 2009 Jacob Dekel
 *
 *	This object manages one line of the configuration file
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

#ifndef CONFIGLINE_H
#define CONFIGLINE_H

#include <string>
#include <utility>
#include <vector>

typedef std::pair<int,int> TokenPair;

class ConfigLine
{
public:
  ConfigLine(const char *line);
  virtual ~ConfigLine();
    void  replaceLine(const std::string& newLine);
    void  replaceParameters(const std::string& newParm);
    void setDeleted(bool parm);
    const std::string& getLine() const;
    inline bool isRemark() const { return mRemark; }
    inline bool isDeleted() const { return mDeleted; }
    inline bool isNew() const { return mNew; }
    inline bool isUpdated() const { return mUpdated; }
    inline void setNew(bool newVal) { mNew=newVal; } ;
    inline void setUpdated(bool updated) { mUpdated=updated; } ;

    std::string getToken(int i) const;
    std::string getLowercaseToken(int i) const;
    std::string getUppercaseToken(int i) const;
    std::string getMultiToken(int from, int to) const;
    inline int size() const  {return mSize;}
    void  parseLine();

protected:
    std::string mLine;

private:
    std::string mKeyword;
    bool   mRemark;
    bool   mDeleted;
    bool   mUpdated;
    bool   mNew;
  std::string::size_type    mKeyStart;
    int    mSize;
    std::vector<TokenPair> mTokens;

    void  tokenize(const std::string& delimiters = " ");
};

#endif // #ifdef CONFIGLINE_H
