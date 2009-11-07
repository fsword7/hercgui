/*
 *  File: Configuration.cpp
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id: Configuration.cpp 34 2009-11-07 06:15:58Z jacob $
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

#include "Configuration.h"
#include "SystemConfigLine.h"
#include "DeviceConfigLine.h"

#include <QFileDialog>

#include <iostream>
#include <sstream>

#define genEntry(keyword)  {#keyword, ConfigurationEditor::keyword},
#define synonymEntry(keyword1, keyword2)  {#keyword1, #keyword2}, {#keyword2, #keyword1},

struct ConfigTableEntry Configuration::mConfigTable[] = {
        genEntry(ARCHMODE)
        genEntry(ASN_AND_LX_REUSE)
        genEntry(AUTO_MOUNT)
        genEntry(AUTO_SCSI_MOUNT)
        //CODEPAGE
        genEntry(CNSLPORT)
        genEntry(CPUMODEL)
        //CONKPALV
        genEntry(CPUPRIO)
        genEntry(CPUSERIAL)
        genEntry(CPUVERID)
        //DEFSYM
        genEntry(DEVPRIO)
        genEntry(DEVTMAX)
        genEntry(DIAG8CMD)
        genEntry(ECPSVM)
        genEntry(ENGINES)
        genEntry(HERCPRIO)
        genEntry(HTTPPORT)
        genEntry(HTTPROOT)
        //genEntry(IGNORE)
        //INCLUDE
        genEntry(IODELAY)
        genEntry(LDMOD)
        genEntry(LEGACYSENSEID)
        genEntry(LOADPARM)
        //LOGOPT
        genEntry(LPARNAME)
        genEntry(MAINSIZE)
        genEntry(MANUFACTURER)
        genEntry(MODEL)
        genEntry(MODPATH)
        genEntry(MOUNTED_TAPE_REINIT)
        genEntry(NUMCPU)
        genEntry(NUMVEC)
        genEntry(OSTAILOR)
        genEntry(PANRATE)
        //PANTITLE --
        genEntry(PGMPRDOS)
        genEntry(PLANT)
        genEntry(SHCMDOPT)
        genEntry(SHRDPORT)
        genEntry(SYSEPOCH)
        genEntry(TIMERINT)
        genEntry(TODDRAG)
        genEntry(TODPRIO)
        //TRACEOPT  --
        genEntry(TZOFFSET)
        genEntry(XPNDSIZE)
        genEntry(YROFFSET)

        {"",NULL}
};

Configuration::Configuration(ConfigFile* configFile, bool newConfig, QWidget *parent)
    : QDialog(parent), mParent(parent), mConfigFile(configFile), mNewConfig(newConfig)
{
    ui.setupUi(this);
    mDevWgt = new DevicesWidget(configFile, ui.Configuration->widget(5));
    mDevWgt->move(0,50);
    mDevWgt->show();
    initilize();
    populate(ConfigurationEditor::toScreen);
}

Configuration::~Configuration()
{
}

void Configuration::initilize()
{

    DoubleDigitSpinBox * hh = new DoubleDigitSpinBox(ui.groupBox_4, true);
	hh->setObjectName(QString::fromUtf8("tzOffsetHH"));
	hh->setGeometry(QRect(ui.tzOffsetHH->geometry()));
	hh->setMinimum(ui.tzOffsetHH->minimum());
	hh->setMaximum(ui.tzOffsetHH->maximum());
	if (ui.tzOffsetHH != NULL) delete ui.tzOffsetHH;
	ui.tzOffsetHH = hh;

    DoubleDigitSpinBox * mm = new DoubleDigitSpinBox(ui.groupBox_4, false);
	mm->setObjectName(QString::fromUtf8("tzOffsetMM"));
	mm->setGeometry(QRect(ui.tzOffsetMM->geometry()));
	mm->setMaximum(ui.tzOffsetMM->maximum());
	if (ui.tzOffsetMM != NULL) delete ui.tzOffsetMM;
	ui.tzOffsetMM = mm;

    connect(ui.pushButton_OK, SIGNAL(clicked()), this , SLOT(okPressed()));
    connect(ui.pushButton_Cancel, SIGNAL(clicked()), this , SLOT(cancelPressed()));
    connect(ui.autoMountBrowseButton, SIGNAL(clicked()), this, SLOT(autoMountBrowsePressed()));
    connect(ui.autoScsiMountCheck, SIGNAL(stateChanged(int)), this, SLOT(autoScsiMountChanged()));
    connect(ui.cpuPrioSlider, SIGNAL(valueChanged(int)), this, SLOT(cpuPrioChanged(int)) );
    connect(ui.devPrioSlider, SIGNAL(valueChanged(int)), this, SLOT(devPrioChanged(int)) );
    connect(ui.hercPrioSlider, SIGNAL(valueChanged(int)), this, SLOT(hercPrioChanged(int)) );
    connect(ui.todPrioSlider, SIGNAL(valueChanged(int)), this, SLOT(todPrioChanged(int)) );
    connect(ui.ecpSvm, SIGNAL(stateChanged(int)), this, SLOT(ecpSvmChanged(int)) );
    connect(ui.httpPortCheckBox, SIGNAL(stateChanged(int)), this, SLOT(httpPortChanged(int)));
    connect(ui.httpRootBrowseButton, SIGNAL(clicked()), this, SLOT(httpRootBrowsePressed()));
    connect(ui.modPathButton, SIGNAL(clicked()), this, SLOT(modPathBrowsePressed()));
}

void Configuration::populate(ConfigurationEditor::Direction dir)
{
    if (mConfigFile == NULL || mNewConfig)
    {
        ui.fileNameLabel->setText("(new file)");
        ui.fileNameLabel_2->setText("(new file)");
        ui.fileNameLabel_3->setText("(new file)");
        ui.fileNameLabel_4->setText("(new file)");
        ui.fileNameLabel_5->setText("(new file)");
        ui.fileNameLabel_6->setText("(new file)");
    }
    else
    {
        ui.fileNameLabel->setText(mConfigFile->getFileName().c_str());
        ui.fileNameLabel_2->setText(mConfigFile->getFileName().c_str());
        ui.fileNameLabel_3->setText(mConfigFile->getFileName().c_str());
        ui.fileNameLabel_4->setText(mConfigFile->getFileName().c_str());
        ui.fileNameLabel_5->setText(mConfigFile->getFileName().c_str());
        ui.fileNameLabel_6->setText(mConfigFile->getFileName().c_str());
    }
    for (int i=0; mConfigTable[i].keyword.compare("") != 0 ; i++)
    {
    	outDebug(5,std::cout << "populate:" << mConfigTable[i].keyword << std::endl;)
        const ConfigLine * configLine = mConfigFile->locateLine(mConfigTable[i].keyword);
        mConfigTable[i].populator(&ui, configLine, dir);
    }
}


void Configuration::okPressed()
{
    populate(ConfigurationEditor::fromScreen);
    if (mNewConfig)
    {
        mParent->connect(this,SIGNAL(OKSignal()), mParent, SLOT(saveConfigAs()));
        emit OKSignal();
        disconnect(this,0,0,0);
    }

    deleteLater();
}

void Configuration::cancelPressed()
{
    disconnect(this,0,0,0);
    deleteLater();
}

void Configuration::autoScsiMountChanged()
{
    ui.autoScisiSecSpin->setEnabled(ui.autoScsiMountCheck->isChecked());
}

void Configuration::cpuPrioChanged(int newValue)
{
    ui.cpuPrioNumber->display(newValue);
}

void Configuration::devPrioChanged(int newValue)
{
    ui.devPrioNumber->display(newValue);
}

void Configuration::ecpSvmChanged(int newValue)
{
    if (newValue == Qt::Checked)
    {
        ui.ecpSvmSpin->setEnabled(true);
    }
    else
    {
        ui.ecpSvmSpin->setEnabled(false);
    }
}

void Configuration::httpPortChanged(int newValue)
{
    if (newValue == Qt::Checked)
    {
        ui.httpPortSpin->setEnabled(true);
    }
    else
    {
        ui.httpPortSpin->setValue(0);
        ui.httpPortSpin->setEnabled(false);
    }
}

void Configuration::hercPrioChanged(int newValue)
{
    ui.hercPrioNumber->display(newValue);
}

void Configuration::todPrioChanged(int newValue)
{
    ui.clockPrioNumber->display(newValue);
}

void Configuration::autoMountBrowsePressed()
{
    std::string s = QFileDialog::getExistingDirectory(this,"Browse Auto Mount Directory","").toStdString();
    ui.autoMountDir->setText(s.c_str());

}

void Configuration::httpRootBrowsePressed()
{
    std::string s = QFileDialog::getExistingDirectory(this,"Browse HTTP Root Directory",ui.httpRootDir->text()).toStdString();
    ui.httpRootDir->setText(s.c_str());
}

void Configuration::modPathBrowsePressed()
{
    std::string s = QFileDialog::getExistingDirectory(this,"Browse Modules Path Directory",ui.modPathButton->text()).toStdString();
    ui.modPath->setText(s.c_str());

}

Configuration::DoubleDigitSpinBox::DoubleDigitSpinBox(QWidget * parent, bool appendPlus):
	QSpinBox(parent)
{
	mAppendPlus = appendPlus;
}

int Configuration::DoubleDigitSpinBox::valueFromText(const QString& text) const
{
	return QSpinBox::valueFromText(text);
}

QString Configuration::DoubleDigitSpinBox::textFromValue(int value) const
{
	std::stringstream ss;
	int sig=1;
	int pValue=value;
	if (value < 0)
	{
		sig = -1;
		pValue = -value;
	}
	ss << pValue;

	QString ret = ss.str().c_str();
	if (pValue < 10) ret = "0" + ret;
	if (sig == -1) ret = "-" + ret;
	if (mAppendPlus && sig != -1) ret = "+" + ret;
	outDebug(5,std::cout << "textFromValue:" << value << "='" << ret.toStdString() << std::endl;)
	return ret;
}

Configuration::HexSpinBox::HexSpinBox(QWidget * parent):
	QSpinBox(parent)
{
	this->setMaximum(0xffff);
}

int Configuration::HexSpinBox::valueFromText(const QString& text) const
{
	hOutDebug(0,"ValueFromText:" << text.toStdString() << ConfigurationEditor::parseNum(text.toStdString(),16) );
	return ConfigurationEditor::parseNum(text.toStdString(),16);
}

QString Configuration::HexSpinBox::textFromValue(int value) const
{
	std::stringstream ss;
	ss << std::hex << value;
	char formatted[5];
	if (value <= 0xffff)
		sprintf(formatted,"%4.4X",(value));
	else
		strcpy(formatted,"0000");
	QString ret(formatted);
	outDebug(0,std::cout << "textFromValue:" << value << "='" << ret.toStdString() << std::endl;)
	return ret;
}

QValidator::State Configuration::HexSpinBox::validate (QString & input, int &) const
{
	if (input.length() > 4 ||
		input.toStdString().find_first_not_of("0123456789ABCDEFabcdef") != std::string::npos)
		return QValidator::Invalid;
	else
		return QValidator::Acceptable;
}

