/*
 *  File:       Dasdconv.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *
 *	Dasdconv utility object
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

#ifndef DASDCONV_H
#define DASDCONV_H

#include <QtGui/QDialog>
#include "ui_Dasdconv.h"

class Dasdconv : public QDialog
{
    Q_OBJECT

public:
    Dasdconv(QWidget *parent = 0);
    virtual ~Dasdconv();

signals:
    void output(QString line);

private slots:
    void runClicked();
    void exitClicked();
    void browseFileClicked();
    void browseOutClicked();
    void runnerMaximumChanged(int maximum);
    void runnerValueChanged(int value);
    void runnerError(const QString& errorLine);

private:
    Ui::DasdconvClass ui;

    int     mPid;
};

#endif // DASDCONV_H
