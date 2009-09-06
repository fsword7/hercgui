/*
 *  File:       CommandLine.cpp
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

#include "CommandLine.h"

#include <QKeyEvent>

#include <iostream>

CommandLine::CommandLine(QWidget * parent)
 : QLineEdit(parent), mHistoryPtr(-1)
{
    setMaxLength(512);

}

CommandLine::~CommandLine()
{
}

QSize CommandLine::sizeHint()
{
    return QSize(1024, 50);
}

void CommandLine::enterPressed(const QString & text1)
{
    outDebug(4,std::cout << "text1:"<<  this->text().toStdString() << std::endl);
    std::string str(text1.toStdString());
    history.insert(history.begin(),str);
    while (history.size() > 100)
        history.erase(history.end()-1);
    mHistoryPtr = -1;
    this->setText("");
}

void CommandLine::keyPressEvent(QKeyEvent * event)
{
    outDebug(4, std::cout << "key:" << event->key() << " " << (event->key() == Qt::Key_Up) << std::endl);
    QLineEdit::keyPressEvent(event);
    switch (event->key())
    {
    case Qt::Key_Enter:
    case Qt::Key_Return:
        enterPressed(this->text());
        break;
    case Qt::Key_Down:
        if (mHistoryPtr >= 0) mHistoryPtr--;
        setLine();
        break;
    case Qt::Key_Up:
        mHistoryPtr++;
        if (mHistoryPtr >= (signed) history.size()) mHistoryPtr = history.size()-1;
        setLine();
        break;
    default:
        break;
    }
}

void CommandLine::setLine()
{
    outDebug(4,std::cout << "setLine:" << mHistoryPtr << " size:" << history.size() << " " << history[mHistoryPtr].c_str() << std::endl);
    if (mHistoryPtr < -1 || mHistoryPtr >= (signed) history.size())
        return;
    if (mHistoryPtr != -1)
        this->setText(history[mHistoryPtr].c_str());
    else
        this->setText("");
}
