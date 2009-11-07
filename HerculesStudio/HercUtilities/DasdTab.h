/*
 *  File:       DasdTab.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id: DasdTab.h 34 2009-11-07 06:15:58Z jacob $
 *
 *	This singleton object holds the table of dasds
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

#ifndef DASDTAB_H_
#define DASDTAB_H_

#include <string>
#include <map>

typedef std::pair<std::string,int> SizePair;
typedef std::map<std::string,int> SizeMap;

class DasdTab
{
public:
    virtual ~DasdTab();

    static DasdTab& getInstance();
    static int getCkdSize(const std::string& dev);
    static int getFbaSize(const std::string& dev);

private:
    static SizeMap ckdTab;
    static SizeMap fbaTab;
    static DasdTab mInstance;

    DasdTab();
    void init();
};

#endif /* DASDTAB_H_ */
