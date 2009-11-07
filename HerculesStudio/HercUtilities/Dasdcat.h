/*
 *  File:       Dasdcat.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id: Dasdcat.h 34 2009-11-07 06:15:58Z jacob $
 *
 *	Dasdcat utility object
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

#ifndef DASDCAT_H
#define DASDCAT_H

#include <QtGui/QDialog>
#include "ui_Dasdcat.h"

class Dasdcat : public QDialog
{
    Q_OBJECT

public:
    Dasdcat(QWidget *parent = 0);
    ~Dasdcat();

private:
    Ui::DasdcatClass ui;

    int     mPid;

private slots:
    void exitClicked();
    void runClicked();
    void browseFileClicked();
    void browseSfClicked();
    void specificToggeled(bool);
    void runnerMaximumChanged(int maximum);
    void runnerValueChanged(int value);
    void runnerError(const QString& errorLine);

signals:
    void output(QString line);
};

#endif // DASDCAT_H
