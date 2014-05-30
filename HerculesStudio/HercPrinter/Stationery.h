/*
 *  File: Stationery.h
 *
 *  Author:     Jacob Dekel
 *  Created on: May 10, 2014
 *
 *  Copyright (c) 2009-2014 Jacob Dekel
 *  $Id$
 *
 *  This object represents the stationery used by the Hercules Studio printer
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

#ifndef STATIONERY_H
#define STATIONERY_H

#include "HerculesStudio.h"
#include <QString>

mk_shared_ptr(Stationery)

class Stationery
{
public:
    Stationery(const QString& name, const QString& paperSize, const QString& size1, const QString& size2,
               int topMargin, bool millimters, bool portrait, const QString& barsColor,
               const QString& barsRGB, bool Decorated, bool tractorFeedHoles);
    unsigned static int paperSizes();
    static const char *paperSize(unsigned int i);
    int *getBarsColor(int * fill);
    int getBarsColorIndex();

    static const char *mBarsColorNames[];

public: // TODO: getters/setters
    QString mName;
    QString mPaperSize;
    QString mSize1;
    QString mSize2;
    int     mTopMargin;
    bool    mMillimeters;
    bool    mPortrait;
    QString mBarsColor;
    QString mBarsRGB;
    bool    mDecorated;
    bool    mTractorFeedHoles;

};

#endif // STATIONERY_H
