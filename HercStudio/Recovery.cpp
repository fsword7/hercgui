/*
 *  File:       Recovery.cpp
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

#include "Recovery.h"
#include "NamedPipe.h"

#include <sstream>

Recovery::Recovery(QWidget *parent)
    : QDialog(parent)
{
  ui.setupUi(this);
  mHerculesConf = readHerculesConf();

  ui.configurationFilename->setText(mHerculesConf.c_str());
  std::stringstream ss;
  ss << NamedPipe::getInstance().getHerculesPid();
  ui.processNumber->setText(ss.str().c_str());

    connect(ui.exitButton, SIGNAL(clicked()), this , SLOT(exit()));
    connect(ui.attachButton, SIGNAL(clicked()), this , SLOT(recover()));
    connect(ui.skipButton, SIGNAL(clicked()), this , SLOT(skip()));
}

Recovery::~Recovery()
{

}

void Recovery::exit()
{
    mHerculesConf = "!";
    this->close();
}

void Recovery::skip()
{
    mHerculesConf = "";
    this->close();
}


void Recovery::recover()
{
    this->close();
}

std::string Recovery::getHerculesConf()
{
    return mHerculesConf;
}

std::string Recovery::readHerculesConf()
{
    std::string cmdLine = NamedPipe::getInstance().getHerculesCommandline();
    int ind = cmdLine.find("-d -f");
    std::string past_f = cmdLine.substr(ind+6);
    ind = past_f.find(" ");
    return past_f.substr(0,ind);
}
