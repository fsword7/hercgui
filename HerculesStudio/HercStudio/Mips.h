/*
 *  File:       Mips.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009-2013 Jacob Dekel
 *  $Id$
 *
 *	This object presents the current mips reading as received from hercules
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

#ifndef MIPS_H_
#define MIPS_H_
#include <QString>
#include <QWidget>

class Mips : public QWidget
{
public:
    Mips(QWidget * parent = 0);
    virtual ~Mips();

    virtual void display(double value) = 0;
	virtual void setActive(bool) = 0;
    virtual void move(int, int) = 0;
    virtual void setToolTip(const QString &) = 0;
    virtual void deleteLater(void) = 0;

    virtual bool isVisible() = 0;

};

#endif /* MIPS_H_ */
