/*
 *  File: PrinterController.h
 *
 *  Author:     Jacob Dekel
 *  Created on: May 10, 2014
 *
 *  Copyright (c) 2009-2014 Jacob Dekel
 *  $Id$
 *
 *  This object handles the printer queue
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

#ifndef PRINTERCONTROLLER_H
#define PRINTERCONTROLLER_H

#include "SynchronizedQueue.h"
#include "HerculesStudio.h"

#include <QObject>

mk_shared_ptr(PdfPrinter)
mk_shared_ptr(PrintRunner)
mk_shared_constptr(PrinterItem)

class PrinterController : public QObject
{
    Q_OBJECT
public:
    PrinterController(PrinterItemConstPtr printerItem, PdfPrinterPtr printer);
    ~PrinterController();

    void exec();
    void stop();

private:
    PrinterItemConstPtr mPrinterItem;
    PdfPrinterPtr mPrinter;
    PrintRunnerPtr mRunner;
    SynchronizedQueue mQueue;

private slots:
    void lineReceived();

signals:
    void connected();
    void disconnected();
    void waiting();
    void stoppedWaiting();
};

#endif // PRINTERCONTROLLER_H
