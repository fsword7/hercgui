/*
 *  File:       Dasdcat.cpp
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id: Dasdcat.cpp 34 2009-11-07 06:15:58Z jacob $
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

#include "Dasdcat.h"
#include "HerculesStudio.h"
#include "UtilityExecutor.h"
#include "UtilityRunner.h"
#include "StringTokenizer.h"

#include <QFileDialog>
#include <QMessageBox>

#include <csignal>


Dasdcat::Dasdcat(QWidget *parent)
    : QDialog(parent), mPid(-1)
{
  ui.setupUi(this);
  ui.listRadioButton->setChecked(true);
  ui.memberName->setDisabled(true);

    connect(ui.runButton, SIGNAL(clicked()), this, SLOT(runClicked()));
    connect(ui.exitButton, SIGNAL(clicked()), this, SLOT(exitClicked()));
    connect(ui.browseFileButton, SIGNAL(clicked()), this, SLOT(browseFileClicked()));
    connect(ui.browseSfButton, SIGNAL(clicked()), this, SLOT(browseSfClicked()));
    connect(ui.specificRadioButton, SIGNAL(toggled(bool)), this, SLOT(specificToggeled(bool)));
}

Dasdcat::~Dasdcat()
{

}

void Dasdcat::runClicked()
{
    if (mPid > 0)
    {
        kill(mPid, SIGKILL);
        QMessageBox::warning(this, "dasdcat",
                                            "dasdcat operation was aborted at user's request",
                                            QMessageBox::Ok,
                                            QMessageBox::NoButton);
        mPid=-1;
        ui.runButton->setText("Ok");
        return;
    }
    if (ui.filename->text().isEmpty())
    {
        QMessageBox::warning(this, "dasdcat", "Please specify ckd file to process",
                QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }
    if (ui.dsname->text().isEmpty())
    {
        QMessageBox::warning(this, "dasdcat", "Please specify PDS name",
                QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }
    std::vector<std::string> parameters;

    parameters.push_back("-i");
    parameters.push_back(ui.filename->text().toStdString());

    if (!ui.shadowFile->text().isEmpty())
    {
        std::string sf = "sf=";
        sf += ui.shadowFile->text().toStdString();
        parameters.push_back(sf);
    }
    std::string dsnameSpec = ui.dsname->text().toStdString();
    dsnameSpec += "/";
    if (ui.listRadioButton->isChecked())
    {
        dsnameSpec+= "?";
    }
    else if (ui.specificRadioButton->isChecked())
    {
        if (ui.memberName->text().isEmpty())
        {
            QMessageBox::warning(this, "dasdcat", "Member name must be provided when selecting\n'show specific member content'",
                            QMessageBox::Ok, QMessageBox::NoButton);
            return;
        }
        dsnameSpec += ui.memberName->text().toStdString();
        dsnameSpec += ":c";  // make it readable on log
    }
    else if (ui.allRadioButton)
    {
        dsnameSpec += "*:c";
    }
    parameters.push_back(dsnameSpec);
    parameters.push_back("EXTERNALGUI");
    std::string command = "dasdcat";

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

    fileNo = executor->getPipeIn();
    FILE * fileIn = fdopen(fileNo,"w");
    putc('s', fileIn);
    fclose(fileIn);


    connect(runner, SIGNAL(valueChanged(int)), this, SLOT(runnerValueChanged(int)));
    connect(runner, SIGNAL(maximumChanged(int)), this, SLOT(runnerMaximumChanged(int)));
    connect(runner, SIGNAL(error(QString)), this, SLOT(runnerError(QString)));
    connect(runnerError, SIGNAL(error(QString)), this, SLOT(runnerError(QString)));
    ui.runButton->setText("Stop");

}


void Dasdcat::exitClicked()
{
    deleteLater();
}

void Dasdcat::browseFileClicked()
{
    QString s = QFileDialog::getOpenFileName(this,"Browse for disk to update",ui.filename->text());
    ui.filename->setText(s);
}

void Dasdcat::browseSfClicked()
{
    QString s = QFileDialog::getOpenFileName(this,"Browse for shadow file",ui.shadowFile->text());
    ui.shadowFile->setText(s);
}

void Dasdcat::specificToggeled(bool checked)
{
    ui.memberName->setEnabled(checked);
}

void Dasdcat::runnerMaximumChanged(int maximum)
{
    outDebug(2, std::cout << "dasdisup maximum is " << maximum << std::endl);
}

void Dasdcat::runnerValueChanged(int)
{
        return;
}

void Dasdcat::runnerError(const QString& line)
{
    Tokenizer::handle pos, lastPos;
    std::string word = StringTokenizer::getFirstWord(line.toStdString(), pos, lastPos, " \t\n");
    if (word == "HHCDU024E" )//&& line.indexOf("EXTERNALGUI") != -1)
    {
        ui.runButton->setText("Run");
        mPid=-1;
    }
    else
    {
        emit output(line);
    }
}

