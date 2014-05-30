/*
 *  File:       SynchronizedQueue.cpp
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009-2013 Jacob Dekel
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

#include "SynchronizedQueue.h"
#include <QMutexLocker>

SynchronizedQueue::SynchronizedQueue() : mQueue()
{

}

SynchronizedQueue::~SynchronizedQueue()
{

}

bool SynchronizedQueue::empty() const
{
    return mQueue.empty();
}

QByteArray& SynchronizedQueue::front()
{
    return mQueue.front();
}

void SynchronizedQueue::push_back(const QByteArray &str)
{
    QMutexLocker lock(&mMutex);
    mQueue.push_back(str);
}

void SynchronizedQueue::push_back(QByteArray &&str)
{
    QMutexLocker lock(&mMutex);
    mQueue.push_back(str);
}

void SynchronizedQueue::pop_front()
{
    QMutexLocker lock(&mMutex);
    mQueue.pop_front();
}

int SynchronizedQueue::size()
{
	return mQueue.size();
}

void SynchronizedQueue::clear()
{
	mQueue.clear();
}
