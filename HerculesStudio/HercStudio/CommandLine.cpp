/*
 *  File:       CommandLine.cpp
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009-2013 Jacob Dekel
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

#include "CommandLine.h"
#include "Preferences.h"

#include <QKeyEvent>

#include <iostream>

CommandLine::CommandLine(QWidget * parent)
 : QLineEdit(parent), mHistoryPtr(-1)
{
    setMaxLength(512);
    setFont();

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
    hOutDebug(4,"text1:"<<  this->text().toStdString());
    mHistory.insert(mHistory.begin(),text1);
    while (mHistory.size() > 100)
        mHistory.erase(mHistory.end()-1);
    mHistoryPtr = -1;
    this->setText("");
}

void CommandLine::keyPressEvent(QKeyEvent * event)
{
    hOutDebug(4, "key:" << event->key() << " " << (event->key() == Qt::Key_Up));
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
        if (mHistoryPtr >= (signed) mHistory.size()) mHistoryPtr = mHistory.size()-1;
        setLine();
        break;
    case Qt::Key_C:
        if (event->modifiers() & Qt::CTRL)
        emit ctrl_c();
        break;

    default:
        break;
    }
}

void CommandLine::setLine()
{
    hOutDebug(4,"setLine:" << mHistoryPtr << " size:" << mHistory.size() << " " << mHistory[mHistoryPtr].toStdString());
    if (mHistoryPtr < -1 || mHistoryPtr >= (signed) mHistory.size())
        return;
    if (mHistoryPtr != -1)
        this->setText(mHistory[mHistoryPtr]);
    else
        this->setText("");
}

void CommandLine::setFont()
{
	Preferences& pref = Preferences::getInstance();
	QFont font(pref.fontName(Preferences::CommandFontObject).c_str(),
			pref.fontSize(Preferences::CommandFontObject),
			(pref.fontIsBold(Preferences::CommandFontObject) ? QFont::Bold : QFont::Normal),
			pref.fontIsItalic(Preferences::CommandFontObject));

    font.setStyleHint(QFont::Courier);
    QLineEdit::setFont(font);
}

bool CommandLine::empty()
{
	return (!(mHistory.size() > 0));
}

void CommandLine::save()
{
    Preferences& pref = Preferences::getInstance();
    while(mHistory.size() > 30)
        mHistory.pop_back();
    pref.setHistory(mHistory);
}

void CommandLine::restore()
{
    Preferences& pref = Preferences::getInstance();
    pref.getHistory(mHistory);

}
