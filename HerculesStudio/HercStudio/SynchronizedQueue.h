/*
 *  File:       SynchronizedQueue.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *
 *	This object implementa queue to which is access is serialized on write
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

#ifndef SYNCHRONIZEDQUEUE_H_
#define SYNCHRONIZEDQUEUE_H_
#include <QMutex>
#include <string>
#include <list>

typedef std::list<std::string> HercQueue;

class SynchronizedQueue
{
public:
  SynchronizedQueue();
  virtual ~SynchronizedQueue();

  bool            empty ( ) const;
  std::string&    front ( ) ;
  void            pop_front ( );
  void            push_back(const std::string& charBuff);

private:
    HercQueue   mQueue;
    QMutex      mMutex;
};

#endif /* SYNCHRONIZEDHERCULESQUEUE_H_ */
