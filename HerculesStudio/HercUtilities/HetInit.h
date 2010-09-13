/*
 *  File:       HetInit.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Sep 3, 2010
 *
 *  Copyright (c) 2009-2010 Jacob Dekel
 *  $Id$
 *
 *  Hetinit utility object
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

#ifndef HETINIT_H
#define HETINIT_H

#include "GenericUtility.h"

#include <QtGui/QDialog>
#include "ui_HetInit.h"

class VolserSpinBox : public QAbstractSpinBox
{
    Q_OBJECT
public:
    VolserSpinBox(QWidget * parent = 0);
    virtual ~VolserSpinBox();

    virtual QFlags<QAbstractSpinBox::StepEnabledFlag> stepEnabled () const;
    virtual QString text() const;
    virtual void setText(QString text);

protected:
    QFlags<QAbstractSpinBox::StepEnabledFlag> mStepFlag;
    virtual void stepBy (int steps);

protected slots:
    bool verify();

};

class HetInit : public GenericUtility
{
    Q_OBJECT

public:
    HetInit(QWidget *parent = 0);
    ~HetInit();

private:
    Ui::HetInitClass ui;
    QAbstractSpinBox *mVolser;

    virtual void finishedSlot();

private slots:
    void exitClicked();
    void runClicked();
    void browseFileClicked();
    void browseDirClicked();
};

#endif // HETINIT_H
