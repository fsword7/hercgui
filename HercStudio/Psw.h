/*
 *  File:       Psw.cpp
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *
 *	This object managed display of the psw
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

#ifndef PSW_H_
#define PSW_H_

#include <StatusUpdateCollector.h>

#include <QWidget>
#include <QLabel>

class Psw: public StatusUpdateCollector, public QLabel
{
public:
	Psw( QWidget * parent=NULL );
	virtual ~Psw();

	virtual void notify(const std::string& );
	void setActive(bool active);

private:
	std::string mLine;
	bool mActive;
};

#endif /* PSW_H_ */
