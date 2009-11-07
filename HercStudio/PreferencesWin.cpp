/*
 *  File:       PreferencesWin.cpp
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id: PreferencesWin.cpp 34 2009-11-07 06:15:58Z jacob $
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

#include "PreferencesWin.h"
#include "Fonts.h"

#include <QFileDialog>

#include <iostream>

PreferencesWin::PreferencesWin( const std::string& currentPath, Preferences *preferences, QWidget *parent)
    : QDialog(parent), mCurrentPath(currentPath), mPreferences(preferences)
{
	ui.setupUi(this);
	connect(ui.okButton, SIGNAL(pressed()), this, SLOT(okPressed()));
	connect(ui.cancelButton, SIGNAL(pressed()), this, SLOT(cancelPressed()));
	connect(ui.hercDirButton, SIGNAL(pressed()), this, SLOT(hercDirPressed()));
	connect(ui.configDirButton, SIGNAL(pressed()), this, SLOT(configDirPressed()));
	connect(ui.logsDirButton, SIGNAL(pressed()), this, SLOT(logsDirPressed()));

	ui.hercDirLineEdit->setText(mPreferences->hercDir().c_str());
	ui.configDirLineEdit->setText(mPreferences->configDir().c_str());
	ui.logsDirLineEdit->setText(mPreferences->logsDir().c_str());

	QStringList& list = Fonts::getInstance().getList();
	ui.fontNameCombo->addItems(list);
	int index = Fonts::getInstance().indexOf(mPreferences->fontName().c_str());
	if (index == -1) index = 0;
    ui.fontNameCombo->setCurrentIndex(index);

    ui.fontSizeSpinBox->setValue(mPreferences->fontSize());

    int variation = mPreferences->fontVariation();
    ui.boldCheckBox->setChecked((variation & Preferences::Bold) != 0);
    ui.italicCheckBox->setChecked((variation & Preferences::Italic) != 0);
    ui.logTimestampCheckbox->setChecked(mPreferences->logTimestamp());
}

PreferencesWin::~PreferencesWin()
{
}

void PreferencesWin::okPressed()
{
	mPreferences->setHercDir(ui.hercDirLineEdit->text().toStdString());
	mPreferences->setConfigDir(ui.configDirLineEdit->text().toStdString());
	mPreferences->setLogsDir(ui.logsDirLineEdit->text().toStdString());
	mPreferences->setFontName(ui.fontNameCombo->currentText().toStdString());
	mPreferences->setFontSize(ui.fontSizeSpinBox->value());
	int variation = 0;
	if (ui.boldCheckBox->isChecked()) variation |= Preferences::Bold;
	if (ui.italicCheckBox->isChecked()) variation |= Preferences::Italic;
	mPreferences->setFontVariation(variation);
	mPreferences->setLogTimestamp(ui.logTimestampCheckbox->isChecked());
	mPreferences->write();
	emit logFontChanged();
	close();
}

void PreferencesWin::cancelPressed()
{
  close();
}

void PreferencesWin::hercDirPressed()
{
  QString s = QFileDialog::getExistingDirectory(this,
        tr("Find Hercules directory"),
        ui.hercDirLineEdit->text(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  if (s.length() > 0)
    ui.hercDirLineEdit->setText(s);
}
void PreferencesWin::configDirPressed()
{
  QString s = QFileDialog::getExistingDirectory(this,
        tr("Hercules configurations directory"),
        ui.configDirLineEdit->text(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  if (s.length() > 0)
    ui.configDirLineEdit->setText(s);
}
void PreferencesWin::logsDirPressed()
{
  QString s = QFileDialog::getExistingDirectory(this,
        tr("Logs directory"),
        ui.logsDirLineEdit->text(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  if (s.length() > 0)
    ui.logsDirLineEdit->setText(s);
}
