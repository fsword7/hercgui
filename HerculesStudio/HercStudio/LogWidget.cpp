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
#include <QTabBar>

#include <fcntl.h>
#include <sys/time.h>


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
		timeStamp[0] = '\0';
	QString s = text;
	if (text.toAscii().data()[0] == '<')
	s = text.mid(24);
	QTextEdit::append(timeStamp + s);
}

void PlainLogWidget::getTimeStamp()
{
	static struct timeval  now;
    static time_t          tt;

	gettimeofday( &now, NULL );
	tt = now.tv_sec;
	strncpy( timeStamp, ctime(&tt)+11, 8 );
	memcpy(timeStamp+8, " ", 2);
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
		timeStamp[0] ='\0';
	QString s = text;
	if (text.toAscii().data()[0] == '<')
		s = text.mid(24);
	if (s.left(9).compare("HHC00001I") == 0)
	{
		s = s.mid(9);
		mLogs[1]->append(timeStamp + s);
	}
	else
	{
		mLogs[0]->append(timeStamp + s);
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
