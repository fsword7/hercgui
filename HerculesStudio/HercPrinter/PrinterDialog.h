/*
 *  File: PrinterDialog.h
 *
 *  Author:     Jacob Dekel
 *  Created on: May 10, 2014
 *
 *  Copyright (c) 2009-2013 Jacob Dekel
 *  $Id$
 *
 *  This object handles the Hercules Studio printer dialog
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
#ifndef PRINTERDIALOG_H
#define PRINTERDIALOG_H

#include "IpValidator.h"
#include "HerculesStudio.h"

#include <QDialog>
#include <QSystemTrayIcon>

class mMovie;
class QSystemTrayIcon;

namespace Ui {
class PrinterDialog;
}

mk_shared_ptr(PrinterController)
mk_shared_ptr(PdfPrinter)
mk_shared_ptr(PrinterItem)


class PrinterDialog : public IpValidator
{
    Q_OBJECT

public:
    explicit PrinterDialog(QWidget *parent = 0, const QString& item=QString());
    ~PrinterDialog();

    void stop();
    bool isConnected();

private:
    void populate(QString printerName);
    bool save(bool quiet);
    void closeEvent(QCloseEvent * event);
    void systrayHint();
    void setProtected(bool protect);
    bool testDirectory(QString dirName);

    Ui::PrinterDialog      *ui;
    QSystemTrayIcon        * mSystemTrayIcon;
    PrinterItemPtr         mPrinterItem;
    PrinterControllerPtr   mController;
    PdfPrinterPtr          mPrinter;
    bool                   mListening;
    bool                   mSystrayHintEjected;
    QSharedPointer<QMovie> mMovie;


private slots:
    void dirClicked();
    void deleteClicked();
    void adjustPrinter(QString name);
    void printerNameChanged(QString name);
    void cancelClicked();
    void connectClicked();
    void connected();
    void waiting();
    void stoppedWaiting();
    void hideRestore();
    void systrayClick(QSystemTrayIcon::ActivationReason);
};

#endif // PRINTERDIALOG_H
