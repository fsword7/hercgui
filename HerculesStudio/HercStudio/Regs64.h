/*
 *  File:       Regs64.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *
 *	This object is the base class for all registers widgets
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

#ifndef REGS64_H_
#define REGS64_H_

#include "Regs32.h"

class Regs64: public Regs32
{
public:
	Regs64(QWidget * parent, std::string command);
	virtual ~Regs64();
	void setActive(bool active);
	void notify(const std::string& statusLine);
	static std::string gCommand;
	static std::string fCommand;
	static std::string cCommand;

protected:
  QLabel mLine5, mLine6, mLine7, mLine8;
};

#endif /* REGS64_H_ */
