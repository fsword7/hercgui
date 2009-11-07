/*
 *  File:       DasdLoad.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id: DasdLoad.h 34 2009-11-07 06:15:58Z jacob $
 *
 *	Dasdload utility object
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

#ifndef DASDLOAD_H
#define DASDLOAD_H

#include <QtGui/QDialog>
#include "ui_DasdLoad.h"

class DasdLoad : public QDialog
{
    Q_OBJECT

public:
    DasdLoad(QWidget *parent = 0);
    ~DasdLoad();

private:
    Ui::DasdLoadClass ui;
    int               mPid;
    bool              mEnded;

private slots:
    void exitClicked();
    void runClicked();
    void runnerMaximumChanged(int maximum);
    void runnerValueChanged(int value);
    void runnerError(const QString& errorLine);
    void browseCtlClicked();
    void browseOutClicked();

signals:
    void output(QString line);
};

#endif // DASDLOAD_H
