/*
 *  File:       LogRunner.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id: LogRunner.h 34 2009-11-07 06:15:58Z jacob $
 *
 *	This object collects data from the hercules log into the queue
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

#ifndef LOGRUNNER_H_
#define LOGRUNNER_H_

#include "Runner.h"
#include "SynchronizedQueue.h"

#include <QThread>
#include <QWidget>

class LogRunner :  public Runner
{
  Q_OBJECT
public:
  LogRunner(SynchronizedQueue& logQueue);
  virtual ~LogRunner();

  virtual void run();

signals:
    void newData();

private:
  std::string mLogFifo;
  bool mRunning;
};

#endif /* LOGRUNNER_H_ */
