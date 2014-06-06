/*
 *  File: PrintRunner.h
 *
 *  Author:     Jacob Dekel
 *  Created on: May 10, 2014
 *
 *  Copyright (c) 2009-2014 Jacob Dekel
 *  $Id$
 *
 *  This object monitors printer events
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
#ifndef PRINTRUNNER_H
#define PRINTRUNNER_H

#include "Runner.h"
#include "HerculesStudio.h"

#include <QTcpSocket>

mk_shared_constptr(PrinterItem)

class PrinterSocket : public QTcpSocket
{
public:
    PrinterSocket();
    QByteArray readLine();
    qint64 bytesAvailable() const;
private:
    QByteArray returnLine(const char *end);
    char *nextPos();
    char mBuff[2048];
    char *mBuffPos;
};

class PrintRunner : public Runner
{
    Q_OBJECT
public:
    PrintRunner(SynchronizedQueue& queue, PrinterItemConstPtr& printerItem, int maxQueueSize);

    virtual void run();
    void stop();

private:
    SynchronizedQueue& mQueue;
    PrinterItemConstPtr mPrinterItem;
    static const int MAX_LINE = 2048;
    int mMaxQueueSize;
    QSharedPointer<PrinterSocket> mSocket;
    bool mRunning;
    char mBuff[MAX_LINE];
    int  mCurrBufLen;

    void waitForConnected();
    void readFromSocket();


private slots:
    void socketStateChanged(QAbstractSocket::SocketState state);

signals:
    void waiting();
    void stoppedWaiting();
    void connected();
    void disconnected();
};

#endif // PRINTRUNNER_H
