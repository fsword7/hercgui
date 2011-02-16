/*
 * LogsWidget.cpp
 *
 *  Created on: Jul 2, 2010
 *      Author: yakov
 */
/*
 *  File:       LogsWidget.cpp
 *
 *  Author:     Jacob Dekel
 *  Created on: July 1, 2010
 *
 *  Copyright (c) 2010 Jacob Dekel
 *  $Id:
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

#include "LogWidget.h"
#include "Preferences.h"
#include "HerculesStudio.h"

#include <QTextEdit>
#include <QTabWidget>
#include <QFile>
#include <QTextBlock>
#include <QTextStream>

#include <fcntl.h>
#include <time.h>
#ifndef hFramework
#include <sys/time.h>
#endif


#define for_each_log \
    for (int for_loop_i=0; for_loop_i<2; for_loop_i++)
#define current_log mLogs[for_loop_i]

PlainLogWidget::PlainLogWidget(QWidget * parent)
:QTextEdit(parent)
{
}

void PlainLogWidget::append(const QString & text)
{
    if (Preferences::getInstance().logTimestamp())
    {
        getTimeStamp();
    }
    else
        mTimeStamp[0] = '\0';
    QString s = text;
    if (text.toAscii().data()[0] == '<')
    s = text.mid(24);

    QColor green(10,120,10), yellow(215,201,45), red(240,20,20);
    QColor keepC = textColor();
    if (text.left(3).compare(QString("HHC")) == 0)
    {
		if (text.mid(8,1).compare(QString("I")) == 0)
			setTextColor(green);
		else if (text.mid(8,1).compare(QString("W")) == 0)
			setTextColor(yellow);
		else if (text.mid(8,1).compare(QString("E")) == 0)
			setTextColor(red);
	}
	if (QTextEdit::document()->blockCount()%20 == 0)
		writeToFile();
	QTextEdit::append(mTimeStamp + s);
	setTextColor(keepC);
}

void PlainLogWidget::getTimeStamp()
{
    struct tm *current;
    time_t now;
    time(&now);
    current = localtime(&now);

    strftime(mTimeStamp, 255, "%H:%M:%S", current); //"%m-%d-%y %H:%M:%S"
    memcpy(mTimeStamp+8, " ", 2);
}

bool PlainLogWidget::empty()
{
    return this->document()->isEmpty();
}

QString PlainLogWidget::toPlainText()
{
    return QTextEdit::toPlainText();
}

void PlainLogWidget::clear()
{
    QTextEdit::clear();
}

bool PlainLogWidget::isOSLog()
{
    return false;
}

void PlainLogWidget::writeToFile()
{
	QString filename="/home/yakov/Desktop/Hercules.log";
	QFile file(filename);
	file.open(QIODevice::Append | QIODevice::Text);
	QTextStream out(&file);

	QTextDocument *oldDocument = QTextEdit::document();
	QTextBlock block = oldDocument->begin();
	while(block != oldDocument->end())
	{
		out << block.text().toAscii().data() << "\n";
		block=block.next();
	}


	QTextDocument *newBlock = new QTextDocument(this);
	QTextEdit::setDocument(newBlock);
}


LogWidget::LogWidget(QWidget * parent)
: PlainLogWidget(NULL)
{
    mTabWidget = new QTabWidget(parent);
    mLogs[0] = new QTextEdit();
    mLogs[1] = new QTextEdit();
    mTabWidget->addTab(mLogs[0], "Hercules");
    mTabWidget->addTab(mLogs[1], "OS");
    for_each_log
    {
        current_log->setReadOnly(true);
        current_log->setVisible(true);
    }
    //const QColor black(0,0,0);
    //const QColor white(255,255,255);
    //QBrush blackBrush(black);
    //QPalette blackPalette(white, black);
    //mLogs[1]->setPalette(blackPalette);
    //mLogs[1]->setAutoFillBackground(true);
}

LogWidget::~LogWidget()
{
}

QTabWidget * LogWidget::tabWidget()
{
    return mTabWidget;
}
void LogWidget::clear()
{
    for_each_log
        current_log->clear();
}

void LogWidget::setReadOnly(bool ro)
{
    for_each_log
        current_log->setReadOnly(ro);
}

void LogWidget::append(const QString & text)
{
    if (Preferences::getInstance().logTimestamp())
    {
        getTimeStamp();
    }
    else
        mTimeStamp[0] ='\0';
    QString s = text;
    if (text.toAscii().data()[0] == '<')
        s = text.mid(24);
    if (s.left(3).compare("HHC") != 0)
    {
        mLogs[1]->append(mTimeStamp + s);
    }
    else
    {
        QColor green(10,120,10), yellow(215,201,45), red(240,20,20);
        QColor keepC = mLogs[0]->textColor();
        if (text.left(3).compare(QString("HHC")) == 0)
        {
            if (text.mid(8,1).compare(QString("I")) == 0)
                mLogs[0]->setTextColor(green);
            else if (text.mid(8,1).compare(QString("W")) == 0)
                mLogs[0]->setTextColor(yellow);
            else if (text.mid(8,1).compare(QString("E")) == 0)
                mLogs[0]->setTextColor(red);
        }
        mLogs[0]->append(mTimeStamp + s);
        mLogs[0]->setTextColor(keepC);
    }
}

void LogWidget::setFont(const QFont & font)
{
    for_each_log
        current_log->setFont(font);
}

void LogWidget::setTextBackgroundColor  (const QColor &color)
{
    for_each_log
        current_log->setTextBackgroundColor(color);
}

void LogWidget::setTextColor  (const QColor &color)
{
    for_each_log
        current_log->setTextColor(color);
}

QString LogWidget::toPlainText()
{
    QString ret;
    for_each_log
        ret += current_log->toPlainText();
    return ret;
}

bool LogWidget::empty()
{
    return mLogs[0]->document()->isEmpty() && mLogs[1]->document()->isEmpty();
}

bool LogWidget::isOSLog()
{
    return (mTabWidget->currentIndex() == 1);
}
