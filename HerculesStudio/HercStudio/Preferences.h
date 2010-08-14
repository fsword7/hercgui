/*
 *  File:       Preferences.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id: Preferences.h 34 2009-11-07 06:15:58Z jacob $
 *
 *	This singleton object manages user preferences
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

#ifndef PREFERENCES_H_
#define PREFERENCES_H_

#include "Psw.h"

#include <string>
#include <vector>

#include <QString>

class Preferences_1_0
{
public:
	enum keys
	{
	HerculesDir = 0,
	ConfigurationDir = 1,
	LogsDir = 2,
	Font = 3,
	FontSize = 4,
	FontVariation = 5,
	LogTimestamp = 6,
	RegsViews = 7
	};
	enum general
	{
		NumberOfLines = 8
	};

	enum Views
	{
		ViewGR32 = 0x80,
		ViewCR32 = 0x40,
		ViewFR32 = 0x20,
		ViewAR32 = 0x10,
		ViewGR64 = 0x08,
		ViewCR64 = 0x04,
		ViewFR64 = 0x02,
		ViewPsw = 0x01
	};

	enum VariationMasks
	{
	  Bold = 128,
	  Italic = 64
	};

	virtual ~Preferences_1_0();
	static Preferences_1_0& getInstance();

	void write();

	void setHercDir(const std::string& hercDir);
	std::string&  hercDir(){return mPrefs[HerculesDir]; } ;
	void setConfigDir(const std::string& logsDir) ;
	void setVolatileConfigDir(const std::string& logsDir) ;
	const std::string&  configDir();
	void setLogsDir(const std::string& logsDir) ;
	std::string& logsDir(){return mPrefs[LogsDir]; } ;
	void setFontName( const std::string& fontName) ;
	std::string& fontName () ;
	void setFontSize( int variation );
	int  fontSize();
	void setFontVariation( int variation );
	int  fontVariation();
	bool fontIsBold();
	bool fontIsItalic();
	bool logTimestamp();
	void setLogTimestamp(bool isTrue);

	// register views
	void setRegs(Views v,bool view);
	bool regs(Views v);

protected:
    Preferences_1_0();
private:
	void readPref();
	void assertConfDir();
	std::string getValue(char * line, const char * keyword);
	QString getFileName();

	std::string mPath;
	int mView;
	std::string mVolatileConfigDir;

	static Preferences_1_0 *instance;
	static const char *sFileName;
	static const char *sKeywords[];
	std::vector<std::string> mPrefs;
};


class Preferences_1_1
{
public:
	enum keys
	{
	HerculesDir = 0,
	ConfigurationDir = 1,
	LogsDir = 2,
	Font = 3,
	FontSize = 4,
	FontVariation = 5,
	LogTimestamp = 6,
	RegsViews = 7,
	Version = 8,
	LogFont = 9,
	RegsFont = 10,
	PswFont = 11,
	CommandFont = 12
	};
	enum FontObject
	{
		LogFontObject = 0,
		RegsFontObject = 1,
		PswFontObject = 2,
		CommandFontObject = 3
	};
	enum general
	{
		NumberOfLines = 13,
		PREF_LINE_LENGTH = 300
	};

	enum Views
	{
		ViewGR32 = 0x80,
		ViewCR32 = 0x40,
		ViewFR32 = 0x20,
		ViewAR32 = 0x10,
		ViewGR64 = 0x08,
		ViewCR64 = 0x04,
		ViewFR64 = 0x02,
		ViewPsw = 0x01
	};

	virtual ~Preferences_1_1();
	static Preferences_1_1& getInstance();

	void write();

	// hercules directory
	void setHercDir(const std::string& hercDir);
	std::string&  hercDir(){return mPrefs[HerculesDir]; } ;

	// configuration default directory
	void setConfigDir(const std::string& logsDir) ;
	void setVolatileConfigDir(const std::string& logsDir) ;
	const std::string&  configDir();

	// log directory
	void setLogsDir(const std::string& logsDir) ;
	std::string& logsDir(){return mPrefs[LogsDir]; } ;

	// font name
	void setFontName( FontObject fontObject, const std::string& fontName) ;
	std::string fontName (FontObject fontObject) ;

	// font size
	void setFontSize( FontObject fontObject, int variation );
	int  fontSize(FontObject fontObject);

	// bold
	void setBold( FontObject fontObject, bool bold);
	bool fontIsBold(FontObject fontObject);

	// italic
	void setItalic (FontObject fontObject, bool italic);
	bool fontIsItalic(FontObject fontObject);

	// logs time stamp
	bool logTimestamp();
	void setLogTimestamp(bool isTrue);
	const std::string&  version();

	// register views
	void setRegs(Views v,bool view);
	bool regs(Views v);

protected:
    Preferences_1_1();

private:
	void readPref();
	void assertConfDir();
	std::string getValue(char * line, const char * keyword);
	QString getFileName();
	const std::string& getVersion();
	keys fontObjectToIndex(FontObject fontObject);

	std::string mPath;
	int mView;
	std::string mVolatileConfigDir;

	static Preferences_1_1 *instance;
	static const char *sFileName;
	static const char *sKeywords[];
	std::vector<std::string> mPrefs;
	static const char cFontLog[], cFontRegs[], cFontPsw[], cFontCommand[];
};

class Preferences_1_2_0
{
public:
	enum keys
	{
	HerculesDir = 0,
	ConfigurationDir = 1,
	LogsDir = 2,
	Font = 3,
	FontSize = 4,
	FontVariation = 5,
	LogTimestamp = 6,
	RegsViews = 7,
	Version = 8,
	LogFont = 9,
	RegsFont = 10,
	PswFont = 11,
	CommandFont = 12,
	MipsAsGauge = 13,
	PswMode = 14,
	SplitLog = 15,
	AutosaveLog = 16
	};
	enum FontObject
	{
		LogFontObject = 0,
		RegsFontObject = 1,
		PswFontObject = 2,
		CommandFontObject = 3
	};
	enum general
	{
		NumberOfLines = 17,
		PREF_LINE_LENGTH = 300
	};

	enum Views
	{
		ViewGR32 = 0x80,
		ViewCR32 = 0x40,
		ViewFR32 = 0x20,
		ViewAR32 = 0x10,
		ViewGR64 = 0x08,
		ViewCR64 = 0x04,
		ViewFR64 = 0x02,
		ViewPsw = 0x01
	};

	virtual ~Preferences_1_2_0();
	static Preferences_1_2_0& getInstance();

	void write();

	// hercules directory
	void setHercDir(const std::string& hercDir);
	std::string&  hercDir(){return mPrefs[HerculesDir]; } ;

	// configuration default directory
	void setConfigDir(const std::string& logsDir) ;
	void setVolatileConfigDir(const std::string& logsDir) ;
	const std::string&  configDir();

	// log directory
	void setLogsDir(const std::string& logsDir) ;
	std::string& logsDir(){return mPrefs[LogsDir]; } ;

	// font name
	void setFontName( FontObject fontObject, const std::string& fontName) ;
	std::string fontName (FontObject fontObject) ;

	// font size
	void setFontSize( FontObject fontObject, int variation );
	int  fontSize(FontObject fontObject);

	// bold
	void setBold( FontObject fontObject, bool bold);
	bool fontIsBold(FontObject fontObject);

	// italic
	void setItalic (FontObject fontObject, bool italic);
	bool fontIsItalic(FontObject fontObject);

	// logs time stamp
	bool logTimestamp();
	void setLogTimestamp(bool isTrue);
	const std::string&  version();

	// register views
	void setRegs(Views v,bool view);
	bool regs(Views v);

	// Mips as gauge
	void setMipsAsGauge(bool gauge);
	bool mipsAsGauge();

	// Psw Mode
	Psw::PswMode pswMode();
	void setPswMode(Psw::PswMode mode);

	// Split log
	bool  splitLog();
	void setSplitLog(bool);

	// Autosave log
	bool  autosaveLog();
	void setAutosaveLog(bool);

protected:
    Preferences_1_2_0();

private:
	void readPref();
	void assertConfDir();
	std::string getValue(char * line, const char * keyword);
	QString getFileName();
	const std::string& getVersion();
	keys fontObjectToIndex(FontObject fontObject);

	QString mPath;
	int mView;
	std::string mVolatileConfigDir;

	static Preferences_1_2_0 *instance;
	static const char *sFileName;
	static const char *sKeywords[];
	std::vector<std::string> mPrefs;
	static const char cFontLog[], cFontRegs[], cFontPsw[], cFontCommand[],
		cMipsAsGauge[], cPswMode[], cSplitLog[], cAutosaveLog[];
};

class QSettings;

class Preferences
{
public:
	enum keys
	{
	HerculesDir = 0,
	ConfigurationDir = 1,
	LogsDir = 2,
	LogTimestamp = 3,
	RegsViews = 4,
	Version = 5,
	LogFont = 6,
	RegsFont = 7,
	PswFont = 8,
	CommandFont = 9,
	MipsAsGauge = 10,
	PswMode = 11,
	SplitLog = 12,
	AutosaveLog = 13
	};

	enum FontObject
	{
		LogFontObject = 0,
		RegsFontObject = 1,
		PswFontObject = 2,
		CommandFontObject = 3
	};

	enum Views
	{
		ViewGR32 = 0,
		ViewCR32 = 1,
		ViewFR32 = 2,
		ViewAR32 = 3,
		ViewGR64 = 4,
		ViewCR64 = 5,
		ViewFR64 = 6,
		ViewPsw = 7
	};

	virtual ~Preferences();
	static Preferences& getInstance();

	void write();

	// hercules directory
	void setHercDir(const std::string& hercDir);
	std::string  hercDir();

	// configuration default directory
	void setConfigDir(const std::string& configDir) ;
	void setVolatileConfigDir(const std::string& logsDir) ;
	std::string  configDir();
	std::string  configDir_();

	// log directory
	void setLogsDir(const std::string& logsDir) ;
	std::string logsDir();

	// font name
	void setFontName( FontObject fontObject, const std::string& fontName) ;
	std::string fontName (FontObject fontObject) ;

	// font size
	void setFontSize( FontObject fontObject, int variation );
	int  fontSize(FontObject fontObject);

	// bold
	void setBold( FontObject fontObject, bool bold);
	bool fontIsBold(FontObject fontObject);

	// italic
	void setItalic (FontObject fontObject, bool italic);
	bool fontIsItalic(FontObject fontObject);

	// logs time stamp
	bool logTimestamp();
	void setLogTimestamp(bool isTrue);
	const std::string&  version();

	// register views
	void setRegs(Views v,bool view);
	bool regs(Views v);

	// Mips as gauge
	void setMipsAsGauge(bool gauge);
	bool mipsAsGauge();

	// Psw Mode
	void setPswMode(Psw::PswMode mode);
	Psw::PswMode pswMode();

	// Split log
	bool  splitLog();
	void setSplitLog(bool);

	// Autosave log
	bool  autosaveLog();
	void setAutosaveLog(bool);

protected:
    Preferences();

private:
	static Preferences *instance;
	QSettings *mSettings;

	static const char *sKeywords[];
	static const char *sRegsViews[];
	std::string mVolatileConfigDir;

	const char * fontObjectToString(FontObject fontObject);
	void convert();
};

#endif /* PREFERENCES_H_ */
