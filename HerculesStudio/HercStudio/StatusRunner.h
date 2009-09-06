/*
 *  File:       StatusRunner.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *
 *	This object collects status data from hercules
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

#ifndef STATUSRUNNER_H_
#define STATUSRUNNER_H_
#include "Runner.h"


class StatusRunner : public Runner
{
  Q_OBJECT
public:
  StatusRunner(SynchronizedQueue& statusQueue);
  virtual ~StatusRunner();

  virtual void run();

signals:
    void newData();

private:
};

#endif /* STATUSRUNNER_H_ */
