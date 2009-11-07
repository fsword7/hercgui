/*
 *  File: Configuration.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id: Configuration.h 34 2009-11-07 06:15:58Z jacob $
 *
 *	This object manages the Hercules configuration
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

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "ConfigFile.h"
#include "HerculesStudio.h"
#include "ConfigurationEditor.h"
#include "DevicesWidget.h"

#include "ui_Configuration.h"

#include <QtGui/QDialog>

struct ConfigTableEntry
{
    std::string keyword;
    void (*populator)(Ui::ConfigurationClass *, const ConfigLine *, ConfigurationEditor::Direction dir);
};

struct SynonymTableEntry
{
    std::string keyword1;
    std::string keyword2;
};

class Configuration : public QDialog
{
    Q_OBJECT

public:
    Configuration(ConfigFile* confignFile, bool newConfig = false, QWidget *parent = 0);
    virtual ~Configuration();

    class DoubleDigitSpinBox : public QSpinBox
    {
    public:
    	DoubleDigitSpinBox(QWidget * parent = 0, bool appendPlus=false);
    protected:
    	virtual int valueFromText(const QString& text) const;
    	virtual QString textFromValue(int value) const;
    private:
    	bool mAppendPlus;
    };

    class HexSpinBox : public QSpinBox
    {
    public:
    	HexSpinBox(QWidget * parent = 0);
    	virtual QString textFromValue(int value) const;
    protected:
    	virtual int valueFromText(const QString& text) const;
    	QValidator::State validate(QString & input, int & pos) const;
    };

private:
    Ui::ConfigurationClass ui;
    DevicesWidget * mDevWgt;
    QWidget   * mParent;
    ConfigFile* mConfigFile;
    bool        mNewConfig;
    static struct ConfigTableEntry mConfigTable[];

    void initilize();
    void populate(ConfigurationEditor::Direction);

signals:
     void OKSignal();

 private slots:
     void okPressed();
     void cancelPressed();
     void autoMountBrowsePressed();
     void autoScsiMountChanged();
     void cpuPrioChanged(int);
     void devPrioChanged(int);
     void hercPrioChanged(int);
     void todPrioChanged(int);
     void ecpSvmChanged(int);
     void httpPortChanged(int);
     void httpRootBrowsePressed();
     void modPathBrowsePressed();
};

#endif // CONFIGURATION_H
