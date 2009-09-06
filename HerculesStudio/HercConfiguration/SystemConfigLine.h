/*
 *  File: SystemConfigLine.h
 *
 *  Author:     Jacob Dekel
 *  Created on:
 *
 *  Copyright (c) 2009 Jacob Dekel
 *
 *	This object manages a configuration line of system parameters
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

#ifndef SYSTEMCONFIGLINE_H_
#define SYSTEMCONFIGLINE_H_

#include "ConfigLine.h"

class SystemConfigLine: public ConfigLine
{
public:
    SystemConfigLine(const char *line);
    virtual ~SystemConfigLine();
    std::string getKeyword() const;

};

#endif /* SYSTEMCONFIGLINE_H_ */
