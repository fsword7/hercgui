/*
 *  File: Fonts.cpp
 *
 *  Author:     Jacob Dekel
 *  Created on: Nov 9, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id: Fonts.cpp 34 2009-11-07 06:15:58Z jacob $
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

#include "FontPreferences.h"
#include "Preferences.h"
#include "StringTokenizer.h"
#include "ConfigurationEditor.h"
#include "HerculesStudio.h"

#include <QFile>
#include <sstream>

FontPreferences::FontPreferences(std::string line) :
mLine(line), mName("(System Default)"), mSize(9), mBold(false), mItalic(false)
{
	setPrefLine(line);
}

FontPreferences::~FontPreferences()
{

}

void FontPreferences::setFontName( const std::string& fontName)
{
	mName = fontName;
}

std::string& FontPreferences::fontName()
{
	return mName;
}

void FontPreferences::setFontSize( int fontSize )
{
	mSize = fontSize;
	hOutDebug(2, "FontPreferences set size" << mSize);
}

int  FontPreferences::fontSize()
{
	hOutDebug(2, "FontPreferences return size " << mSize);
	return mSize;
}

void FontPreferences::setFontBoldness( bool isBold )
{
	mBold = isBold;
}

void FontPreferences::setFontItalic( bool isItalic )
{
	mItalic = isItalic;
}

bool FontPreferences::fontIsBold()
{
	return mBold;
}

bool FontPreferences::fontIsItalic()
{
	return mItalic;
}

int  FontPreferences::fontVariation()
{
	return (mBold ? Bold : 0) + (mItalic ? Italic : 0);
}

void  FontPreferences::setPrefLine(std::string line)
{
	hOutDebug(2, "FontPreferences Line='" << line << "'");
	if (line.length() == 0) return;
	Tokenizer::handle pos=0, lastPos=0;
	std::string word = StringTokenizer::getFirstWord(line, pos, lastPos, ",");
	if (word == "") return;
	mName = word;
	word = StringTokenizer::getNextWord(line, pos, lastPos, ",");
	mSize = ConfigurationEditor::parseNum(word,10);
	mBold = ("true" == StringTokenizer::getNextWord(line, pos, lastPos, ","));
	mItalic = ("true" == StringTokenizer::getNextWord(line, pos, lastPos, ", \n\t"));
	return;
}

std::string FontPreferences::prefLine()
{
	std::stringstream ss;
	ss << mName << "," << mSize << "," << (mBold ? "true" : "false" ) << "," << (mItalic ? "true" : "false") << "\n";
	return ss.str();
}
