/*
 *  File:       IplConfig.h
 *
 *  Author:     Jacob Dekel
 *  Created on: June 22, 2010
 *
 *  Copyright (c) 2009-2013 Jacob Dekel
 *  $Id$
 *
 *	Main window object
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
#ifndef IPLCONFIG_H
#define IPLCONFIG_H

#include <QtWidgets/QDialog>
#include "ui_IplConfig.h"

class IplConfig : public QDialog
{
    Q_OBJECT

public:
    IplConfig(QString &devno, QString& loadParm, QWidget *parent = 0);
    virtual ~IplConfig();

protected:
    QString textFromValue(int value) const; //TODO was duplicated. move to util object
    int valueFromText(const QString& text) const; //TODO was duplicated. move to util object

private:
    Ui::IplConfigClass ui;
    QString &mDevno;
    QString &mLoadParm;

private slots:
    void okPressed();
    void cancelPressed();

signals:
	void doIpl(QString devNo, QString loadParm);
};

#endif // IPLCONFIG_H
