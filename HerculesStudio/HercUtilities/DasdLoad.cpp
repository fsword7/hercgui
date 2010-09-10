/*
 *  File:       DasdLoad.cpp
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id: DasdLoad.cpp 34 2009-11-07 06:15:58Z jacob $
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

#include "DasdLoad.h"
#include "UtilityExecutor.h"
#include "UtilityRunner.h"
#include "StringTokenizer.h"


#include <QMessageBox>
#include <QFileDialog>

#include <csignal>

DasdLoad::DasdLoad(QWidget *parent)
    : QDialog(parent)
{
  ui.setupUi(this);

    connect(ui.runButton, SIGNAL(clicked()), this, SLOT(runClicked()));
    connect(ui.exitButton, SIGNAL(clicked()), this, SLOT(exitClicked()));
    connect(ui.browseCtlButton, SIGNAL(clicked()), this, SLOT(browseCtlClicked()));
    connect(ui.browseOutButton, SIGNAL(clicked()), this, SLOT(browseOutClicked()));
}

DasdLoad::~DasdLoad()
{

}

void DasdLoad::exitClicked()
{
    deleteLater();
}

void DasdLoad::runClicked()
{
    if (mExecutor != NULL)
    {
    	mExecutor->terminate();
        QMessageBox::warning(this, "dasdinit",
                                            "dasdinit operation was aborted at user's request",
                                            QMessageBox::Ok,
                                            QMessageBox::NoButton);
        ui.runButton->setText("Ok");
        return;
    }
    mEnded = false;
    std::vector<std::string> parameters;

    switch (ui.compressionComboBox->currentIndex())
    {
        case 1:
            parameters.push_back("-z");
            break;
        case 2:
            parameters.push_back("-bz2");
            break;
        case 3:
            parameters.push_back("-0");
            break;
        default:
            break;
    }
    if (ui.lfsCheckBox->isChecked())
        parameters.push_back("-lfs");
    if (ui.altCheckBox->isChecked())
        parameters.push_back("-a");
    parameters.push_back(ui.control->text().toStdString());
    std::string fullPath;
    if (ui.path->text().length() > 0)
    {
        fullPath = ui.path->text().toStdString();
        fullPath += "/";
    }
    fullPath += ui.filename->text().toStdString();
    parameters.push_back(fullPath);
    parameters.push_back("EXTERNALGUI");
    std::string command = "dasdload";
    std::string path = "";

    ui.progressBar->setVisible(true);
    mExecutor = new UtilityExecutor();
    UtilityRunner * runner = new UtilityRunner(mExecutor);
    runner->start();
    UtilityRunner * errorRunner = new UtilityRunner(mExecutor);
    errorRunner->start();
    mExecutor->run(command, path, parameters, runner, errorRunner);

    connect(runner, SIGNAL(valueChanged(int)), this, SLOT(runnerValueChanged(int)));
    connect(runner, SIGNAL(maximumChanged(int)), this, SLOT(runnerMaximumChanged(int)));
    connect(runner, SIGNAL(error(QString)), this, SLOT(errorRunner(QString)));
    connect(errorRunner, SIGNAL(valueChanged(int)), this, SLOT(runnerValueChanged(int)));
    connect(errorRunner, SIGNAL(maximumChanged(int)), this, SLOT(runnerMaximumChanged(int)));
    connect(errorRunner, SIGNAL(error(QString)), this, SLOT(errorRunner(QString)));
    ui.runButton->setText("Stop");
}

void DasdLoad::browseCtlClicked()
{
    QString s = QFileDialog::getOpenFileName(this,"Browse for input file path",ui.path->text());
    ui.control->setText(s);
}

void DasdLoad::browseOutClicked()
{
    QString s = QFileDialog::getExistingDirectory(this,"Browse for output file path",ui.path->text());
    ui.path->setText(s);
}

void DasdLoad::runnerMaximumChanged(int maximum)
{
    ui.progressBar->setMaximum(maximum);
}

void DasdLoad::runnerValueChanged(int value)
{
    if (value > 0)
        ui.progressBar->setValue(value);
    else
        ui.progressBar->setValue(ui.progressBar->maximum());
}

void DasdLoad::runnerError(const QString& line)
{
    Tokenizer::handle pos, lastPos;
    std::string word = StringTokenizer::getFirstWord(line.toStdString(), pos, lastPos, " \t\n");
    outDebug(3, std::cout << "runnerError " << word << std::endl);
    if (word.compare(0,5,"IPOS=") == 0)
        return;
    if (word == "HHCDL016I")
    {
        emit output(line);
        ui.progressBar->setValue(ui.progressBar->maximum());
        if (mEnded)
            return;
        mEnded = true;
        QMessageBox::information(this, "dasdload", "Disk creation successfully completed!",
                QMessageBox::Ok,
                QMessageBox::NoButton);
        ui.runButton->setText("Run");
        return;
    }
    emit output(line);
}


