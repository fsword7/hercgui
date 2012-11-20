/*
 *  File:       main.cpp
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
 */

#include "MainWindow.h"
#include "MainPanel.h"
#include "Arguments.h"


#include <QtGui>
#include <QApplication>

int main(int argc, char *argv[])
{
	Arguments::getInstance().parse(argc, argv);
	if (Arguments::getInstance().helpRequested())
	{
		Arguments::getInstance().printUsage();
		return 1;
	}

    QApplication a(argc, argv);
    a.setAttribute(Qt::AA_DontShowIconsInMenus, false);

    MainWindow w;

    w.show();
    return a.exec();
}
