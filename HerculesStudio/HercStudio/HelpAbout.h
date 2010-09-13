/*
 *  File:       HelpAbout.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id$
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

#ifndef HELPABOUT_H
#define HELPABOUT_H

#include <QtGui/QWidget>
#include "ui_helpabout.h"

class HelpAbout : public QDialog
{
    Q_OBJECT

public:
    HelpAbout(QWidget *parent = 0);
    ~HelpAbout();

public slots:
    void okPressed();

private:
    Ui::HelpAboutClass ui;

};

#endif // HELPABOUT_H
