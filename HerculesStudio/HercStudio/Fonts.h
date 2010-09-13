/*
 *  File: Fonts.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 10, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id$
 *
 *	This object is a front-end to the framework fonts services
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

#ifndef FONTS_H_
#define FONTS_H_

#include <QStringList>

class Fonts
{
public:
    static Fonts& getInstance();
    virtual ~Fonts();

    QStringList& getList();
    QString at(int index);
    int indexOf(const QString& fontName);

private:
    Fonts();

    static Fonts *mInstance;
    QStringList mFonts;
};

#endif /* FONTS_H_ */
