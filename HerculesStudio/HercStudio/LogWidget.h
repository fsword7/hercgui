/*
 *  File:       LogsWidget.h
 *
 *  Author:     Jacob Dekel
 *  Created on: July 1, 2010
 *
 *  Copyright (c) 2010 Jacob Dekel
 *  $Id:
 *
 *	Multiple QTextEdit wrapper with selection tab
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

#ifndef LOGSWIDGET_H_
#define LOGSWIDGET_H_

#include <QTextEdit>
#include <QResizeEvent>
#include <QString>

class QTabWidget;
class PlainLogWidget : public QTextEdit
{
    Q_OBJECT
public:
	enum WriteType
	{
		MenuCommand,
		Overflow,
		Shutdown
	} ;
	PlainLogWidget(QWidget * parent, const char * suffix="");
	virtual void append(const QByteArray & text);
	virtual QString toPlainText();
	virtual bool isOSLog();
	virtual void writeToFile(WriteType type);
	virtual void preferencesChanged();
	virtual void setIpled(bool ipled);
    virtual void clear();
    virtual bool textIsSelected();  // text to be copied available
    virtual QString copySelectedText(); // copies selected text to clipboard


protected:

	void  getTimeStamp(bool withDate);
	void  setLogFileName (QString& suffix);
	char  mTimeStamp[64];
	int   mLogFileLines;
	bool  mIpled;
	bool  mSaveLog;
	QString mLogFileName;
	QColor mGreen, mYellow, mRed, mBlack, mWhite;
	long  mLinesWritten;
	bool  mDarkBackground;
    bool  mSelectedTextExists;

private slots:
    void  keepSelection(bool selected);

signals:
    void logCopy();
};


class LogWidget : public PlainLogWidget
{
	Q_OBJECT
public:
	LogWidget(QWidget * parent);
	virtual ~LogWidget();

	int  active();
	void setActive(int i);
	void setReadOnly(bool readOnly);
	void setFont(const QFont & font);
	void setTextBackgroundColor  (const QColor &);
	void setTextColor (const QColor &);
	QTabWidget * tabWidget();
	virtual bool empty();
	virtual void clear();
    virtual bool textIsSelected();
    virtual QString copySelectedText();

	virtual void append(const QByteArray & text);
	virtual QString toPlainText();
	virtual bool isOSLog();
	virtual void writeToFile(PlainLogWidget::WriteType writeType);
	virtual void preferencesChanged();
	virtual void setIpled(bool ipled);

private:
	const int cHercIndex;
	const int cOsIndex;
	QTabWidget * mTabWidget;
	PlainLogWidget * mLogs[2];
	int	mActive;
	bool mIpled;
};

#endif /* LOGSWIDGET_H_ */
