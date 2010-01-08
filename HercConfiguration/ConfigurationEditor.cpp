/*
 *  File: ConfigurationEditor.cpp
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id: ConfigurationEditor.cpp 34 2009-11-07 06:15:58Z jacob $
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

#include "ConfigurationEditor.h"
#include "HerculesStudio.h"

#include <iostream>
#include <sstream>
#include <iomanip>

#define min(a,b) ((a) < (b) ? (a) : (b))  //util
#define max(a,b) ((a) > (b) ? (a) : (b))  //util

#define TextItem(PARM,parm) \
void ConfigurationEditor::PARM(Ui::ConfigurationClass * configUi, const ConfigLine * configLine, Direction dir) \
{                                                       \
    outDebug(4,std::cout << #PARM << std::endl);        \
    if (dir == toScreen)                                \
    {                                                   \
        std::string parm = configLine->getToken(1);     \
        configUi->parm->setText(parm.c_str());          \
    }                                                   \
    else                                                \
    {                                                   \
        /*TODO: verification routine*/                  \
        if (configUi->parm->isModified())               \
        {                                               \
           const_cast<ConfigLine *>(configLine)->replaceParameters(configUi->parm->text().toStdString()); \
        }                                               \
    }                                                   \
}

#define SpinItem(PARM,_parm, default_) \
void ConfigurationEditor::PARM(Ui::ConfigurationClass * configUi, const ConfigLine * configLine, Direction dir) \
{ \
    outDebug(4,std::cout << #PARM << std::endl);      \
    if (dir == toScreen)                              \
    {                                                 \
        std::string parm = configLine->getToken(1);   \
        if (parm[0] == '+') parm = parm.substr(1);    \
        int num = parseNum(parm);                     \
        if (num == 0) num = default_;				  \
        configUi->_parm->setValue(num);               \
        mSpinMap[#PARM] = configUi->_parm->value();   \
    }                                                 \
    else                                              \
    {                                                 \
        /*TODO: verification routine*/                \
        if (mSpinMap[#PARM] != configUi->_parm->value())\
            const_cast<ConfigLine *>(configLine)->replaceParameters(configUi->_parm->text().toStdString()); \
    } \
}

#define SliderItem(PARM,_parm) \
void ConfigurationEditor::PARM(Ui::ConfigurationClass * configUi, const ConfigLine * configLine, Direction dir) \
{ \
    outDebug(4,std::cout << #PARM << std::endl);        \
    if (dir == toScreen)                                \
    {                                                   \
        std::string parm = configLine->getToken(1);     \
        int num = parseNum(parm);                       \
        configUi->_parm->setValue(num);                 \
        mSpinMap[#PARM] = configUi->_parm->value();     \
    }                                                   \
    else                                                \
    {   if(configUi->_parm->isWindowModified()){};      \
        /*TODO: verification routine*/                  \
        if (mSpinMap[#PARM] != configUi->_parm->value())\
        {                                               \
            std::stringstream s;                        \
            s << configUi->_parm->value();      \
            std::string parm = s.str();                 \
            const_cast<ConfigLine *>(configLine)->replaceParameters(s.str()); \
        }                                               \
    } \
}

#define CheckBoxItem(PARM,parm, CHECKED,  UNCHECKED) \
void ConfigurationEditor::PARM(Ui::ConfigurationClass * configUi, const ConfigLine * configLine, Direction dir) \
{                                                                                   \
    outDebug(4,std::cout << #PARM << std::endl);                                    \
    if (dir == toScreen)                                                            \
    {                                                                               \
        QString parm=configLine->getToken(1).c_str();                               \
        if (parm=="") parm=#UNCHECKED;                                              \
        configUi->parm->setChecked( (parm.toUpper() == #CHECKED )  ? true : false); \
        mCheckBoxMap[#PARM] = configUi->parm->isChecked();                          \
    }                                                                               \
    else                                                                            \
    {                                                                               \
        if ( (configUi->parm->isChecked() != mCheckBoxMap[#PARM]) )                 \
        {                                                                           \
            if (configUi->parm->isChecked())                                   \
                const_cast<ConfigLine *>(configLine)->replaceParameters(#CHECKED);  \
            else                                                                    \
                const_cast<ConfigLine *>(configLine)->replaceParameters(#UNCHECKED);\
        }                                                                           \
    }                                                                               \
}

std::map<std::string,bool>ConfigurationEditor::mCheckBoxMap;
std::map<std::string,int>ConfigurationEditor::mSpinMap;

ConfigurationEditor::ConfigurationEditor()
{
    outDebug(5,std::cout << "ConfigurationEditor::ConfigurationEditor" << std::endl);
}

ConfigurationEditor::~ConfigurationEditor()
{
}

void ConfigurationEditor::ARCHMODE(Ui::ConfigurationClass * configUi, const ConfigLine * configLine, Direction dir)
{
    if (dir == toScreen)
    {
        outDebug(4,std::cout << "ARCHMODE" << std::endl);
        std::string token = configLine->getLowercaseToken(1);
        if (token == "s/370")
            configUi->s370Button->setChecked(true);
        else if (token == "esa/390")
            configUi->ESA390Button->setChecked(true);
        else if ((token == "esame") || (token == "z/arch") || (token == "z/arch"))
            configUi->ZArchButton->setChecked(true);
        int value;
        if (configUi->s370Button->isChecked()) value = 0;
        else if (configUi->ESA390Button->isChecked()) value = 1;
        else value = 2;
        mSpinMap["ARCHMODE"] = value;
    }
    else
    {
        int value;
        if (configUi->s370Button->isChecked()) value = 0;
        else if (configUi->ESA390Button->isChecked()) value = 1;
        else value = 2;
        if (value != mSpinMap["ARCHMODE"])
        {
            std::string parm;
            if (configUi->s370Button->isChecked())
                parm="S/370";
            else if(configUi->ESA390Button->isChecked())
                parm="ESA/390";
            else
                parm="z/Arch";
            const_cast<ConfigLine *>(configLine)->replaceParameters(parm);
        }
    }
}

CheckBoxItem(ASN_AND_LX_REUSE,alrfCheck, ENABLE, DISABLE)

void ConfigurationEditor::AUTO_MOUNT(Ui::ConfigurationClass * configUi, const ConfigLine * configLine, Direction dir)
{
    // TODO: accept "-", but do not generate one
    outDebug(4,std::cout << "AUTO_MOUNT" << std::endl);
    if (dir == toScreen)
    {
        std::string parm=configLine->getToken(1);
        if (parm=="+" || parm == "-")
            parm=configLine->getToken(1);
        configUi->autoMountDir->setText(parm.c_str());
    }
    else if (configUi->autoMountDir->isModified())
    {
        const std::string val = configUi->autoMountDir->text().toStdString();
        const_cast<ConfigLine *>(configLine)->replaceParameters(val);
    }
}

void ConfigurationEditor::AUTO_SCSI_MOUNT(Ui::ConfigurationClass * configUi, const ConfigLine * configLine, Direction dir)
{
    outDebug(4,std::cout << "AUTO_SCSI_MOUNT" << std::endl);
    if (dir == toScreen)
    {
        std::string parm = configLine->getLowercaseToken(1);
        int num = parseNum(parm);
        if (parm.length() == 0)
        	num = 50;
        if (parm == "yes")
        {
            configUi->autoScsiMountCheck->setChecked(true);
            configUi->autoScisiSecSpin->setDisabled(false);
            configUi->autoScisiSecSpin->setValue(50);
        }
        else if (parm.length() != 0  && parm.compare("no") != 0)
        {

            configUi->autoScsiMountCheck->setChecked(true);
            configUi->autoScisiSecSpin->setDisabled(false);
            configUi->autoScisiSecSpin->setValue(num);
        }
        else
        {
            configUi->autoScsiMountCheck->setChecked(false);
            configUi->autoScisiSecSpin->setValue(num);
            configUi->autoScisiSecSpin->setDisabled(true);
        }
        mCheckBoxMap["AUTO_SCSI_MOUNT"] = configUi->autoScsiMountCheck->isChecked();
        mSpinMap["AUTO_SCSI_MOUNT"] = configUi->autoScisiSecSpin->value();
    }
    else if (mCheckBoxMap["AUTO_SCSI_MOUNT"] != configUi->autoScsiMountCheck->isChecked() ||
             mSpinMap["AUTO_SCSI_MOUNT"] != configUi->autoScisiSecSpin->value() )
    {
        std::string parm;
        if (!configUi->autoScsiMountCheck->isChecked())
        {
            parm = "NO";
        }
        else
        {
            int val = configUi->autoScisiSecSpin->value();
            std::stringstream s;
            s << val;
            parm = s.str();
        }
        outDebug(3,std::cout << "parm=" << parm << std::endl);
        const_cast<ConfigLine *>(configLine)->replaceParameters(parm);
    }
}

SpinItem(CNSLPORT,ConsolePortSpin, 3270)
TextItem(CPUMODEL,CpuModel)
SliderItem(CPUPRIO,cpuPrioSlider)
TextItem(CPUSERIAL,cpuSerial)
TextItem(CPUVERID,cpuVerid)
SliderItem(DEVPRIO,devPrioSlider)
SpinItem(DEVTMAX,devTMaxSpin, 0)

void ConfigurationEditor::DIAG8CMD(Ui::ConfigurationClass * configUi, const ConfigLine * configLine, Direction dir)
{
    outDebug(4,std::cout << "DIAG8CMD" << std::endl);
    if (dir == toScreen)
    {
        std::string enableDisable = configLine->getLowercaseToken(1);
        std::string echoNoEcho = configLine->getLowercaseToken(2);
        configUi->diag8CmdCheckBox->setChecked(enableDisable == "enable");
        configUi->diag8EchoCheckBox->setChecked(echoNoEcho.length() > 0 && echoNoEcho == "echo");
        mCheckBoxMap["DIAG8CMD"]=configUi->diag8CmdCheckBox->isChecked();
        mCheckBoxMap["DIAG8ECHO"]=configUi->diag8EchoCheckBox->isChecked();
    }
    else if (mCheckBoxMap["DIAG8CMD"] != configUi->diag8CmdCheckBox->isChecked() ||
             mCheckBoxMap["DIAG8ECHO"]!= configUi->diag8EchoCheckBox->isChecked())
    {
        std::string enable, echo;
        if (configUi->diag8EchoCheckBox->isChecked())
            echo = "ECHO";
        else
            echo = "NOECHO";
        if (configUi->diag8CmdCheckBox->isChecked())
            enable = "ENABLE";
        else
            enable = "DISABLE";

        std::string parm = enable + " " + echo;
        const_cast<ConfigLine *>(configLine)->replaceParameters(parm);
    }
}

TextItem(ENGINES,engines)

void ConfigurationEditor::ECPSVM(Ui::ConfigurationClass * configUi, const ConfigLine * configLine, Direction dir)
{
    outDebug(4,std::cout << "ECPSVM" << std::endl);
    if (dir == toScreen)
    {
        if (configLine->getLowercaseToken(1) == "yes" || configLine->getLowercaseToken(1) == "level")
        {
            configUi->ecpSvmSpin->setEnabled(true);
            configUi->ecpSvm->setChecked(true);
            int level = 20;
            if (configLine->getToken(2) != "")
                level = parseNum(configLine->getToken(2));
            configUi->ecpSvmSpin->setValue(level);
        }
        else
        {
            configUi->ecpSvm->setChecked(false);
            configUi->ecpSvmSpin->setValue(20);
            configUi->ecpSvmSpin->setEnabled(false);
        }
        mCheckBoxMap["ECPSVM"] = configUi->ecpSvm->isChecked();
        mSpinMap["ECPSVM"] = configUi->ecpSvmSpin->value();
    }
    else if (  mCheckBoxMap["ECPSVM"] != configUi->ecpSvm->isChecked() ||
               mSpinMap["ECPSVM"] != configUi->ecpSvmSpin->value() )
    {
        std::string parm;
        if (configUi->ecpSvm->isChecked())
        {
			std::stringstream ss;
			ss << configUi->ecpSvmSpin->value();
			parm = "LEVEL " + ss.str();
        }
        else parm = "NO";
        const_cast<ConfigLine *>(configLine)->replaceParameters(parm);
    }
}

SliderItem(HERCPRIO,hercPrioSlider)

void ConfigurationEditor::HTTPPORT(Ui::ConfigurationClass * configUi, const ConfigLine * configLine, Direction dir) \
{
    outDebug(4,std::cout << "HTTPPORT" << std::endl);
    if (dir == toScreen)
    {
        std::string parm = configLine->getToken(1);
        int num = parseNum(parm);
        if (num !=0)
        {
            configUi->httpPortSpin->setValue(num);
            configUi->httpPortSpin->setEnabled(true);
            configUi->httpPortCheckBox->setChecked(true);
        }
        else
        {
            configUi->httpPortCheckBox->setChecked(false);
            configUi->httpPortSpin->setValue(0);
            configUi->httpPortSpin->setEnabled(false);
        }
        mSpinMap["HTTPPORT"] = configUi->httpPortSpin->value();
    }
    else
    {
        if (mSpinMap["HTTPPORT"] != configUi->httpPortSpin->value())
            const_cast<ConfigLine *>(configLine)->replaceParameters(configUi->httpPortSpin->text().toStdString());
    }
}

void ConfigurationEditor::HTTPROOT(Ui::ConfigurationClass * configUi, const ConfigLine * configLine, Direction dir)
{
    outDebug(4,std::cout << "HTTPROOT" << std::endl);
    if (dir == toScreen)
    {
        std::string parm=configLine->getToken(1);
        if (parm=="+" || parm == "-")
            parm=configLine->getToken(1);
        configUi->httpRootDir->setText(parm.c_str());
    }
    else if (configUi->httpRootDir->isModified())
    {
        const std::string val = configUi->httpRootDir->text().toStdString();
        const_cast<ConfigLine *>(configLine)->replaceParameters(val);
    }
}

SpinItem(IODELAY,ioDelaySpin, 0)
TextItem(LDMOD,ldmod)

void ConfigurationEditor::LEGACYSENSEID(Ui::ConfigurationClass * configUi, const ConfigLine * configLine, Direction dir)
{
    outDebug(4,std::cout << "LEGACYSENSEID" << std::endl);
    if (dir == toScreen)
    {
        std::string parm=configLine->getToken(1);
        configUi->legacySenseIdCheckbox->setChecked( (parm == "ON") || (parm == "ENABLE") ? true : false);
        mCheckBoxMap["LEGACYSENSEID"] = configUi->legacySenseIdCheckbox->isChecked();
    }
    else if (mCheckBoxMap["LEGACYSENSEID"] != configUi->legacySenseIdCheckbox->isChecked())
    {
        if (configUi->legacySenseIdCheckbox->isChecked())
            const_cast<ConfigLine *>(configLine)->replaceParameters("ENABLE");
        else
            const_cast<ConfigLine *>(configLine)->replaceParameters("DISABLE");
    }
}

TextItem(LOADPARM,loadParm)
TextItem(LPARNAME,lparName)
SpinItem(MAINSIZE,mainSizeSpin, 64)
TextItem(MANUFACTURER,manufacturer)
TextItem(MODEL,model)
CheckBoxItem(MOUNTED_TAPE_REINIT,mountedTapeReinitCheckbox,ALLOW,DISALLOW)

void ConfigurationEditor::MODPATH(Ui::ConfigurationClass * configUi, const ConfigLine * configLine, Direction dir)
{
    outDebug(4,std::cout << "MODPATH" << std::endl);
    if (dir == toScreen)
    {
        std::string parm=configLine->getToken(1);
        if (parm=="+" || parm == "-")
            parm=configLine->getToken(1);
        configUi->modPath->setText(parm.c_str());
    }
    else if (configUi->modPath->isModified())
    {
        const std::string val = configUi->modPath->text().toStdString();
        const_cast<ConfigLine *>(configLine)->replaceParameters(val);
    }
}

SpinItem(NUMCPU,numCpuSpin, 1)
SpinItem(NUMVEC,numVecSpin, 0)

void ConfigurationEditor::OSTAILOR(Ui::ConfigurationClass * configUi, const ConfigLine * configLine, Direction dir)
{
    outDebug(4,std::cout << "OSTAILOR" << std::endl);
    if (dir == toScreen)
    {
        QString qParm=configLine->getToken(1).c_str();
        std::string parm = qParm.toUpper().toStdString();
        int initialValue;
        if (parm == "QUIET")
        {
            configUi->quiet->setChecked(true);
            initialValue = 1;
        }
        else if (parm == "VM")
        {
            configUi->vm->setChecked(true);
            initialValue = 2;
        }
        else if (parm == "VSE")
        {
            configUi->vse->setChecked(true);
            initialValue = 3;
        }
        else if (parm == "LINUX")
        {
            configUi->linuxb->setChecked(true);
            initialValue = 4;
        }
        else if (parm == "OS/390")
        {
            configUi->os390->setChecked(true);
            initialValue = 5;

        }
        else if (parm == "Z/OS")
        {
            configUi->zOS->setChecked(true);
            initialValue = 6;
        }
        else
        {
            configUi->none->setChecked(true);
            initialValue = 0;
        }
        mSpinMap["OSTAILOR"] = initialValue;

    }
    else
    {
        int value;
        if (configUi->quiet->isChecked()) value=1;
        else if (configUi->vm->isChecked()) value=2;
        else if (configUi->vse->isChecked()) value=3;
        else if (configUi->linuxb->isChecked()) value=4;
        else if (configUi->os390->isChecked()) value=5;
        else if (configUi->zOS->isChecked()) value=6;
        else value=0;
        if (value != mSpinMap["OSTAILOR"])
        {
            if (configUi->quiet->isChecked())
                const_cast<ConfigLine *>(configLine)->replaceParameters("QUIET");
            else if (configUi->vm->isChecked())
                const_cast<ConfigLine *>(configLine)->replaceParameters("VM");
            else if (configUi->vse->isChecked())
                const_cast<ConfigLine *>(configLine)->replaceParameters("VSE");
            else if (configUi->linuxb->isChecked())
                const_cast<ConfigLine *>(configLine)->replaceParameters("LINUX");
            else if (configUi->os390->isChecked())
                const_cast<ConfigLine *>(configLine)->replaceParameters("OS/390");
            else if (configUi->zOS->isChecked())
                const_cast<ConfigLine *>(configLine)->replaceParameters("z/OS");
            else if (configUi->none->isChecked())
                const_cast<ConfigLine *>(configLine)->replaceParameters("NONE");
        }
    }
}

SpinItem(PANRATE,panRateSpin, 500)
CheckBoxItem(PGMPRDOS,pgmPrdOs,LICENSED,RESTRICTED)
TextItem(PLANT,plant)
CheckBoxItem(SHCMDOPT,shCmdOpt,DISABLE, NODIAG8)
SpinItem(SHRDPORT,shrdPortSpin, 3990)
SpinItem(SYSEPOCH,sysepochSpin, 1900)
SpinItem(TIMERINT,timerIntSpin, 50)
SpinItem(TODDRAG,todDragSpin, 0)
SliderItem(TODPRIO,todPrioSlider)

void ConfigurationEditor::TZOFFSET(Ui::ConfigurationClass * configUi, const ConfigLine * configLine, Direction dir)
{
    hOutDebug(5,"TZOFFSET:");
    if (dir == toScreen)
    {
        std::string parm = configLine->getToken(1);
        if (parm.length() < 4) return;
        outDebug(5,std::cout << "parm=" << parm << std::endl;)
        int hh, mm;
        int sig = 1;
        if (parm[0] == '+') parm = parm.substr(1);
        if (parm[0] == '-')
        {
        	parm = parm.substr(1);
        	sig = -1;
        }
        if (parm.length() == 3)
        {
        	hh = sig*(parm[0]-'0');
        	mm = parseNum(parm.substr(1));
        }
        else
        {
        	hh = sig*parseNum(parm.substr(0,2));
        	mm = parseNum(parm.substr(2,2));
        }
        configUi->tzOffsetHH->setValue(hh);
        configUi->tzOffsetMM->setValue(mm);
        outDebug(5,std::cout << "HH:" << hh << "MM:" << mm<< std::endl;)
        mSpinMap["TZOFFSET"] = parseNum(parm);
    }
    else
    {
    	int iParm = configUi->tzOffsetHH->value()*100 + configUi->tzOffsetMM->value();
        if (mSpinMap["TZOFFSET"] != iParm)
        {
        	QString parm = configUi->tzOffsetHH->text() + configUi->tzOffsetMM->text();
        	outDebug(5,std::cout << "parm:" << parm.toAscii().data() << std::endl;)
            const_cast<ConfigLine *>(configLine)->replaceParameters(parm.toStdString());
        }
    }
}



SpinItem(XPNDSIZE,xpndSizeSpin, 0)
SpinItem(YROFFSET,yrOffsetSpin, 0)

/////////////////////////////////////////////////////////////////////////////////////////////////

bool ConfigurationEditor::validateNum(const std::string& str, int base)
{
	if (base == 16)
	{
		QString qstr = str.c_str();
		if (qstr.toLower().toStdString().find_first_not_of("0123456789abcdef") != std::string::npos)
			return false;
		else
			return true;
	}
	return true;
}
int ConfigurationEditor::parseNum(const std::string& str, int base)
{
    int ret=0;
    int sign = 1;
    const char *c = str.c_str();
    static std::string digits = "0123456789abcdef";
    if (*c == '-')
    {
        sign = -1;
        c++;
    }
    for ( ; *c != '\0' ; c++)
    {
        char pc = *c;
        if (base==10 && !isdigit(pc)) break;
        if ( (pc <= 'Z') && (pc >= 'A') ) pc+=('a'-'A');
        ret *= base;
        //ret += (*c-'0');
        size_t i = digits.find_first_of(pc) ;
        ret += i;
    }
    return ret*sign;
}

std::string ConfigurationEditor::normalizeNum(const std::string& str, int width, int base)
{
    int devno = parseNum(str,base);
    std::stringstream newLine;
    newLine << std::hex << std::setw(width) << std::setfill('0') << std::uppercase
        << devno ;
    return newLine.str();
}
