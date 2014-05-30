#include "PrinterController.h"
#include "PrintRunner.h"
#include "PagePrinter.h"
#include "HerculesStudio.h"

/*
 * PrinterController ---> runner -+-> PrinterSocket.connect
 *     V   A                V
 *     |   |                |
 *     |   +---newData----- +
 *     |
 *     +-----------> Printer.print
 */

PrinterController::PrinterController(PrinterItemConstPtr printerItem, PdfPrinterPtr printer) :
    mPrinterItem(printerItem), mPrinter(printer), mRunner(NULL)
{
}

PrinterController::~PrinterController()
{

}

void PrinterController::exec()
{
    if (mRunner.isNull())
    {
        mRunner = PrintRunnerPtr(new PrintRunner(mQueue, mPrinterItem, 132*80*2));
        connect(mRunner.data(), SIGNAL(newData()), this, SLOT(lineReceived()));
        connect(mRunner.data(), SIGNAL(connected()), this, SIGNAL(connected()));
        connect(mRunner.data(), SIGNAL(disconnected()), this, SIGNAL(disconnected()));
        connect(mRunner.data(), SIGNAL(waiting()), this, SIGNAL(waiting()));
        connect(mRunner.data(), SIGNAL(stoppedWaiting()), this, SIGNAL(stoppedWaiting()));
    }
    mRunner->start();
    hOutDebug(0,"Print runner started");
}

void PrinterController::stop()
{
    if (!mRunner.isNull()) {
        if (mRunner->isRunning())
        {
            mRunner->stop();
        }
        for (int i=0; i<10 && !mRunner->isFinished(); i++)
            QThread::msleep(50);
        mPrinter->close();
    }
}


void PrinterController::lineReceived()
{
    while(!mQueue.empty())
    {
        mPrinter->print(mQueue.front());
        mQueue.pop_front();
    }
}
