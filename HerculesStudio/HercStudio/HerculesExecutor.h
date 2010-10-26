/*
 *  File:       HerculesExecutor.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id$
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
#include <QMainWindow>
#include <QProcess>
#include <string>

class HerculesExecutor
{
public:
  HerculesExecutor(QMainWindow& mainWindow, int pid=0);
  virtual ~HerculesExecutor();

  int run(std::string configName, std::string herculesPath);
  void issueCommand(const char * command);
  void issueFormattedCommand(const char *format, const char * arg1);
  void issueFormattedCommand(const char *format, const int arg1);
  void issueFormattedCommand(const char *format, const char * arg1, const char *arg2);
  QByteArray getLine();
  bool getLine(char * buff, int max);
  QByteArray getStatusLine();
  bool getStatusLine(char * buff, int max);
  QProcess * getQProcess(){ return mProcess; };
  int state() {return mProcess->state();};

private:
  QMainWindow & mMainWindow;
  QProcess    *mProcess;
  int	  	  mPid;
};

#endif /* HERCULESEXECUTOR_H_ */
