/*
 *  File:       DasdInit.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id: DasdInit.h 34 2009-11-07 06:15:58Z jacob $
 *
 *	Dasdinit utility object
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

#ifndef DASDINIT_H
#define DASDINIT_H

#include "UtilityExecutor.h"

#include <QtGui/QDialog>
#include <QtGui/QWidget>
#include <QThread>
#include <QMutex>
#include <QValidator>
#include "ui_DasdInit.h"

class DasdInit : public QDialog
{
    Q_OBJECT

public:
    DasdInit(QWidget *parent = 0);
    ~DasdInit();

private:
    Ui::dasdinitClass ui;
    bool mEnded;
    int mPid;
    QMutex mMutex;

    void initialize();
    void setSizeValue();
    bool processIsRunning(int pid);
    bool fileExists(const std::string & fileName);

    class VolserValidator : public QValidator
    {
    public:
    	VolserValidator(QObject * parent);
    	virtual State validate(QString & input, int & pos) const;
    };

private slots:
    void rawSlot(int);
    void sizeClicked();
    void exitClicked();
    void runClicked();
    void newDevType();
    void browseClicked();
    void filenameFocus();
    void runnerMaximumChanged(int maximum);
    void runnerValueChanged(int value);
    void runnerError(const QString& errorLine);

signals:
    void output(QString line);
};

#endif // DASDINIT_H
