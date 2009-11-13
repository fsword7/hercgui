/*
 *  File:       FontPreferences.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Nov 9, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id: Preferences.h 34 2009-11-07 06:15:58Z jacob $
 *
 *	This class manages font preferences for one object
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

#ifndef FONTPREFERENCES_H_
#define FONTPREFERENCES_H_

#include <string>

class QFile;

class FontPreferences {
public:
	enum variations
	{
		Bold = 128,
		Italic = 64
	};
	FontPreferences(std::string line);
	virtual ~FontPreferences();

	void setFontName( const std::string& fontName) ;
	std::string& fontName () ;
	void setFontSize( int size );
	int  fontSize();
	void setFontBoldness( bool isBold );
	void setFontItalic( bool isItalic );
	bool fontIsBold();
	bool fontIsItalic();
	int  fontVariation();
	std::string prefLine();
	void setPrefLine(std::string line);

private:
	std::string mLine;
	std::string mName;
	int  mSize;
	bool mBold;
	bool mItalic;
};

#endif /* FONTPREFERENCES_H_ */
