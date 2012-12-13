/*
 *  File: Fonts.cpp
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 10, 2009
 *
 *  Copyright (c) 2009-2013 Jacob Dekel
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

#include "Fonts.h"
#include "HerculesStudio.h"

#include <QFontDatabase>
#include <QStringList>

Fonts * Fonts::mInstance = NULL;

Fonts::Fonts()
{
    QFontDatabase db;
    mFonts = db.families();
    mFonts.insert(0,"(system default)");
    for (int i=0; i<mFonts.size(); ++i)
    {
		outDebug(5,std::cout << i << ": " << mFonts.at(i).toLocal8Bit().constData()  << std::endl);
    }
}

Fonts& Fonts::getInstance()
{
    if (mInstance != NULL)
        return *mInstance;
    mInstance = new Fonts();
    return *mInstance;
}

QStringList& Fonts::getList()
{
    return mFonts;
}

QString Fonts::at(int index)
{
    return mFonts.at(index);
}

int Fonts::indexOf(const QString& fontName)
{
    return mFonts.indexOf(fontName);
}

Fonts::~Fonts()
{
    // TODO Auto-generated destructor stub
}
