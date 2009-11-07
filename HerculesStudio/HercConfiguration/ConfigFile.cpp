/*
 *  File: ConfigFile.cpp
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id: ConfigFile.cpp 34 2009-11-07 06:15:58Z jacob $
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

#include "ConfigFile.h"
#include "Configuration.h"
#include "SystemConfigLine.h"
#include "DeviceConfigLine.h"

#include <cstdio>
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

#define synonymEntry(keyword1, keyword2)  {#keyword1, #keyword2}, {#keyword2, #keyword1},

struct SynonymTableEntry ConfigFile::mSynonyms[] = {
        synonymEntry(ASN_AND_LX_REUSE,ALRF)
        {"", ""}
};


ConfigFile::ConfigFile(const string& fileName) :
    mLastSys(-1), mNewConfig(false)
{
    mFileName = fileName;
    initialize();
}

void ConfigFile::initialize()
{
	mLastSys = -1;
    FILE * f = fopen(mFileName.c_str(), "r");
    if (f==NULL)
    {
        outDebug(4, std::cout << "file not found:" << mFileName << std::endl);
        openTemplate();
    }
    mFileArray.clear();

    char buff[MAX_CONFIG_LINE];
    bool devices = false;
    while ( getNextLine(buff, MAX_CONFIG_LINE, f) != NULL )
    {
        std::string s(buff);
        hOutDebug(5,"LINE:" << buff);
        size_t nb = s.find_first_not_of(" ");
        if (nb != std::string::npos &&
            (s.substr(nb,1).find_first_not_of("0123456789") != std::string::npos) ) // not device
        {
            outDebug(4, std::cout << mFileArray.size() << " " << buff << std::endl);
            SystemConfigLine l(s.c_str());
            mFileArray.push_back(l);

            if (!l.isRemark() && !devices)
                mLastSys = mFileArray.size()-1;
        }
        else  // device
        {
            devices=true;
            DeviceConfigLine l(s.c_str());
            int devno = ConfigurationEditor::parseNum(l.getToken(0),16);
            std::stringstream newLine;
            newLine << std::hex << std::setw(4) << std::setfill('0') << std::uppercase
                << devno << " " << l.getMultiToken(1,0) << "\n";
            DeviceConfigLine n(newLine.str().c_str());
            mFileArray.push_back(n);
            if (l.getMultiCount() > 1)
            {
                int additions = l.getMultiCount();
                while (additions>1)
                {
                    devno++;
                    newLine.str("");
                    newLine << std::hex << std::setw(4) << std::setfill('0') << std::uppercase
                        << devno << " " << l.getMultiToken(1,0) << "\n";
                    DeviceConfigLine n(newLine.str().c_str());
                    mFileArray.push_back(n);
                    additions--;
                }
            }
        }
    }
    if (f != NULL) fclose(f);
    mChanged = false;

    outDebug(3, std::cout << "### last sys=" << mLastSys << " size=" << mFileArray.size() << std::endl);
    outDebug(3, std::cout << mFileArray[mLastSys].getLine() << std::endl)

}

ConfigFile::~ConfigFile()
{

}

void ConfigFile::addNonDev(SystemConfigLine * configLine)
{
    FileArray::iterator it = mFileArray.begin() + mLastSys+1;
    mFileArray.insert(it, *configLine);
    ++mLastSys;
    mChanged = true;
}

void ConfigFile::addDev(DeviceConfigLine * configLine)
{
    outDebug(2, std::cout << "add dev after " << mFileArray.size() << " line:'" << configLine->getLine() << std::endl);
    configLine->parseLine();
    mFileArray.push_back(*configLine);
    mChanged = true;
}

const std::string& ConfigFile::getFileName() const
{
    return mFileName;
}

DeviceConfigLine * ConfigFile::getDevice(int index) const
{
    if (index <= mLastSys)
    {
        std::cout << "ConfigFile[" << index << "] requested where lastsys=" << mLastSys << std::endl;
        abort();
    }
    if (static_cast<unsigned int>(index) >= mFileArray.size())
    {
        std::cout << "ConfigFile[" << index << "] requested where size=" << mFileArray.size() << std::endl;
        abort();
    }
    return static_cast<DeviceConfigLine *> (const_cast<ConfigLine *>(&mFileArray.at(index)));
}

DeviceConfigLine * ConfigFile::getDevice(const std::string& devno) const
{
    for (size_t i=mLastSys+1; i<mFileArray.size(); i++)
    {
        DeviceConfigLine * l = getDevice(i);
        if (l->getToken(0) == devno) return l;
    }
    return NULL;
}

DeviceConfigLine * ConfigFile::findFirstDeviceByType(const char * type) const
{
	DeviceConfigLine * ret;
	for (unsigned int i=mLastSys+1; i < mFileArray.size() ; i++)
	{
		ret = getDevice(i);
		if (ret->getToken(1) == type)
			return ret;
	}
	return NULL;
}

SystemConfigLine * ConfigFile::operator[] (int index) const
{
    if (index > mLastSys)
    {
        outDebug(5, std::cout << "ConfigFile[" << index << "] requested where lastsys=" << mLastSys << std::endl);
        abort();
    }
    return static_cast<SystemConfigLine *> (const_cast<ConfigLine *>(&mFileArray.at(index)));
}

int ConfigFile::size() const
{
        return mFileArray.size();
}

bool ConfigFile::write(const string& fileName)
{
    FILE * f = fopen(fileName.c_str(), "w");
    if (f==NULL) return false;
    for (int i=0; i<size(); i++)
    {
        ConfigLine& configLine = mFileArray[i];
        if (!configLine.isNew() || configLine.isUpdated())
            fputs(mFileArray[i].getLine().c_str(),f);
        configLine.setUpdated(false);
    }
    fclose(f);
    mFileName = fileName;
    mChanged = false;
    mNewLines.clear();
    initialize();
    return true;
}

bool ConfigFile::write()
{
    return write(mFileName);
}

void ConfigFile::deleteLine(int index)
{
    mFileArray.erase(mFileArray.begin()+index);
    mChanged = true;
}

bool ConfigFile::changed()
{
    if (mChanged) return true;

    for (int i=0; i<size(); i++)
    {
        ConfigLine configLine = mFileArray[i];
        if (configLine.isUpdated())
           return true;
    }
    return false;
}

const ConfigLine * ConfigFile::locateLine(const std::string& keyword, bool synonyms)
{
    outDebug(5, std::cout << "localLine seraching for '" << keyword << "'" << std::endl);
    for (int i=0; i<=getLastSys(); i++)
    {
        outDebug(5, std::cout << "examining  " << (*this)[i]->getLine() << "=='" << (*this)[i]->getToken(0) << "'" <<std::endl);
        if ( !(*this)[i]->isRemark()  && ((*this)[i]->getToken(0) == keyword))
        {
            outDebug(5, std::cout << "token(0):'" << (*this)[i]->getLine() << "'" <<  std::endl);
            return (*this)[i];
        }
    }

    // look in new lines
    for (unsigned int i=0; i<mNewLines.size(); i++)
    {
        ConfigLine * cl = &mNewLines[i];
        outDebug(4, std::cout << "new line test:" << cl->getLine() << std::endl);
        if (cl->getToken(0) == keyword)
        {
            outDebug(4, std::cout << "found" << std::endl);
            return cl;
        }
    }

    if (synonyms)
    {
        outDebug(4, std::cout << "locating in synonyms " <<  keyword << " in synonyms " << std::endl);
        for (int i=0; mSynonyms[i].keyword1.length() > 0; i++)
        {
            if (mSynonyms[i].keyword1.compare(keyword) == 0)
            {
                const ConfigLine * tmp = locateLine(mSynonyms[i].keyword2, false);
                if (tmp != NULL)
                {
                    outDebug(4, std::cout << "found in synonyms" << std::endl);
                    return tmp;
                }
            }
        }
    }

    std::string line(keyword + "\n");
    SystemConfigLine * ret = new SystemConfigLine(line.c_str());
    ret->setNew(true);
    outDebug(4, std::cout << "new line 1:" << ret->getToken(0) << " 2:" << ret->getToken(1) << std::endl);
    mNewLines.push_back(*ret);
    return ret;

}

void ConfigFile::appendNewLines()
{
    outDebug(4, std::cout << "append new lines:" << mNewLines.size() << std::endl);
    // append new system lines
    for (unsigned int i=0; i<mNewLines.size(); i++)
    {
        outDebug(3, std::cout << mNewLines[i].getLine() << std::endl);
        SystemConfigLine * cl = &mNewLines[i];
        addNonDev(cl);
    }
    // append new device lines
    for (unsigned int i=0; i<mNewDeviceLines.size(); i++)
    {
        outDebug(3, std::cout << mNewDeviceLines[i].getLine() << std::endl);
        DeviceConfigLine * cl = &mNewDeviceLines[i];
        addDev(cl);
    }
}

void ConfigFile::setNew(bool isNew)
{
	mNewConfig = isNew;
}

void ConfigFile::openTemplate()
{
	templateCounter = -1;
}

char *ConfigFile::getNextLine(char * buff, int len, FILE * f)
{
	if (f != NULL)
		return fgets(buff, len, f);

	const char * from = configTemplate[++templateCounter];
	if (strcmp("EOF",from) == 0)
		return NULL;

	strncpy(buff,from, len);
	return buff;
}

const char * ConfigFile::configTemplate[50] = {
"#------------------------------------------------------------------------------\n",
"#       CPU Configuration\n",
"#------------------------------------------------------------------------------\n",
"\n",
"   CPUSERIAL 002623              # CPU serial number\n",
"   CPUMODEL  3090                # CPU model number\n",
"   MODEL     EMULATOR            # STSI returned model\n",
"   PLANT     ZZ                  # STSI returned plant\n",
"   MANUFACTURER HRC              # STSI returned manufacturer\n",
"   LPARNAME  HERCULES            # DIAG 204 returned lparname\n",
"   CPUVERID  FD                  # CPU Version Identification\n",
"   MAINSIZE  128                 # Main storage size in megabytes\n",
"\n",
"#------------------------------------------------------------------------------\n",
"#       OS Tailoring\n",
"#------------------------------------------------------------------------------\n",
"\n",
"   LOADPARM  0120....            # IPL parameter\n",
"\n",
"#------------------------------------------------------------------------------\n",
"#       Hercules Service Processor and Hercules Application Window\n",
"#------------------------------------------------------------------------------\n",
"\n",
"   DIAG8CMD  disable echo                # OS may not issue commands via DIAG 8\n",
"\n",
"   HTTPPORT  8081 noauth userid password # HTTP server port\n",
"\n",
"  PANRATE   FAST                        # Panel refresh rate\n",
"\n",
"\n",
"#-------------------------------------------------------------------------------\n",
"#       Advanced Hercules Performance Tailoring\n",
"#\n",
"#       +----------------------------------------------------------------+\n",
"#       | Caution:  Modification of these parameters may adversely       |\n",
"#       |           affect the performance of the host system            |\n",
"#       |           and/or Hercules.                                     |\n",
"#       +----------------------------------------------------------------+\n",
"#-------------------------------------------------------------------------------\n",
"\n",
"   HERCPRIO    0                 # Hercules process runs at Normal priority\n",
"   CPUPRIO    15                 # CPU thread(s) run at Low priority\n",
"   DEVPRIO     8                 # Device thread(s) run at Below Normal priority\n",
"   TODPRIO   -20                 # TOD Clock and timer thread are Time Critical\n",
"\n",
"#------------------------------------------------------------------------------\n",
"#       I/O Devices\n",
"#------------------------------------------------------------------------------\n",
"\n",
"EOF"};
