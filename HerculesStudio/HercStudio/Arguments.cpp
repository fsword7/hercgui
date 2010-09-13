/*
 *  File:       Arguments.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Oct 6, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
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

#include "HerculesStudio.h"
#include "Arguments.h"

#include <string>

Arguments Arguments::instance;

Arguments::Arguments() :
	mHelp(false)
{
}

Arguments::~Arguments()
{
}

Arguments& Arguments::getInstance()
{
	return instance;
}

bool Arguments::helpRequested() const
{
	return mHelp;
}

const std::string& Arguments::configFileName() const
{
	return mConfigFile;
}

const std::string& Arguments::resourceFileName() const
{
	return mResourceFile;
}

int Arguments::parse(int argc, char * argv[])
{
	while(--argc)
    {
		argv++;
	    if (argv[0][0] != '-') return(-1);

	    switch(argv[0][1])
	    {
		case('h'):
            mHelp = true;
            break;

		case('f'):
        	--argc;
            ++argv;
            if (argc > 0) mConfigFile = argv[0];
            break;

		case('r'):
			--argc;
			++argv;
			if (argc > 0) mResourceFile = argv[0];
			break;

		default:
			break;
	    }

    }
	hOutDebug(1,"resource file:'" << mResourceFile << "'");
	return 0;
}

void Arguments::printUsage()
{
	std::cout << "Usage:" << std::endl << std::endl
			<< "\tHerculesStudio [ -f <hercules-config-file> ] [ -r <hercules-run-commands-file> ] [ -h ]"
			<< std::endl;
}

