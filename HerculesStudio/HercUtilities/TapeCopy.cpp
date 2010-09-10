/*
 *  File:       TapeCopy.cpp
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 31, 2010
 *
 *  Copyright (c) 2009-2010 Jacob Dekel
 *  $Id: DasdIsup.h 34 2009-11-07 06:15:58Z jacob $
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
#include "TapeCopy.h"
#include "Preferences.h"
#include "UtilityExecutor.h"
#include "UtilityRunner.h"
#include "StringTokenizer.h"

#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTimer>

#include <csignal>


TapeCopy::TapeCopy(QWidget *parent)
    : GenericUtility("tapecopy",parent)
{
    ui.setupUi(this);
    mUpperGeometry = ui.FileBox->geometry();
    mLowerGeometry = ui.ScsiBox->geometry();
    connect(ui.scsiToFileRadio, SIGNAL(clicked()), this, SLOT(scsiToTape()));
    connect(ui.fileToScsiRadio, SIGNAL(clicked()), this, SLOT(tapeToScsi()));
    connect(ui.runButton, SIGNAL(clicked()), this, SLOT(runClicked()));
    connect(ui.cancelButton, SIGNAL(clicked()), this, SLOT(cancelClicked()));
    connect(ui.browseFileButton, SIGNAL(clicked()), this, SLOT(browseFileClicked()));
}

TapeCopy::~TapeCopy()
{

}

void TapeCopy::runClicked()
{
    if (!runOrStopClicked())
    {
        ui.runButton->setText("Run");
        return;
    }
    if (ui.filename->text().isEmpty())
    {
        QMessageBox::warning(this, "tapecopy", "Please specify tape file to process",
                QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }
    if (!QFile::exists(ui.filename->text()))
    {
        QMessageBox::warning(this, "tapecopy", "The specified file does not exist",
                QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }

    std::vector<std::string> parameters;
    parameters.push_back(ui.filename->text().toStdString());
    std::string scsi="/dev/st";
    scsi+=ui.spinBox->text().toStdString();
    parameters.push_back(scsi.c_str());
    std::string command = "tapecopy";

    execute(command, Preferences::getInstance().hercDir(), parameters);

    ui.runButton->setText("Stop");
}

void TapeCopy::browseFileClicked()
{
    QString dir = ui.filename->text();
    if (dir.isEmpty())
        dir = Preferences::getInstance().configDir().c_str();
    QString s = QFileDialog::getOpenFileName(this,"Browse for tape to map",dir);
    ui.filename->setText(s);
}

void TapeCopy::cancelClicked()
{
    deleteLater();
}

void TapeCopy::tapeToScsi()
{
    ui.FileBox->setGeometry(mUpperGeometry);
    ui.ScsiBox->setGeometry(mLowerGeometry);
    ui.FileBox->setTitle("Input Tape");
    ui.ScsiBox->setTitle("Output Tape");
}

void TapeCopy::scsiToTape()
{
    ui.FileBox->setGeometry(mLowerGeometry);
    ui.ScsiBox->setGeometry(mUpperGeometry);
    ui.FileBox->setTitle("Output Tape");
    ui.ScsiBox->setTitle("Input Tape");
}

void TapeCopy::finishedSlot()
{

}
