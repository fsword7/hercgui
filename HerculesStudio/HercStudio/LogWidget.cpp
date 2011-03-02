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

PlainLogWidget::PlainLogWidget(QWidget * parent, const char * suffix)
:QTextEdit(parent)
{
	mSaveLog = Preferences::getInstance().autosaveLog();
	mLogFileLines = Preferences::getInstance().logFileLines();
	QString parm(suffix);
	setLogFileName(parm);
}

void PlainLogWidget::setLogFileName (QString& suffix)
{
	getTimeStamp(true);
	mLogFileName = Preferences::getInstance().logsDir().c_str();
	mLogFileName += "/hercules";
	mLogFileName += mTimeStamp;
	if (suffix.length() != 0)
	{
		mLogFileName += ".";
		mLogFileName += suffix;
  	}
	mLogFileName += ".log";
}

void PlainLogWidget::append(const QByteArray & text)
{
    if (Preferences::getInstance().logTimestamp())
    {
        getTimeStamp(false);
    }
    else
        mTimeStamp[0] = '\0';
    QByteArray s = text;
    if (text.data()[0] == '<')
		s = text.mid(24);

    QColor green(10,120,10), yellow(215,201,45), red(240,20,20);
    QColor keepC = textColor();
    if (strncmp(text.data(),"HHC",3) == 0)
    {
		if (text[8] == 'I')
			setTextColor(green);
		else if (text.mid(8,1) =="W")
			setTextColor(yellow);
		else if (text.mid(8,1) =="E")
			setTextColor(red);
	}
	QTextEdit::append(QByteArray(mTimeStamp) + s);
	setTextColor(keepC);
	if (QTextEdit::document()->blockCount()%mLogFileLines == 0)
	{
		writeToFile(false);
	}
}

void PlainLogWidget::getTimeStamp(bool withDate)
{
    struct tm *current;
    time_t now;
    time(&now);
    current = localtime(&now);

    if (withDate)
    	strftime(mTimeStamp, 255, "%Y-%m-%d-%H-%M-%S", current);
    else
    {
    	strftime(mTimeStamp, 255, "%H:%M:%S", current); //"%m-%d-%y %H:%M:%S"
    	memcpy(mTimeStamp+8, " ", 2);
    }
}

QString PlainLogWidget::toPlainText()
{
	return QTextEdit::toPlainText();
}

bool PlainLogWidget::isOSLog()
{
    return false;
}

void PlainLogWidget::writeToFile(bool menuCommand)
{
	QTextDocument *oldDocument = QTextEdit::document();
	if (mSaveLog || menuCommand)
	{
		QFile file(mLogFileName);

		if (!file.open(QIODevice::Append | QIODevice::Text))
		{
			hOutDebug(0,"Failed to open file " << mLogFileName.toStdString());
			return ;
		}
		QTextStream out(&file);

		QTextBlock block = oldDocument->begin();
		while(block != oldDocument->end())
		{
			out << block.text().toAscii().data() << "\n";
			block=block.next();
		}

		if (!menuCommand)
		{
			getTimeStamp(true);
			QString sepLine = "---------------- log was saved at ";
			sepLine += mTimeStamp;
			sepLine += "-------------------------";
			QTextEdit::append(sepLine);
		}
	}
	oldDocument->clear();
	QTextDocument *newBlock = new QTextDocument(this);
	QTextEdit::setDocument(newBlock);
}

void PlainLogWidget::preferencesChanged()
{
	mLogFileLines = Preferences::getInstance().logFileLines(); // might have been updated
}

LogWidget::LogWidget(QWidget * parent)
: PlainLogWidget(NULL), cHercIndex(0), cOsIndex(1)
{
    mTabWidget = new QTabWidget(parent);
    mLogs[cHercIndex] = new PlainLogWidget(this, "hercules");
    mLogs[cOsIndex] = new PlainLogWidget(this, "os");
    mTabWidget->addTab(mLogs[cHercIndex], "Hercules");
    mTabWidget->addTab(mLogs[cOsIndex], "OS");
    for_each_log
    {
        current_log->setReadOnly(true);
        current_log->setVisible(true);
    }
    //const QColor black(0,0,0);
    //const QColor white(255,255,255);
    //QBrush blackBrush(black);
    //QPalette blackPalette(white, black);
    //mLogs[cOsIndex]->setPalette(blackPalette);
    //mLogs[cOsIndex]->setAutoFillBackground(true);
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

void LogWidget::append(const QByteArray & text)
{
    if (Preferences::getInstance().logTimestamp())
    {
        getTimeStamp(false);
    }
    else
        mTimeStamp[0] ='\0';
    QByteArray s = text;
    if (text.data()[0] == '<')
        s = text.mid(24);
    if (strncmp(text.data(),"HHC",3) == 0)
    {
        mLogs[cOsIndex]->append(QByteArray(mTimeStamp) + s);
    }
    else
    {
        QColor green(10,120,10), yellow(215,201,45), red(240,20,20);
        QColor keepC = mLogs[cHercIndex]->textColor();
        if (strncmp(text.data(),"HHC",3) == 0)
        {
            if (text[8] == 'I')
                mLogs[cHercIndex]->setTextColor(green);
            else if (text[8] == 'W')
                mLogs[cHercIndex]->setTextColor(yellow);
            else if (text[8] == 'E')
                mLogs[cHercIndex]->setTextColor(red);
        }
        mLogs[cHercIndex]->append(QByteArray(mTimeStamp) + s);
        mLogs[cHercIndex]->setTextColor(keepC);
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
    return mLogs[cHercIndex]->document()->isEmpty() && mLogs[cOsIndex]->document()->isEmpty();
}

bool LogWidget::isOSLog()
{
    return (mTabWidget->currentIndex() == 1);
}

void LogWidget::writeToFile(bool menuCommand)
{
	for_each_log
		current_log->writeToFile(menuCommand);
}

void LogWidget::preferencesChanged()
{
	for_each_log
		current_log->preferencesChanged();
}
