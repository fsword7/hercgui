/*
 *  File:       UtilityExecutor.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id: UtilityExecutor.h 34 2009-11-07 06:15:58Z jacob $
 *
 *	This object runs the various utilities as a child process
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

#ifndef UTILITYEXECUTOR_H_
#define UTILITYEXECUTOR_H_

#include "HerculesStudio.h"

#include <string>
#include <vector>
#include <algorithm>

class UtilityExecutor
{
public:
    UtilityExecutor();
    virtual ~UtilityExecutor();

    int run(const std::string & command, const std::string & pPath, std::vector<std::string> pParameters);
    int getPipeIn();
    int getPipeOut();
    int getPipeError();
    static bool evaluateProcess(int pid);

private:
    int mPipeIn[2];
    int mPipeOut[2];
    int mPipeError[2];
};

#endif /* UTILITYEXECUTOR_H_ */
