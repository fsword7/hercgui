/*
 *  File:       PreferencesWin.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id$
 *
 *	This object presents/updates the user preferences
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

#ifndef PREFERENCESWIN_H
#define PREFERENCESWIN_H

#include <QtGui/QDialog>
#include "ui_PreferencesWin.h"
#include "Preferences.h"

class PreferencesWin : public QDialog
{
    Q_OBJECT

public:
    PreferencesWin(const std::string& currentPath, Preferences * preferences, QWidget *parent = 0);
    ~PreferencesWin();

    std::string getHercDir();

private:
    Ui::PreferencesWinClass ui;

    std::string mCurrentPath;
    Preferences * mPreferences;


public slots:
	void okPressed();
	void cancelPressed();
	void hercDirPressed();
	void configDirPressed();
	void logsDirPressed();

signals:
	void preferencesChanged();
};

#endif // PREFERENCESWIN_H
