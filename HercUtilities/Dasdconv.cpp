/*
 *  File:       Dasdconv.cpp
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

#include "Dasdconv.h"
#include "HerculesStudio.h"
#include "UtilityExecutor.h"
#include "UtilityRunner.h"

#include <QFileDialog>
#include <QMessageBox>

#include <csignal>


Dasdconv::Dasdconv(QWidget *parent)
    : QDialog(parent), mPid(-1)
{
  ui.setupUi(this);

    connect(ui.runButton, SIGNAL(clicked()), this, SLOT(runClicked()));
    connect(ui.exitButton, SIGNAL(clicked()), this, SLOT(exitClicked()));
    connect(ui.browseFileButton, SIGNAL(clicked()), this, SLOT(browseFileClicked()));
    connect(ui.browseOutButton, SIGNAL(clicked()), this, SLOT(browseOutClicked()));

}

Dasdconv::~Dasdconv()
{

}

void Dasdconv::runClicked()
{
    if (mPid > 0)
    {
        kill(mPid, SIGKILL);
        QMessageBox::warning(this, "dasdconv",
                                            "dasdconv operation was aborted at user's request",
                                            QMessageBox::Ok,
                                            QMessageBox::NoButton);
        mPid=-1;
        ui.runButton->setText("Ok");
        return;
    }
    if (ui.infile->text().isEmpty())
    {
        QMessageBox::warning(this, "dasdconv", "Please specify input file to convert",
                QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }
    if (!QFile::exists(ui.infile->text()))
    {
        QMessageBox::warning(this, "dasdconv", "Input file does not exist",
                QMessageBox::Ok, QMessageBox::NoButton);
        return;

    }
    if (ui.filename->text().isEmpty())
    {
        QMessageBox::warning(this, "dasdconv", "Please specify ckd file to create",
                QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }
    if (ui.path->text().isEmpty())
    {
        QMessageBox::warning(this, "dasdconv", "Please specify output directory",
                QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }
    if (!QFile::exists(ui.path->text()))
    {
        QMessageBox::warning(this, "dasdconv", "Output directory does not exist",
                QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }
    QFileInfo dirInfo(ui.path->text());
    if (!dirInfo.isDir())
    {
        QMessageBox::warning(this, "dasdconv", "Output path is not a directory",
                QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }
    std::vector<std::string> parameters;

    if (ui.replaceCheckbox->isChecked())
        parameters.push_back("-r");
    if (ui.lfsCheckbox->isChecked())
        parameters.push_back("-lfs");

    parameters.push_back(ui.infile->text().toStdString());

    std::string fullPath;
    if (ui.path->text().length() > 0)
    {
        fullPath = ui.path->text().toStdString();
        fullPath += "/";
    }
    fullPath += ui.filename->text().toStdString();

    parameters.push_back(fullPath);
    parameters.push_back("EXTERNALGUI");
    std::string command = "dasdconv";

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
    connect(runnerError, SIGNAL(valueChanged(int)), this, SLOT(runnerValueChanged(int)));
    connect(runnerError, SIGNAL(maximumChanged(int)), this, SLOT(runnerMaximumChanged(int)));
    connect(runnerError, SIGNAL(error(QString)), this, SLOT(runnerError(QString)));
    ui.runButton->setText("Stop");

}


void Dasdconv::exitClicked()
{
    deleteLater();
}

void Dasdconv::browseFileClicked()
{
    QString s = QFileDialog::getOpenFileName(this,"Browse for input disk",ui.filename->text());
    ui.infile->setText(s);
    if (ui.path->text().isEmpty())
    {
        QDir d("/");
        std::string path = d.absoluteFilePath(s).toStdString();
        int lastSlash = s.toStdString().find_last_of('/');
        ui.path->setText(path.substr(0,lastSlash).c_str());
    }

}
void Dasdconv::browseOutClicked()
{
    QString s = QFileDialog::getExistingDirectory(this,"Browse for output file path",ui.path->text());
    ui.path->setText(s);
}


void Dasdconv::runnerMaximumChanged(int maximum)
{
    ui.progressBar->setMaximum(maximum);
}

void Dasdconv::runnerValueChanged(int value)
{
    outDebug(5, std::cout << "dasdconv got value changed " << value << std::endl);
    if (value > 0)
        ui.progressBar->setValue(value);
    else
    {
        ui.progressBar->setValue(ui.progressBar->maximum());
    }
}

void Dasdconv::runnerError(const QString& line)
{
    outDebug(2, std::cout << "Dasdconv::runnerError " << line.toStdString() << std::endl);
    emit output(line);
    if (line.toStdString().compare(0,28,"DASD conversion successfully") == 0)
    {
        ui.runButton->setText("Run");
        mPid=-1;
        QMessageBox::information(this, "dasdload", "Disk conversion successfully completed!",
                QMessageBox::Ok,
                QMessageBox::NoButton);

    }
}



