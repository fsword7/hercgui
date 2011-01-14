/*
 *  File:       Dasdls.cpp
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id$
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
#include "HerculesStudio.h"
#include "UtilityRunner.h"
#include "Preferences.h"
#include "StringTokenizer.h"


#include <QFileDialog>
#include <QMessageBox>

#include <csignal>

Dasdls::Dasdls(QWidget *parent)
    : GenericUtility("dasdls",parent)
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
    if (!runOrStopClicked())
    {
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
    std::string command = "dasdls";

    execute(command, Preferences::getInstance().hercDir(), parameters);

    connect(mErrorRunner, SIGNAL(valueChanged(int)), this, SLOT(runnerValueChanged(int)));
    connect(mErrorRunner, SIGNAL(maximumChanged(int)), this, SLOT(runnerMaximumChanged(int)));
    ui.runButton->setText("Stop");
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
	mMaximum = maximum;
    outDebug(2, std::cout << "dasdls maximum is " << maximum << std::endl);
}

void Dasdls::runnerValueChanged(int value)
{
	hOutDebug(2,"value:" << value);
	return;
}

void Dasdls::finishedSlot()
{
	mExecutor = NULL;
	if (mFinishedOK)
	{
		QMessageBox::information(this, "dasdls", "Disk listing successfully completed!",
			QMessageBox::Ok,
			QMessageBox::NoButton);
	}
	ui.runButton->setText("Run");
	deleteLater();
}
