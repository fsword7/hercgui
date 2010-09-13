/*
 *  File:       Preferences.cpp
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
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

#include "Preferences.h"
#include "HerculesStudio.h"
#include "SystemUtils.h"
#include "ConfigurationEditor.h"

#include <QDir>
#include <QSettings>

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cstdlib>

Preferences * Preferences::instance = NULL;

const char * Preferences::sKeywords[] = {
		"HerculesDir",
		"ConfigurationDir",
		"LogsDir" ,
        "LogTimeStamp",
        "RegsViews",
        "Version",
        "LogFont",
        "RegistersFont",
        "PswFont",
        "CommandFont",
        "MipgAsGauge",
        "PswMode",
        "SplitLog",
        "AutosaveLog"};

const char * Preferences::sRegsViews[] = {
		"ViewGR32",
		"ViewCR32",
		"ViewFR32",
		"ViewAR32",
		"ViewGR64",
		"ViewCR64",
		"ViewFR64",
		"ViewPsw"
};

#define PrefSet(FUNC,TOKEN) \
	void Preferences::FUNC(const std::string& p) \
	{ \
		mSettings->setValue(sKeywords[TOKEN], p.c_str()); \
	}

#define PrefSetInt(FUNC,TOKEN, TYPE) \
	void Preferences::FUNC(const TYPE p) \
	{ \
		mSettings->setValue(sKeywords[TOKEN], p); \
	}

#define PrefSetBool(FUNC,TOKEN) \
	void Preferences::FUNC(const bool p) \
	{ \
		mSettings->setValue(sKeywords[TOKEN], p); \
	}

#define Pref(FUNC,TOKEN) \
	std::string Preferences::FUNC() \
	{ \
		return mSettings->value(sKeywords[TOKEN]).toString().toStdString(); \
	}

#define PrefInt(FUNC,TOKEN, TYPE) \
	TYPE Preferences::FUNC() \
	{ \
		return mSettings->value(sKeywords[TOKEN]).toInt(); \
	}

#define PrefBool(FUNC,TOKEN) \
	bool Preferences::FUNC() \
	{ \
		return mSettings->value(sKeywords[TOKEN]).toBool(); \
	}

#define GenFontKey(KEY,TYPE) \
		const char *key = fontObjectToString(fontObject); \
		std::string fullKey = "fonts/"; \
		fullKey += key; \
		fullKey += TYPE;

Preferences::Preferences()
{
	mSettings = new QSettings("org.mvsdasd","HerculesStudioz");
	if (mSettings->value("version") != "1.3")
	{
		QString oldFile = QDir::homePath() + "/.config/HercStudio.pref";
		if (SystemUtils::fileExists(oldFile))
		{
			convert();
		}
		else
		{
			// set default values
			setLogsDir(QDir::homePath().toStdString()+"/Desktop");
		    setPswMode(Psw::StatusBar);
		}
		mSettings->setValue(sKeywords[Version],"1.3");
	}
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

Pref(hercDir,HerculesDir)
Pref(configDir_,ConfigurationDir)
Pref(logsDir,LogsDir)
PrefBool(logTimestamp,LogTimestamp)
PrefBool(splitLog,SplitLog)
PrefBool(autosaveLog, AutosaveLog)
PrefBool(mipsAsGauge, MipsAsGauge)

PrefSet(setHercDir,HerculesDir)
PrefSet(setConfigDir,ConfigurationDir)
PrefSet(setLogsDir,LogsDir)
PrefSetInt(setPswMode, PswMode, Psw::PswMode)
PrefSetBool(setLogTimestamp, LogTimestamp)
PrefSetBool(setSplitLog, SplitLog)
PrefSetBool(setAutosaveLog,AutosaveLog)
PrefSetBool(setMipsAsGauge,MipsAsGauge)

std::string Preferences::configDir()
{
	if (mVolatileConfigDir =="")
		return configDir_();
	else
		return mVolatileConfigDir;
}


void Preferences::setVolatileConfigDir(const std::string& configDir)
{
    mVolatileConfigDir = configDir;
}

Psw::PswMode Preferences::pswMode()
{
	return static_cast<Psw::PswMode>((mSettings->value(sKeywords[PswMode]).toInt()));
}

void Preferences::setRegs(Views v, bool view)
{
	std::string key = "registers/";
	key += sRegsViews[v];
	mSettings->setValue(key.c_str(),view);
}

bool Preferences::regs(Views v)
{
	std::string key = "registers/";
	key += sRegsViews[v];
	return mSettings->value(key.c_str()).toBool();
}

void Preferences::setFontName(FontObject fontObject, const std::string& fontName)
{
	GenFontKey(key,"/Name")
	mSettings->setValue(fullKey.c_str(),fontName.c_str());
}
std::string Preferences::fontName(FontObject fontObject)
{
	GenFontKey(key,"/Name")
	return mSettings->value(fullKey.c_str()).toString().toStdString();
}

void Preferences::setFontSize(FontObject fontObject, int fontSize)
{
	GenFontKey(key,"/Size");
	mSettings->setValue(fullKey.c_str(),fontSize);
}

int Preferences::fontSize(FontObject fontObject)
{
	GenFontKey(key,"/Size");
	if (mSettings->contains(fullKey.c_str()))
		return mSettings->value(fullKey.c_str()).toInt();
	else
		return 9;
}

void Preferences::setBold(FontObject fontObject, bool bold)
{
	GenFontKey(key,"/Bold");
	mSettings->setValue(fullKey.c_str(),bold);
}

bool Preferences::fontIsBold(FontObject fontObject)
{
	GenFontKey(key,"/Bold");
	return mSettings->value(fullKey.c_str()).toBool();
}

void Preferences::setItalic(FontObject fontObject, bool italic)
{
	GenFontKey(key,"/Italic");
	mSettings->setValue(fullKey.c_str(),italic);
}

bool Preferences::fontIsItalic(FontObject fontObject)
{
	GenFontKey(key,"/Italic");
	return mSettings->value(fullKey.c_str()).toBool();
}


const char * Preferences::fontObjectToString(FontObject fontObject)
{
	switch(fontObject)
	{
		case RegsFontObject: return "RegsFont"; break;
		case PswFontObject: return "PswFont"; break;
		case CommandFontObject: return "CommandFont"; break;
		default: return "LogFont"; break;
	}
}

// for backward compatibility
void Preferences::write()
{

}

void Preferences::convert()
{
	hOutDebug(0,"converting old preferences");
	Preferences_1_2_0& oldPreferences = Preferences_1_2_0::getInstance();
	setHercDir(oldPreferences.hercDir());
	setConfigDir(oldPreferences.configDir());
	setLogsDir(oldPreferences.logsDir());
	for (FontObject f=LogFontObject; f<CommandFontObject; f=static_cast<FontObject>(f+1) )
	{
		setFontName(f,oldPreferences.fontName(static_cast<Preferences_1_2_0::FontObject>(f)));
		setFontSize(f,oldPreferences.fontSize(static_cast<Preferences_1_2_0::FontObject>(f)));
		setBold(f,oldPreferences.fontIsBold(static_cast<Preferences_1_2_0::FontObject>(f)));
		setItalic(f,oldPreferences.fontIsItalic(static_cast<Preferences_1_2_0::FontObject>(f)));
	}
	setLogTimestamp(oldPreferences.logTimestamp());
	setRegs(ViewGR32,oldPreferences.regs(Preferences_1_2_0::ViewGR32));
	setRegs(ViewCR32,oldPreferences.regs(Preferences_1_2_0::ViewCR32));
	setRegs(ViewFR32,oldPreferences.regs(Preferences_1_2_0::ViewFR32));
	setRegs(ViewAR32,oldPreferences.regs(Preferences_1_2_0::ViewAR32));
	setRegs(ViewGR64,oldPreferences.regs(Preferences_1_2_0::ViewGR64));
	setRegs(ViewCR64,oldPreferences.regs(Preferences_1_2_0::ViewCR64));
	setRegs(ViewFR64,oldPreferences.regs(Preferences_1_2_0::ViewFR64));
	setRegs(ViewPsw,oldPreferences.regs(Preferences_1_2_0::ViewPsw));
	setMipsAsGauge(oldPreferences.mipsAsGauge());
	setPswMode(oldPreferences.pswMode());
	setSplitLog(oldPreferences.splitLog());
	setAutosaveLog(oldPreferences.autosaveLog());
}

//////////////////////////////////////////////////////////////////////
#include "FontPreferences.h"

Preferences_1_2_0 * Preferences_1_2_0::instance = NULL;

const char Preferences_1_2_0::cFontLog[]      = "LogFont=";
const char Preferences_1_2_0::cFontRegs[]     = "RegistersFont=";
const char Preferences_1_2_0::cFontPsw[]      = "PswFont=";
const char Preferences_1_2_0::cFontCommand[]  = "CommandFont=";
const char Preferences_1_2_0::cMipsAsGauge[]  = "MipgAsGauge=";
const char Preferences_1_2_0::cPswMode[]      = "PswMode=";
const char Preferences_1_2_0::cSplitLog[]     = "SplitLog=";
const char Preferences_1_2_0::cAutosaveLog[]  = "AutosaveLog=";

const char * Preferences_1_2_0::sKeywords[] = {
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
        cMipsAsGauge,
		cPswMode,
		cSplitLog,
		cAutosaveLog};
const char * Preferences_1_2_0::sFileName = "HercStudio.pref";

Preferences_1_2_0::Preferences_1_2_0() :
    mPath(""),
    mVolatileConfigDir("")
{
    assertConfDir();
    for (unsigned int i=0; i< NumberOfLines ; i++)
    {
    	mPrefs.push_back("");
    }
    mPath = QDir::homePath() + "/.config";
    setHercDir("");
    setConfigDir("");
    setLogsDir(QDir::homePath().toStdString()+"/Desktop");
    setFontName(LogFontObject, "(system default)");
    setFontSize(LogFontObject,9);
    setPswMode(Psw::StatusBar);
    readPref();
}

Preferences_1_2_0::~Preferences_1_2_0()
{

}

Preferences_1_2_0& Preferences_1_2_0::getInstance()
{
    if (instance == NULL)
        instance = new Preferences_1_2_0();
    return *instance;
}

void Preferences_1_2_0::readPref()
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
    	hOutDebug(1,"converting preferences");
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
		setPswMode(Psw::StatusBar);
		setSplitLog(true);
		setAutosaveLog(false);
		write();
    }
    else if (getVersion() == "1.1")
    {
    	hOutDebug(1,"converting preferences");
    	mPrefs[Version] = "1.2";
    	setMipsAsGauge(false);
    	setPswMode(Psw::StatusBar);
		setSplitLog(true);
		setAutosaveLog(false);
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

std::string Preferences_1_2_0::getValue(char * line, const char * keyword)
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

void Preferences_1_2_0::write()
{
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

QString Preferences_1_2_0::getFileName()
{
    QString ret = mPath;
    ret += "/";
    ret += sFileName;
    outDebug(4,std::cout << "file name=" << ret.toStdString() << std::endl);
    return ret;
}

void Preferences_1_2_0::setHercDir(const std::string& hercDir)
{
    mPrefs[HerculesDir] = hercDir;
}

void Preferences_1_2_0::setConfigDir(const std::string& configDir)
{
    mPrefs[ConfigurationDir] = configDir;
    mVolatileConfigDir = configDir;
}

void Preferences_1_2_0::setVolatileConfigDir(const std::string& configDir)
{
    mVolatileConfigDir = configDir;
}

const std::string& Preferences_1_2_0::configDir()
{
	if (mVolatileConfigDir =="")
		return mPrefs[ConfigurationDir];
	else
		return mVolatileConfigDir;
}

void Preferences_1_2_0::setLogsDir(const std::string& logsDir)
{
    mPrefs[LogsDir] = logsDir;
}


void Preferences_1_2_0::assertConfDir()
{
    //TODO
}

std::string Preferences_1_2_0::fontName(FontObject fontObject)
{
    FontPreferences fp(mPrefs[fontObjectToIndex(fontObject)]);
    return fp.fontName();
}

void Preferences_1_2_0::setFontName(FontObject fontObject, const std::string& fontName)
{
	FontPreferences fp(mPrefs[fontObjectToIndex(fontObject)]);
	fp.setFontName(fontName);
	mPrefs[fontObjectToIndex(fontObject)] = fp.prefLine();

}

int Preferences_1_2_0::fontSize(FontObject fontObject)
{
    FontPreferences fp(mPrefs[fontObjectToIndex(fontObject)]);
	hOutDebug(3, "Preferences::fontSize " << fp.fontSize());
    return fp.fontSize();
}

void Preferences_1_2_0::setFontSize(FontObject fontObject, int size)
{
	FontPreferences fp(mPrefs[fontObjectToIndex(fontObject)]);
	fp.setFontSize(size);
	mPrefs[fontObjectToIndex(fontObject)] = fp.prefLine();
	hOutDebug(3, "Preferences::setFontSize " << fp.prefLine());
}

void Preferences_1_2_0::setBold(FontObject fontObject, bool bold)
{
	FontPreferences fp(mPrefs[fontObjectToIndex(fontObject)]);
	fp.setFontBoldness(bold);
	mPrefs[fontObjectToIndex(fontObject)] = fp.prefLine();
}

bool Preferences_1_2_0::fontIsBold(FontObject fontObject)
{
    FontPreferences fp(mPrefs[fontObjectToIndex(fontObject)]);
    return fp.fontIsBold();
}

void Preferences_1_2_0::setItalic(FontObject fontObject, bool italic)
{
	FontPreferences fp(mPrefs[fontObjectToIndex(fontObject)]);
	fp.setFontItalic(italic);
	mPrefs[fontObjectToIndex(fontObject)] = fp.prefLine();
}

bool Preferences_1_2_0::fontIsItalic(FontObject fontObject)
{
    FontPreferences fp(mPrefs[fontObjectToIndex(fontObject)]);
    return fp.fontIsItalic();
}

bool Preferences_1_2_0::logTimestamp()
{
	return mPrefs[LogTimestamp] == "TRUE";
}

void Preferences_1_2_0::setLogTimestamp(bool isTrue)
{
	mPrefs[LogTimestamp] = isTrue ? "TRUE" : "FALSE" ;
}

void Preferences_1_2_0::setRegs(Views v, bool view)
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

bool Preferences_1_2_0::regs(Views v)
{
	return (mView & v);
}

const std::string& Preferences_1_2_0::getVersion()
{
	return mPrefs[Version];
}

Preferences_1_2_0::keys Preferences_1_2_0::fontObjectToIndex(FontObject fontObject)
{
	switch(fontObject)
	{
		case RegsFontObject: return RegsFont; break;
		case PswFontObject: return PswFont; break;
		case CommandFontObject: return CommandFont; break;
		default: return LogFont; break;
	}
}

void Preferences_1_2_0::setMipsAsGauge(bool gauge)
{
	mPrefs[MipsAsGauge] = (gauge ? "TRUE" : "FALSE");
}

bool Preferences_1_2_0::mipsAsGauge()
{
	return mPrefs[MipsAsGauge] == "TRUE";
}

void Preferences_1_2_0::setPswMode(Psw::PswMode mode)
{
	mPrefs[PswMode] = ( mode == Psw::Docked ? "0" : "1");
}

Psw::PswMode Preferences_1_2_0::pswMode()
{
	return ( (strcmp(mPrefs[PswMode].c_str(),"0") == 0 ) ?
		Psw::Docked :
		Psw::StatusBar ) ;
}

void Preferences_1_2_0::setSplitLog(bool split)
{
	mPrefs[SplitLog] = ( split ? "TRUE" : "FALSE");
}

bool Preferences_1_2_0::splitLog()
{
	return ( (strcmp(mPrefs[SplitLog].c_str(),"TRUE") == 0 ) ?
		true :
		false ) ;
}

void Preferences_1_2_0::setAutosaveLog(bool autosave)
{
	mPrefs[AutosaveLog] = ( autosave ? "TRUE" : "FALSE");
}

bool Preferences_1_2_0::autosaveLog()
{
	return ( (strcmp(mPrefs[AutosaveLog].c_str(),"TRUE") == 0 ) ?
		true :
		false ) ;
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
        char line[Preferences_1_2_0::PREF_LINE_LENGTH];
        if ( file.readLine(line, Preferences_1_2_0::PREF_LINE_LENGTH) == -1 )
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

