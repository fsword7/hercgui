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
#include "FontPreferences.h"
#include "HerculesStudio.h"
#include "ConfigurationEditor.h"

#include <QDir>

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cstdlib>

Preferences * Preferences::instance = NULL;

const char Preferences::cFontLog[]      = "LogFont=";
const char Preferences::cFontRegs[]     = "RegistersFont=";
const char Preferences::cFontPsw[]      = "PswFont=";
const char Preferences::cFontCommand[]  = "CommandFont=";
const char Preferences::cMipsAsGauge[]    = "MipgAsGauge=";

const char * Preferences::sKeywords[] = {
		"HerculesDir=",
		"ConfigurationDir=",
		"LogsDir=" ,
        "Font=",
        "FontSize=",
        "FontVariation=",
        "LogTimeStamp=",
        "RegsViews=",
        "Version=",
        cFontLog,
        cFontRegs,
        cFontPsw,
        cFontCommand,
        cMipsAsGauge};
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
    setFontName(LogFontObject, "(system default)");
    setFontSize(LogFontObject,9);
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
    if (getVersion() == "")
    {
    	hOutDebug(0,"converting preferences");
    	mPrefs[Version] = "1.2";
		Preferences_1_0& oldP = Preferences_1_0::getInstance();
		setHercDir(oldP.hercDir() );
		setConfigDir(oldP.configDir());
		setLogsDir(oldP.logsDir());
		FontPreferences fp("");
		fp.setFontName( oldP.fontName() );
		fp.setFontSize( oldP.fontSize() );
		fp.setFontBoldness( oldP.fontIsBold() );
		fp.setFontItalic( oldP.fontIsItalic() );
		mPrefs[LogFont] = fp.prefLine();

		FontPreferences dummyFP("");
		mPrefs[RegsFont] = dummyFP.prefLine();
		mPrefs[PswFont] = dummyFP.prefLine();
		mPrefs[CommandFont] = dummyFP.prefLine();
		setMipsAsGauge(false);
		write();
    }
    else if (getVersion() == "1.1")
    {
    	hOutDebug(0,"converting preferences");
    	mPrefs[Version] = "1.2";
    	setMipsAsGauge(false);
    	write();
    }


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

std::string Preferences::fontName(FontObject fontObject)
{
    FontPreferences fp(mPrefs[fontObjectToIndex(fontObject)]);
    return fp.fontName();
}

void Preferences::setFontName(FontObject fontObject, const std::string& fontName)
{
	FontPreferences fp(mPrefs[fontObjectToIndex(fontObject)]);
	fp.setFontName(fontName);
	mPrefs[fontObjectToIndex(fontObject)] = fp.prefLine();

}

int Preferences::fontSize(FontObject fontObject)
{
    FontPreferences fp(mPrefs[fontObjectToIndex(fontObject)]);
	hOutDebug(3, "Preferences::fontSize " << fp.fontSize());
    return fp.fontSize();
}

void Preferences::setFontSize(FontObject fontObject, int size)
{
	FontPreferences fp(mPrefs[fontObjectToIndex(fontObject)]);
	fp.setFontSize(size);
	mPrefs[fontObjectToIndex(fontObject)] = fp.prefLine();
	hOutDebug(3, "Preferences::setFontSize " << fp.prefLine());
}

void Preferences::setBold(FontObject fontObject, bool bold)
{
	FontPreferences fp(mPrefs[fontObjectToIndex(fontObject)]);
	fp.setFontBoldness(bold);
	mPrefs[fontObjectToIndex(fontObject)] = fp.prefLine();
}

bool Preferences::fontIsBold(FontObject fontObject)
{
    FontPreferences fp(mPrefs[fontObjectToIndex(fontObject)]);
    return fp.fontIsBold();
}

void Preferences::setItalic(FontObject fontObject, bool italic)
{
	FontPreferences fp(mPrefs[fontObjectToIndex(fontObject)]);
	fp.setFontItalic(italic);
	mPrefs[fontObjectToIndex(fontObject)] = fp.prefLine();
}

bool Preferences::fontIsItalic(FontObject fontObject)
{
    FontPreferences fp(mPrefs[fontObjectToIndex(fontObject)]);
    return fp.fontIsItalic();
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

const std::string& Preferences::getVersion()
{
	return mPrefs[Version];
}

Preferences::keys Preferences::fontObjectToIndex(FontObject fontObject)
{
	switch(fontObject)
	{
		case RegsFontObject: return RegsFont; break;
		case PswFontObject: return PswFont; break;
		case CommandFontObject: return CommandFont; break;
		default: return LogFont; break;
	}
}

void Preferences::setMipsAsGauge(bool gauge)
{
	mPrefs[MipsAsGauge] = (gauge ? "TRUE" : "FALSE");
}

bool Preferences::mipsAsGauge()
{
	mPrefs[MipsAsGauge] == "TRUE";
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////

Preferences_1_0 * Preferences_1_0::instance = NULL;
const char * Preferences_1_0::sKeywords[] = {
		"HerculesDir=",
		"ConfigurationDir=",
		"LogsDir=" ,
        "Font=",
        "FontSize=",
        "FontVariation=",
        "LogTimeStamp=",
        "RegsViews="};
const char * Preferences_1_0::sFileName = "HercStudio.pref";

Preferences_1_0::Preferences_1_0() :
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

Preferences_1_0::~Preferences_1_0()
{
}


Preferences_1_0& Preferences_1_0::getInstance()
{
    if (instance == NULL)
        instance = new Preferences_1_0();
    return *instance;
}

void Preferences_1_0::readPref()
{
    QFile file(getFileName());
    file.open(QIODevice::ReadOnly);

    for (unsigned int i=0; i<NumberOfLines; i++)
    {
        char line[Preferences::PREF_LINE_LENGTH];
        if ( file.readLine(line, Preferences::PREF_LINE_LENGTH) == -1 )
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

std::string Preferences_1_0::getValue(char * line, const char * keyword)
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

void Preferences_1_0::write()
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

QString Preferences_1_0::getFileName()
{
    QString ret = mPath.c_str();
    ret += "/";
    ret += sFileName;
    outDebug(4,std::cout << "file name=" << ret.toStdString() << std::endl);
    return ret;
}

void Preferences_1_0::setHercDir(const std::string& hercDir)
{
    mPrefs[HerculesDir] = hercDir;
}

void Preferences_1_0::setConfigDir(const std::string& configDir)
{
    mPrefs[ConfigurationDir] = configDir;
    mVolatileConfigDir = configDir;
}

void Preferences_1_0::setVolatileConfigDir(const std::string& configDir)
{
    mVolatileConfigDir = configDir;
}

const std::string& Preferences_1_0::configDir()
{
	if (mVolatileConfigDir =="")
		return mPrefs[ConfigurationDir];
	else
		return mVolatileConfigDir;
}

void Preferences_1_0::setLogsDir(const std::string& logsDir)
{
    mPrefs[LogsDir] = logsDir;
}


void Preferences_1_0::assertConfDir()
{
}

std::string& Preferences_1_0::fontName()
{
    return mPrefs[Font];
}

void Preferences_1_0::setFontName(const std::string& font)
{
    mPrefs[Font] = font;
}

int Preferences_1_0::fontVariation()
{
    return ConfigurationEditor::parseNum(mPrefs[FontVariation],10);
}

void Preferences_1_0::setFontVariation(int fontVariation)
{
    std::stringstream ss;
    ss << fontVariation;
    mPrefs[FontVariation] = ss.str();
}

int Preferences_1_0::fontSize()
{
    return ConfigurationEditor::parseNum(mPrefs[FontSize],10);
}

void Preferences_1_0::setFontSize(int size)
{
    std::stringstream ss;
    ss << size;
    mPrefs[FontSize] = ss.str();
}

bool Preferences_1_0::fontIsBold()
{
    return ( (fontVariation() & Bold ) != 0 );
}

bool Preferences_1_0::fontIsItalic()
{
    return ( (fontVariation() & Italic ) != 0 );
}

bool Preferences_1_0::logTimestamp()
{
	return mPrefs[LogTimestamp] == "TRUE";
}

void Preferences_1_0::setLogTimestamp(bool isTrue)
{
	mPrefs[LogTimestamp] = isTrue ? "TRUE" : "FALSE" ;
}

void Preferences_1_0::setRegs(Views v, bool view)
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

bool Preferences_1_0::regs(Views v)
{
	return (mView & v);
}

