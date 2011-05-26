/*
 *  File: FreeConfigurationEditor.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Apr 19, 2011
 *
 *  Copyright (c) 2011 Jacob Dekel.
 *  Adapted from Nokia sample CodeEditor -
 *  (Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies))
 *  $Id: TerminalProperties.h 67 2010-09-13 14:06:31Z dekeljacob $
 *
 *	This object manages the terminal configuration properties
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

#ifndef FREECONFIGURATIONEDITOR_H
#define FREECONFIGURATIONEDITOR_H

#include <QPlainTextEdit>

class ConfigurationFreeEditor : public QPlainTextEdit
{
	Q_OBJECT
public:
	explicit ConfigurationFreeEditor(QWidget *parent = 0);
	void lineNumberAreaPaintEvent(QPaintEvent *event);
	int lineNumberAreaWidth();

protected:
	 void resizeEvent(QResizeEvent *event);

 private slots:
	 void updateLineNumberAreaWidth(int newBlockCount);
	 void highlightCurrentLine();
	 void updateLineNumberArea(const QRect &, int);

 private:
	 QWidget *mLineNumberArea;
};

class LineNumberArea : public QWidget
 {
 public:
	 LineNumberArea(ConfigurationFreeEditor *editor) : QWidget(editor) {
		 freeConfigurationEditor = editor;
	 }

	 QSize sizeHint() const {
		 return QSize(freeConfigurationEditor->lineNumberAreaWidth(), 0);
	 }

 protected:
	 void paintEvent(QPaintEvent *event) {
		 freeConfigurationEditor->lineNumberAreaPaintEvent(event);
	 }

 private:
	 ConfigurationFreeEditor *freeConfigurationEditor;
 };

#endif // FREECONFIGURATIONEDITOR_H
