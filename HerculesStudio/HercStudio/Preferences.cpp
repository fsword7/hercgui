/*
 *  File:       Preferences.cpp
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009-2013 JacobJacob Dekel
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
#include "Stationery.h"
#include "PrinterItem.h"
#include "DecolationRules.h"

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
	"AutosaveLog",
	"Theme",
	"LogFileLines",
	"GreenLed",
	"Animate",
    "DarkBackground",
    "IplDevice",
    "LastPrinterName"};

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
	std::string Preferences::FUNC() const\
	{ \
		return mSettings->value(sKeywords[TOKEN]).toString().toStdString(); \
	}

#define PrefInt(FUNC,TOKEN, TYPE) \
	int Preferences::FUNC(void) const \
	{ \
		return mSettings->value(sKeywords[TOKEN]).toInt(); \
	}

#define PrefBool(FUNC,TOKEN) \
	bool Preferences::FUNC() const\
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
	mSettings = new QSettings("org.mvsdasd","HerculesStudio");
    if (mSettings->value("Version").toString().compare("1.4") != 0)
	{
        if (mSettings->value("version").toString().compare("1.3") != 0)
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
		mSettings->setValue(sKeywords[GreenLed], false);
		mSettings->setValue(sKeywords[Animate], false);
		mSettings->setValue(sKeywords[DarkBackground], false);
		mSettings->setValue(sKeywords[Version],"1.4");
	}
	if (logFileLines() <= 0) setLogFileLines(5000);
	if (this->fontName(ConfigurationFontObject).size() == 0) setFontName(ConfigurationFontObject,"Monospace");
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
PrefInt(logFileLines, LogFileLines, int)
PrefBool(greenLed, GreenLed)
PrefBool(animate, Animate)
PrefBool(darkBackground, DarkBackground)
Pref(iplDevice,IplDevice)
Pref(lastPrinterName, LastPrinterName)
PrefBool(balloonDecolation, BalloonDecolation)
PrefBool(balloonStationery, BalloonStationery)

PrefSet(setHercDir,HerculesDir)
PrefSet(setConfigDir,ConfigurationDir)
PrefSet(setLogsDir,LogsDir)
PrefSetInt(setPswMode, PswMode, Psw::PswMode)
PrefSetBool(setLogTimestamp, LogTimestamp)
PrefSetBool(setSplitLog, SplitLog)
PrefSetBool(setAutosaveLog,AutosaveLog)
PrefSetBool(setMipsAsGauge,MipsAsGauge)
PrefSetInt(setLogFileLines,LogFileLines,int)
PrefSetBool(setGreenLed, GreenLed)
PrefSetBool(setAnimate, Animate)
PrefSetBool(setDarkBackground, DarkBackground)
PrefSet(setIplDevice, IplDevice)
PrefSet(setLastPrinterName, LastPrinterName)
PrefSetBool(setBalloonDecolation, BalloonDecolation)
PrefSetBool(setBalloonStationery, BalloonStationery)

std::string Preferences::configDir() const
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

Psw::PswMode Preferences::pswMode() const
{
	return static_cast<Psw::PswMode>((mSettings->value(sKeywords[PswMode]).toInt()));
}

void Preferences::setRegs(Views v, bool view)
{
	std::string key = "registers/";
	key += sRegsViews[v];
	mSettings->setValue(key.c_str(),view);
}

bool Preferences::regs(Views v) const
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
std::string Preferences::fontName(FontObject fontObject) const
{
	GenFontKey(key,"/Name")
	return mSettings->value(fullKey.c_str()).toString().toStdString();
}

void Preferences::setFontSize(FontObject fontObject, int fontSize)
{
	GenFontKey(key,"/Size");
	mSettings->setValue(fullKey.c_str(),fontSize);
}

int Preferences::fontSize(FontObject fontObject) const
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

bool Preferences::fontIsBold(FontObject fontObject) const
{
	GenFontKey(key,"/Bold");
	return mSettings->value(fullKey.c_str()).toBool();
}

void Preferences::setItalic(FontObject fontObject, bool italic)
{
	GenFontKey(key,"/Italic");
	mSettings->setValue(fullKey.c_str(),italic);
}

bool Preferences::fontIsItalic(FontObject fontObject) const
{
	GenFontKey(key,"/Italic");
	return mSettings->value(fullKey.c_str()).toBool();
}

const char * Preferences::fontObjectToString(FontObject fontObject) const
{
	switch(fontObject)
	{
		case RegsFontObject: return "RegsFont"; break;
		case PswFontObject: return "PswFont"; break;
		case CommandFontObject: return "CommandFont"; break;
		case ConfigurationFontObject: return "ConfigurationFont"; break;
		default: return "LogFont"; break;
	}
}

// for backward compatibility
void Preferences::write()
{

}

Preferences::Themes Preferences::theme() const
{
	switch (mSettings->value(sKeywords[Theme]).toInt())
	{
	case 1:
		return Modern;
		break;
	default:
		return Classic;
		break;
	}
}

void Preferences::setTheme(Themes theme)
{
	mSettings->setValue(sKeywords[Theme],theme);
}

bool Preferences::setStationery(const QString& name, const QString& stationeryString, bool replace)
{
    QString key = "Stationery/" + name;
    if (stationeryString.length() != 0 || replace)
    {
        mSettings->setValue(key, stationeryString);

        return true;
    }
    else
    {
        return false;
    }
}

bool Preferences::setStationery(const QString& name, const Stationery& stationery, bool)
{
    mSettings->beginGroup("Stationery");
    mSettings->beginGroup(name);
    mSettings->setValue("Name", stationery.mName);
    mSettings->setValue("PaperSize", stationery.mPaperSize);
    mSettings->setValue("Size1", stationery.mSize1);
    mSettings->setValue("Size2", stationery.mSize2);
    mSettings->setValue("TopMargin", stationery.mTopMargin);
    mSettings->setValue("Milimeters", stationery.mMillimeters);
    mSettings->setValue("Portrait", stationery.mPortrait);
    mSettings->setValue("BarsColor", stationery.mBarsColor);
    mSettings->setValue("BarsRGB", stationery.mBarsRGB);
    mSettings->setValue("Decorated", stationery.mDecorated);
    mSettings->setValue("TractorFeedHols", stationery.mTractorFeedHoles);
    mSettings->endGroup();
    mSettings->endGroup();
    return true;
}

StationeryPtr Preferences::stationery(QString name)
{

    mSettings->beginGroup("Stationery");
    mSettings->beginGroup(name);

    StationeryPtr ret(new Stationery(
                          mSettings->value("Name").toString(),
                          mSettings->value("PaperSize").toString(),
                          mSettings->value("Size1").toString(),
                          mSettings->value("Size2").toString(),
                          mSettings->value("TopMargin").toInt(),
                          mSettings->value("Milimeters").toBool(),
                          mSettings->value("Portrait").toBool(),
                          mSettings->value("BarsColor").toString(),
                          mSettings->value("BarsRGB").toString(),
                          mSettings->value("Decorated").toBool(),
                          mSettings->value("TractorFeedHols").toBool()));


    mSettings->endGroup();
    mSettings->endGroup();
    return ret;
}

QStringList Preferences::stationeryList()
{
    defaultStationery();
    mSettings->beginGroup("Stationery");
    QStringList keys = mSettings->childGroups();
    for (int i = 0; i < keys.size(); ++i)
    {
        hOutDebug(3, "key=" << keys[i].toStdString());
    }
    mSettings->endGroup();
    return keys;
}

void Preferences::deleteStationery(const QString& item)
{
    mSettings->beginGroup("Stationery");
    mSettings->remove(item);
    mSettings->endGroup();
}

bool Preferences::setPrinter(const QString& name, const PrinterItem& printerItem, bool)
{
    mSettings->beginGroup("Printers");
    mSettings->beginGroup(name);
    mSettings->setValue("Name", printerItem.mName);
    mSettings->setValue("Ip", printerItem.mIp);
    mSettings->setValue("Port", printerItem.mPort);
    mSettings->setValue("Stationery", printerItem.mStationery);
    mSettings->setValue("Lines-Per-Inch", printerItem.mLinesPerInch);
    mSettings->setValue("Font-Size", printerItem.mFontSize);
    mSettings->setValue("Spool-Directory", printerItem.mSpoolDir);
    mSettings->setValue("Decolation-Name", printerItem.mDecolationName);
    mSettings->endGroup();
    mSettings->endGroup();
    return true;
}

PrinterItemPtr Preferences::printer(QString name)
{
    PrinterItemPtr ret(new PrinterItem()); //TODO : proper constructor
    mSettings->beginGroup("Printers");
    mSettings->beginGroup(name);
    if (name.length() > 0 && (mSettings->value("Name").toString().compare(name) == 0))
    {
        ret->mName           = mSettings->value("Name").toString();
        ret->mIp             = mSettings->value("Ip").toString();
        ret->mPort           = mSettings->value("Port").toInt();
        ret->mStationery     = mSettings->value("Stationery").toString();
        ret->mLinesPerInch   = mSettings->value("Lines-Per-Inch").toInt();
        ret->mFontSize       = mSettings->value("Font-Size").toInt();
        ret->mSpoolDir       = mSettings->value("Spool-Directory").toString();
        ret->mDecolationName = mSettings->value("Decolation-Name").toString();
    }
    mSettings->endGroup();
    mSettings->endGroup();
    return ret;
}

QStringList Preferences::printerList()
{
    mSettings->beginGroup("Printers");
    QStringList keys = mSettings->childGroups();
    for (int i = 0; i < keys.size(); ++i)
    {
        hOutDebug(2, "printer key=" << keys[i].toStdString());
    }
    mSettings->endGroup();
    return keys;
}

void Preferences::deletePrinter(const QString& item)
{
    mSettings->beginGroup("Printers");
    mSettings->remove(item);
    mSettings->endGroup();
}

QStringList Preferences::decolationList()
{
    defaultDecolations();
    mSettings->beginGroup("Decolations");
    QStringList groups = mSettings->childGroups();
    for (int i = 0; i < groups.size(); ++i)
    {
        hOutDebug(3, "key=" << groups[i].toStdString());
    }
    mSettings->endGroup();
    return groups;
}

void Preferences::defaultStationery()
{
    mSettings->beginGroup("Stationery");
    mSettings->remove("Legacy Green");
    mSettings->setValue("Legacy Green/BarsColor","Green");
    mSettings->setValue("Legacy Green/BarsRGB","#ddffdd");
    mSettings->setValue("Legacy Green/Decorated","true");
    mSettings->setValue("Legacy Green/Milimeters","false");
    mSettings->setValue("Legacy Green/Name","Legacy Green");
    mSettings->setValue("Legacy Green/PaperSize","Std FanFold");
    mSettings->setValue("Legacy Green/Portrait","false");
    mSettings->setValue("Legacy Green/Size1","11");
    mSettings->setValue("Legacy Green/Size2","14.875");
    mSettings->setValue("Legacy Green/TopMargin",8);
    mSettings->setValue("Legacy Green/TractorFeedHols","true");
    mSettings->endGroup();

}

void Preferences::defaultDecolations()
{
    mSettings->beginGroup("Decolations");
    mSettings->remove("mvs38");
    mSettings->setValue("mvs38/Accounting/Date/Column","68");
    mSettings->setValue("mvs38/Accounting/Date/Length","21");
    mSettings->setValue("mvs38/Accounting/Date/Line","30");
    mSettings->setValue("mvs38/Accounting/Jobid/Column","19");
    mSettings->setValue("mvs38/Accounting/Jobid/Length","4");
    mSettings->setValue("mvs38/Accounting/Jobid/Line","30");
    mSettings->setValue("mvs38/Accounting/Jobname/Column","25");
    mSettings->setValue("mvs38/Accounting/Jobname/Length","8");
    mSettings->setValue("mvs38/Accounting/Jobname/Line","30");
    mSettings->setValue("mvs38/FileNameTemplate","$Jobname$.pdf");
    mSettings->setValue("mvs38/Seperator/Pages","1");
    mSettings->setValue("mvs38/Seperator/Triggers/Trigger-0/Column","1");
    mSettings->setValue("mvs38/Seperator/Triggers/Trigger-0/Line","30");
    mSettings->setValue("mvs38/Seperator/Triggers/Trigger-0/Text","****");
    mSettings->setValue("mvs38/Seperator/Triggers/Trigger-0/Type","String");
    mSettings->setValue("mvs38/Seperator/Triggers/Trigger-1/Column","9");
    mSettings->setValue("mvs38/Seperator/Triggers/Trigger-1/Line","30");
    mSettings->setValue("mvs38/Seperator/Triggers/Trigger-1/Text","END");
    mSettings->setValue("mvs38/Seperator/Triggers/Trigger-1/Type","String");
    mSettings->setValue("mvs38/Seperator/Triggers/Trigger-2/Column","1");
    mSettings->setValue("mvs38/Seperator/Triggers/Trigger-2/Line","31");
    mSettings->setValue("mvs38/Seperator/Triggers/Trigger-2/Text","****");
    mSettings->setValue("mvs38/Seperator/Triggers/Trigger-2/Type","String");
    mSettings->setValue("mvs38/Seperator/Triggers/Trigger-3/Column","9");
    mSettings->setValue("mvs38/Seperator/Triggers/Trigger-3/Line","31");
    mSettings->setValue("mvs38/Seperator/Triggers/Trigger-3/Text","END");
    mSettings->setValue("mvs38/Seperator/Triggers/Trigger-3/Type","String");
    mSettings->setValue("mvs38/Seperator/Triggers/Trigger-4/Column","1");
    mSettings->setValue("mvs38/Seperator/Triggers/Trigger-4/Line","32");
    mSettings->setValue("mvs38/Seperator/Triggers/Trigger-4/Text","****");
    mSettings->setValue("mvs38/Seperator/Triggers/Trigger-4/Type","String");
    mSettings->setValue("mvs38/Seperator/Triggers/Trigger-5/Column","9");
    mSettings->setValue("mvs38/Seperator/Triggers/Trigger-5/Line","32");
    mSettings->setValue("mvs38/Seperator/Triggers/Trigger-5/Text","END");
    mSettings->setValue("mvs38/Seperator/Triggers/Trigger-5/Type","String");
    mSettings->remove("mvs-legacy");
    mSettings->setValue("mvs-legacy/Accounting/date/Column","18");
    mSettings->setValue("mvs-legacy/Accounting/date/Length","11");
    mSettings->setValue("mvs-legacy/Accounting/date/Line","51");
    mSettings->setValue("mvs-legacy/Accounting/jobid/Column","18");
    mSettings->setValue("mvs-legacy/Accounting/jobid/Length","8");
    mSettings->setValue("mvs-legacy/Accounting/jobid/Line","33");
    mSettings->setValue("mvs-legacy/Accounting/jobname/Column","18");
    mSettings->setValue("mvs-legacy/Accounting/jobname/Length","8");
    mSettings->setValue("mvs-legacy/Accounting/jobname/Line","34");
    mSettings->setValue("mvs-legacy/Accounting/userid/Column","18");
    mSettings->setValue("mvs-legacy/Accounting/userid/Length","8");
    mSettings->setValue("mvs-legacy/Accounting/userid/Line","35");
    mSettings->setValue("mvs-legacy/FileNameTemplate","$date$/$jobname$-$jobid$.pdf");
    mSettings->setValue("mvs-legacy/Seperator/Pages","2");
    mSettings->setValue("mvs-legacy/Seperator/Triggers/Trigger-0/Column","1");
    mSettings->setValue("mvs-legacy/Seperator/Triggers/Trigger-0/Line","35");
    mSettings->setValue("mvs-legacy/Seperator/Triggers/Trigger-0/Text","* USER ID:");
    mSettings->setValue("mvs-legacy/Seperator/Triggers/Trigger-0/Type","String");
    mSettings->setValue("mvs-legacy/Seperator/Triggers/Trigger-1/Column","1");
    mSettings->setValue("mvs-legacy/Seperator/Triggers/Trigger-1/Line","34");
    mSettings->setValue("mvs-legacy/Seperator/Triggers/Trigger-1/Text","* JOB NAME:");
    mSettings->setValue("mvs-legacy/Seperator/Triggers/Trigger-1/Type","String");
    mSettings->setValue("mvs-legacy/Seperator/Triggers/Trigger-2/Column","1");
    mSettings->setValue("mvs-legacy/Seperator/Triggers/Trigger-2/Line","33");
    mSettings->setValue("mvs-legacy/Seperator/Triggers/Trigger-2/Text","* JOBID:");
    mSettings->setValue("mvs-legacy/Seperator/Triggers/Trigger-2/Type","String");
    mSettings->setValue("mvs-legacy/Seperator/Triggers/Trigger-3/Column","1");
    mSettings->setValue("mvs-legacy/Seperator/Triggers/Trigger-3/Line","31");
    mSettings->setValue("mvs-legacy/Seperator/Triggers/Trigger-3/Text","**END*******END");
    mSettings->setValue("mvs-legacy/Seperator/Triggers/Trigger-3/Type","String");
    mSettings->endGroup();
}

void Preferences::deleteDecolation(const QString& item)
{
    mSettings->beginGroup("Decolations");
    mSettings->remove(item);
    QStringList groups = mSettings->childGroups();
    hOutDebug(0,"left:" << groups.count());
    mSettings->endGroup();
}

DecolationRules * Preferences::decolation(QString name)
{
    DecolationRules * decolation = new DecolationRules();
    decolation->setName(name);
    mSettings->beginGroup("Decolations");
    mSettings->beginGroup(name);

    decolation->setFileNameTemplate(mSettings->value("FileNameTemplate").toString());

    mSettings->beginGroup("Seperator");
    decolation->mSeperator.mPages = mSettings->value("Pages").toInt();
    mSettings->beginGroup("Triggers");

    hOutDebug(5, "Triggers count when getting decolation: " << mSettings->allKeys().count());

    for (int i=0; i<mSettings->childGroups().count(); i++)
    {
        mSettings->beginGroup(QString("Trigger-") + QString::number(i));
        hOutDebug(5,"keys:" << mSettings->allKeys().count());
        Trigger t;
        t.setLine(mSettings->value("Line").toInt());
        t.setColumn(mSettings->value("Column").toInt());
        t.setTriggerType(Trigger::encodeTriggerType(mSettings->value("Type").toString())); //TODO
        t.setText(mSettings->value("Text").toString());
        decolation->mSeperator.mTriggers.insert(i,t);
        mSettings->endGroup(); // one trigger
    }
    mSettings->endGroup(); //triggers
    mSettings->endGroup(); //seperator

    mSettings->beginGroup("Accounting");

    for (int i=0; i<mSettings->childGroups().count(); i++)
    {
        QString group = mSettings->childGroups().at(i);
        mSettings->beginGroup(group);
        hOutDebug(5,"group:" << group.toStdString() << " keys:" << mSettings->allKeys().count());
        AccountingField field;
        field.mName = group;
        field.mLine = mSettings->value("Line").toInt();
        field.mColumn = mSettings->value("Column").toInt();
        field.mLength = mSettings->value("Length").toInt();
        decolation->mAccounting.mFields.push_back(field);
        mSettings->endGroup();
    }
    mSettings->endGroup();  //Accounting
    mSettings->endGroup();  //decolation name
    mSettings->endGroup();  //Decolations

    return decolation;
}

void Preferences::setDecolation(DecolationRules &decolation)
{
    mSettings->beginGroup("Decolations");
    mSettings->beginGroup(decolation.name());
    mSettings->remove("");

    mSettings->setValue("FileNameTemplate", decolation.filenameTemplate());

    mSettings->beginGroup("Seperator");
    mSettings->setValue("Pages", decolation.mSeperator.mPages);
    mSettings->beginGroup("Triggers");
    for (int i=0; i<decolation.mSeperator.mTriggers.count(); i++)
    {
        mSettings->beginGroup(QString("Trigger-") + QString::number(i));
        mSettings->setValue("Line", decolation.mSeperator.mTriggers[i].line());
        mSettings->setValue("Column", decolation.mSeperator.mTriggers[i].column());
        mSettings->setValue("Type", Trigger::decodeTriggerType(decolation.mSeperator.mTriggers[i].triggerType()));
        mSettings->setValue("Text", decolation.mSeperator.mTriggers[i].text());
        mSettings->endGroup(); // one trigger
    }
    mSettings->endGroup(); //triggers
    mSettings->endGroup(); //seperator

    mSettings->beginGroup("Accounting");
    for (int i=0; i<decolation.mAccounting.mFields.count(); i++)
    {
        mSettings->beginGroup(decolation.mAccounting.mFields[i].mName);
        mSettings->setValue("Line", decolation.mAccounting.mFields[i].mLine);
        mSettings->setValue("Column", decolation.mAccounting.mFields[i].mColumn);
        mSettings->setValue("Length", decolation.mAccounting.mFields[i].mLength);
        mSettings->endGroup();
    }
    mSettings->endGroup();  //Accounting
    mSettings->endGroup();  //decolation name
    mSettings->endGroup();  //Decolations
}

void Preferences::convert()
{
    hOutDebug(2,"converting old preferences");
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
	setSplitLog(false);
	setAutosaveLog(oldPreferences.autosaveLog());
	setTheme(Classic);
	setFontName(ConfigurationFontObject,"Mono");
}

void Preferences::setHistory(QVector<QString> &history)
{
    QList<QString> list;
    for (int i=0; i<history.size(); i++)
        list.append(history[i]);

    mSettings->setValue(QString("History"),QVariant(list));
}

void Preferences::getHistory(QVector<QString> &history)
{
    QList<QVariant> list;
    list = mSettings->value("History").toList();
    for (int i=0; i<list.size(); i++)
        history.append(list[i].toString());

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
	catch (const std::out_of_range& )
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
    hOutDebug(4,"keyword:" << keyword << " value=" << ret);
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
    hOutDebug(4,"file name=" << ret.toStdString());
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
	catch (const std::out_of_range&)
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
    hOutDebug(4,"keyword:" << keyword << " value=" << ret);
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
    hOutDebug(4,"file name=" << ret.toStdString());
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

