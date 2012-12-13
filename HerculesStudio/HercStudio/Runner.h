/*
 *  File:       Runner.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009-2013 Jacob Dekel
 *  $Id$
 *
 *	This object is the base class for all "runners" which collect input from hercules
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

#ifndef RUNNER_H_
#define RUNNER_H_

#include "SynchronizedQueue.h"
#include <QThread>
#include <QWidget>

class Runner :  public QThread
{
  Q_OBJECT
public:
  Runner(SynchronizedQueue & queue);
  virtual ~Runner();

  virtual void run()=0;

signals:
  void newData();

protected:
  SynchronizedQueue& mQueue;
  bool mRunning;

};

#endif /* RUNNER_H_ */
