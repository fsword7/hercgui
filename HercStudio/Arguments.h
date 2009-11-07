/*
 *  File:       Arguments.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Oct 6, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id: Arguments.h 34 2009-11-07 06:15:58Z jacob $
 *
 *	Arguments handling class
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

#ifndef ARGUMENTS_H_
#define ARGUMENTS_H_

class Arguments
{
public:
	virtual ~Arguments();
	static Arguments& getInstance();

	int parse(int argc, char *argv[]);
	void printUsage();

	bool helpRequested() const;
	const std::string& configFileName() const;
	const std::string& resourceFileName() const;
private:
	Arguments();

	static Arguments instance;

	bool mHelp;
	std::string mConfigFile;
	std::string mResourceFile;
};

#endif /* ARGUMENTS_H_ */
