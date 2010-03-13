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

	ui.fontNameLog->addItems(list);
	ui.fontNameRegs->addItems(list);
	ui.fontNamePsw->addItems(list);
	ui.fontNameCommand->addItems(list);

	int fontIndex = Fonts::getInstance().indexOf(mPreferences->fontName(Preferences::LogFontObject).c_str());
	if (fontIndex == -1) fontIndex = 0;
    ui.fontNameLog->setCurrentIndex(fontIndex);
    ui.fontSizeLog->setValue(mPreferences->fontSize(Preferences::LogFontObject));
    ui.boldLog->setChecked(mPreferences->fontIsBold(Preferences::LogFontObject));
    ui.italicLog->setChecked(mPreferences->fontIsItalic(Preferences::LogFontObject));

	fontIndex = Fonts::getInstance().indexOf(mPreferences->fontName(Preferences::RegsFontObject).c_str());
	if (fontIndex == -1) fontIndex = 0;
    ui.fontNameRegs->setCurrentIndex(fontIndex);
    ui.fontSizeRegs->setValue(mPreferences->fontSize(Preferences::RegsFontObject));
    ui.boldRegs->setChecked(mPreferences->fontIsBold(Preferences::RegsFontObject));
    ui.italicRegs->setChecked(mPreferences->fontIsItalic(Preferences::RegsFontObject));

	fontIndex = Fonts::getInstance().indexOf(mPreferences->fontName(Preferences::PswFontObject).c_str());
	if (fontIndex == -1) fontIndex = 0;
    ui.fontNamePsw->setCurrentIndex(fontIndex);
    ui.fontSizePsw->setValue(mPreferences->fontSize(Preferences::PswFontObject));
    ui.boldPsw->setChecked(mPreferences->fontIsBold(Preferences::PswFontObject));
    ui.italicPsw->setChecked(mPreferences->fontIsItalic(Preferences::PswFontObject));

	fontIndex = Fonts::getInstance().indexOf(mPreferences->fontName(Preferences::CommandFontObject).c_str());
	if (fontIndex == -1) fontIndex = 0;
    ui.fontNameCommand->setCurrentIndex(fontIndex);
    ui.fontSizeCommand->setValue(mPreferences->fontSize(Preferences::CommandFontObject));
    ui.boldCommand->setChecked(mPreferences->fontIsBold(Preferences::CommandFontObject));
    ui.italicCommand->setChecked(mPreferences->fontIsItalic(Preferences::CommandFontObject));

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


	mPreferences->setFontName(Preferences::LogFontObject, ui.fontNameLog->currentText().toStdString());
	mPreferences->setFontSize(Preferences::LogFontObject, ui.fontSizeLog->value());
	mPreferences->setBold(Preferences::LogFontObject, ui.boldLog->isChecked());
	mPreferences->setItalic(Preferences::LogFontObject, ui.italicLog->isChecked());

	mPreferences->setFontName(Preferences::RegsFontObject, ui.fontNameRegs->currentText().toStdString());
	mPreferences->setFontSize(Preferences::RegsFontObject, ui.fontSizeRegs->value());
	mPreferences->setBold(Preferences::RegsFontObject, ui.boldRegs->isChecked());
	mPreferences->setItalic(Preferences::RegsFontObject, ui.italicRegs->isChecked());

	mPreferences->setFontName(Preferences::PswFontObject, ui.fontNamePsw->currentText().toStdString());
	mPreferences->setFontSize(Preferences::PswFontObject, ui.fontSizePsw->value());
	mPreferences->setBold(Preferences::PswFontObject, ui.boldPsw->isChecked());
	mPreferences->setItalic(Preferences::PswFontObject, ui.italicPsw->isChecked());

	mPreferences->setFontName(Preferences::CommandFontObject, ui.fontNameCommand->currentText().toStdString());
	mPreferences->setFontSize(Preferences::CommandFontObject, ui.fontSizeCommand->value());
	mPreferences->setBold(Preferences::CommandFontObject, ui.boldCommand->isChecked());
	mPreferences->setItalic(Preferences::CommandFontObject, ui.italicCommand->isChecked());

	mPreferences->setLogTimestamp(ui.logTimestampCheckbox->isChecked());
	mPreferences->write();
	emit fontChanged();
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