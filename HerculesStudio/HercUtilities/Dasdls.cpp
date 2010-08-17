/*
 *  File:       Dasdls.cpp
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id: Dasdls.cpp 34 2009-11-07 06:15:58Z jacob $
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

#include "Dasdls.h"
#include "UtilityExecutor.h"
#include "UtilityRunner.h"
#include "StringTokenizer.h"


#include <QFileDialog>
#include <QMessageBox>

#include <csignal>

Dasdls::Dasdls(QWidget *parent)
    : QDialog(parent), mPid(-1)
{
    ui.setupUi(this);

    connect(ui.runButton, SIGNAL(clicked()), this, SLOT(runClicked()));
    connect(ui.exitButton, SIGNAL(clicked()), this, SLOT(exitClicked()));
    connect(ui.browseFileButton, SIGNAL(clicked()), this, SLOT(browseFileClicked()));
    connect(ui.browseSfButton, SIGNAL(clicked()), this, SLOT(browseSfClicked()));
}

Dasdls::~Dasdls()
{

}

void Dasdls::exitClicked()
{
    deleteLater();
}

void Dasdls::runClicked()
{
    if (mPid > 0)
    {
        kill(mPid, SIGKILL);
        QMessageBox::warning(this, "dasdls",
                                            "dasdls operation was aborted at user's request",
                                            QMessageBox::Ok,
                                            QMessageBox::NoButton);
        mPid=-1;
        ui.runButton->setText("Run");
        return;
    }
    if (ui.filename->text().isEmpty())
    {
        QMessageBox::warning(this, "dasdls", "Please specify ckd file to process",
                QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }
    std::vector<std::string> parameters;

    parameters.push_back(ui.filename->text().toStdString());

    if (!ui.shadowFile->text().isEmpty())
    {
        parameters.push_back("sf=" + ui.shadowFile->text().toStdString());
    }
    parameters.push_back("EXTERNALGUI");
    std::string command = "dasdls";

    UtilityExecutor * executor = new UtilityExecutor();
    std::string path = "";
    mLineCount = 0;
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

void Dasdls::browseFileClicked()
{
    QString s = QFileDialog::getOpenFileName(this,"Browse for disk to list",ui.filename->text());
    ui.filename->setText(s);
}

void Dasdls::browseSfClicked()
{
    QString s = QFileDialog::getOpenFileName(this,"Browse for shadow file",ui.shadowFile->text());
    ui.shadowFile->setText(s);
}

void Dasdls::runnerMaximumChanged(int maximum)
{
    outDebug(3, std::cout << "dasdls maximum is " << maximum << std::endl);
}

void Dasdls::runnerValueChanged(int value)
{
    if (value < 0)
    {
    	if (mPid > 0 && mLineCount > 4)
    	{
			mPid = -1;
			QMessageBox::information(this, "dasdls", "Disk listing successfully completed!",
					QMessageBox::Ok,
					QMessageBox::NoButton);
    	}
    	else
    	{
			QMessageBox::warning(this, "dasdls", "Disk listing failed!",
					QMessageBox::Ok,
					QMessageBox::NoButton);

    	}
        ui.runButton->setText("Run");
        return;
    }
}

void Dasdls::runnerError(const QString& line)
{
    Tokenizer::handle pos, lastPos;
    std::string word = StringTokenizer::getFirstWord(line.toStdString(), pos, lastPos, " \t\n");
    if (word == "HHCDU009E" && line.toStdString().find("EXTERNALGUI")!= std::string::npos)
        return;
    if (word == "HHCDU009E" || word == "HHCDU011E")
    {
    	mPid = -1;
    	ui.runButton->setText("Run");
    }
    emit output(line);
    mLineCount++;
}

