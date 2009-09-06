/*
 *  File:       DasdIsup.cpp
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
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

#include "DasdIsup.h"
#include "UtilityExecutor.h"
#include "UtilityRunner.h"
#include "StringTokenizer.h"


#include <QFileDialog>
#include <QMessageBox>

#include <signal.h>

DasdIsup::DasdIsup(QWidget *parent)
    : QDialog(parent), mPid(-1)
{
  ui.setupUi(this);
  ui.progressBar->setVisible(false);

    connect(ui.runButton, SIGNAL(clicked()), this, SLOT(runClicked()));
    connect(ui.exitButton, SIGNAL(clicked()), this, SLOT(exitClicked()));
    connect(ui.browseFileButton, SIGNAL(clicked()), this, SLOT(browseFileClicked()));
    connect(ui.browseSfButton, SIGNAL(clicked()), this, SLOT(browseSfClicked()));
}

DasdIsup::~DasdIsup()
{

}

void DasdIsup::exitClicked()
{
    deleteLater();
}

void DasdIsup::runClicked()
{
    if (mPid > 0)
    {
        kill(mPid, SIGKILL);
        QMessageBox::warning(this, "dasdisup",
                                            "dasdisup operation was aborted at user's request",
                                            QMessageBox::Ok,
                                            QMessageBox::NoButton);
        mPid=-1;
        ui.runButton->setText("Ok");
        return;
    }
    if (ui.filename->text().isEmpty())
    {
        QMessageBox::warning(this, "dasdisup", "Please specify ckd file to process",
                QMessageBox::Ok, QMessageBox::NoButton);
    }
    std::vector<std::string> parameters;

    parameters.push_back(ui.filename->text().toStdString());

    if (!ui.shadowFile->text().isEmpty())
    {
        parameters.push_back(ui.shadowFile->text().toStdString());
    }
    parameters.push_back("EXTERNALGUI");
    std::string command = "dasdisup";

    ui.progressBar->setVisible(true);
    UtilityExecutor * executor = new UtilityExecutor();
    std::string path = "";
    mPid = executor->run(command, path, parameters);
    int fileNo = executor->getPipeOut();
    FILE * file = fdopen(fileNo,"r");
    UtilityRunner * runner = new UtilityRunner(file);
    runner->start();
    fileNo = executor->getPipeError();
    FILE * fileError = fdopen(fileNo,"r");
    UtilityRunner * runnerError = new UtilityRunner(fileError);
    runnerError->start();

    connect(runner, SIGNAL(valueChanged(int)), this, SLOT(runnerValueChanged(int)));
    connect(runner, SIGNAL(maximumChanged(int)), this, SLOT(runnerMaximumChanged(int)));
    connect(runner, SIGNAL(error(QString)), this, SLOT(runnerError(QString)));
    connect(runnerError, SIGNAL(error(QString)), this, SLOT(runnerError(QString)));
    ui.runButton->setText("Stop");

    fileNo = executor->getPipeIn();
    FILE * fileIn = fdopen(fileNo,"w");
    putc('s', fileIn);
    fclose(fileIn);
}

void DasdIsup::browseFileClicked()
{
    QString s = QFileDialog::getOpenFileName(this,"Browse for disk to update",ui.filename->text());
    ui.filename->setText(s);
}

void DasdIsup::browseSfClicked()
{
    QString s = QFileDialog::getOpenFileName(this,"Browse for shadow file",ui.shadowFile->text());
    ui.shadowFile->setText(s);
}

void DasdIsup::runnerMaximumChanged(int maximum)
{
    ui.progressBar->setMaximum(maximum);
    outDebug(2, std::cout << "dasdisup maximum is " << maximum << std::endl);
}

void DasdIsup::runnerValueChanged(int value)
{
    if (value > 0)
        ui.progressBar->setValue(value);
    else if (value < 0)
    {
        ui.progressBar->setValue(ui.progressBar->maximum());
        mPid = -1;
        QMessageBox::information(this, "dasdisup", "Disk update successfully completed!",
                QMessageBox::Ok,
                QMessageBox::NoButton);
        ui.runButton->setText("Run");
        return;
    }
}

void DasdIsup::runnerError(const QString& line)
{
    Tokenizer::handle pos, lastPos;
    std::string word = StringTokenizer::getFirstWord(line.toStdString(), pos, lastPos, " \t\n");
    outDebug(4, std::cout << "runnerError " << line.toStdString() << std::endl);
    ui.runButton->setText("Run");
    mPid = -1;
    emit output(line);
}

