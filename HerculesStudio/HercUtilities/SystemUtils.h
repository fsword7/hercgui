/*
 *  File:       SystemUtils.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 24, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id:
 *
 *	This object provides system-level utilities to other utilities
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

#ifndef SYSTEMUTILS_H_
#define SYSTEMUTILS_H_

#include <QString>
#include <string>

class SystemUtils
{
public:
	SystemUtils();
	virtual ~SystemUtils();

	static bool processIsRunning(int pid);
	static bool fileExists(const std::string& filename);
	static bool fileExists(const QString& filename);
};

#endif /* SYSTEMUTILS_H_ */
