/*
 *  File:       HerculesExecutor.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id: HerculesExecutor.h 34 2009-11-07 06:15:58Z jacob $
 *
 *	This object runs hercules as a child process
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

#ifndef HERCULESEXECUTOR_H_
#define HERCULESEXECUTOR_H_
#include "MainWindow.h"
#include <string>

class HerculesExecutor
{
public:
  HerculesExecutor(QMainWindow& mainWindow);
  virtual ~HerculesExecutor();

  int run(std::string& configName, std::string& herculesPath);

private:
  QMainWindow & mMainWindow;
};

#endif /* HERCULESEXECUTOR_H_ */
