/*
 *  File:       Preferences.cpp
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id: Preferences.cpp 34 2009-11-07 06:15:58Z jacob $
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

#include "Preferences.h"
#include "HerculesStudio.h"
#include "ConfigurationEditor.h"

#include <QDir>

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cstdlib>

Preferences * Preferences::instance = NULL;

const char * Preferences::sKeywords[] = {
		"HerculesDir=",
		"ConfigurationDir=",
		"LogsDir=" ,
        "Font=",
        "FontSize=",
        "FontVariation=",
        "LogTimeStamp=",
        "RegsViews="};
const char * Preferences::sFileName = "HercStudio.pref";

Preferences::Preferences() :
    mPath(""),
    mVolatileConfigDir("")
{
    assertConfDir();
    for (unsigned int i=0; i< NumberOfLines ; i++)
    {
    	mPrefs.push_back("");
    }
    mPath = QDir::homePath().toStdString() + "/.config";
    setHercDir("");
    setConfigDir("");
    setLogsDir(QDir::homePath().toStdString()+"/Desktop");
    setFontName("(system default)");
    setFontSize(9);
    readPref();
}

Preferences::~Preferences()
{
}

Preferences& Preferences::getInstance()
{
    if (instance == NULL)
        instance = new Preferences();
    return *instance;
}

void Preferences::readPref()
{
    QFile file(getFileName());
    file.open(QIODevice::ReadOnly);

    for (unsigned int i=0; i<NumberOfLines; i++)
    {
        char line[PREF_LINE_LENGTH];
        if ( file.readLine(line, PREF_LINE_LENGTH) == -1 )
            break;
        if (strlen(line) < 2)  // skip null/blank lines
        {
            i--;
            continue;
        }
        std::string value = getValue(line, sKeywords[i]);
        mPrefs[i] = value;
    }
    file.close();


    std::string value;
	try
	{
		value = mPrefs.at(RegsViews);
	}
	catch (const std::out_of_range& e )
	{
		value = "0";
	}
	mView =  std::strtol(value.c_str(), NULL, 0) ;
}

std::string Preferences::getValue(char * line, const char * keyword)
{
    if (strncmp(line,keyword,strlen(keyword)) != 0)
    {
        std::string err = "preferences keyword: ";
        err += keyword;
        std::cerr << err << std::endl;
        throw new std::string(err);
    }
    char *p = line+strlen(keyword);
    std::string ret(p);
    if (ret.at(ret.length()-1) == '\n')
        ret = ret.substr(0,ret.length()-1);
    outDebug(4,std::cout << "keyword:" << keyword << " value=" << ret << std::endl);
    return ret;
}

void Preferences::write()
{
    QString fn(mPath.c_str());

    QFile file(getFileName());
    file.open(QIODevice::WriteOnly);
    for(unsigned int i=0; i<NumberOfLines; i++)
    {
        std::string line = sKeywords[i];
        line += mPrefs[i];

        if (line.find("\n") == std::string::npos)
            line += "\n";
        file.write(line.c_str());
    }
    file.close();
}

QString Preferences::getFileName()
{
    QString ret = mPath.c_str();
    ret += "/";
    ret += sFileName;
    outDebug(4,std::cout << "file name=" << ret.toStdString() << std::endl);
    return ret;
}

void Preferences::setHercDir(const std::string& hercDir)
{
    mPrefs[HerculesDir] = hercDir;
}

void Preferences::setConfigDir(const std::string& configDir)
{
    mPrefs[ConfigurationDir] = configDir;
    mVolatileConfigDir = configDir;
}

void Preferences::setVolatileConfigDir(const std::string& configDir)
{
    mVolatileConfigDir = configDir;
}

const std::string& Preferences::configDir()
{
	if (mVolatileConfigDir =="")
		return mPrefs[ConfigurationDir];
	else
		return mVolatileConfigDir;
}

void Preferences::setLogsDir(const std::string& logsDir)
{
    mPrefs[LogsDir] = logsDir;
}


void Preferences::assertConfDir()
{
    //TODO
}

std::string& Preferences::fontName()
{
    return mPrefs[Font];
}

void Preferences::setFontName(const std::string& font)
{
    mPrefs[Font] = font;
}

int Preferences::fontVariation()
{
    return ConfigurationEditor::parseNum(mPrefs[FontVariation],10);
}

void Preferences::setFontVariation(int fontVariation)
{
    std::stringstream ss;
    ss << fontVariation;
    mPrefs[FontVariation] = ss.str();
}

int Preferences::fontSize()
{
    return ConfigurationEditor::parseNum(mPrefs[FontSize],10);
}

void Preferences::setFontSize(int size)
{
    std::stringstream ss;
    ss << size;
    mPrefs[FontSize] = ss.str();
}

bool Preferences::fontIsBold()
{
    return ( (fontVariation() & Bold ) != 0 );
}

bool Preferences::fontIsItalic()
{
    return ( (fontVariation() & Italic ) != 0 );
}

bool Preferences::logTimestamp()
{
	return mPrefs[LogTimestamp] == "TRUE";
}

void Preferences::setLogTimestamp(bool isTrue)
{
	mPrefs[LogTimestamp] = isTrue ? "TRUE" : "FALSE" ;
}

void Preferences::setRegs(Views v, bool view)
{
	if (view)
		mView |= v;
	else
		mView &= 255-v;
	std::stringstream ss;
	ss << mView;
	mPrefs[RegsViews] = ss.str();
	write();
}

bool Preferences::regs(Views v)
{
	return (mView & v);
}
