/*
 *  File:       CommandLine.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009-2013 Jacob Dekel
 *  $Id$
 *
 *	This object manages the command line widget
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

#ifndef COMMANDLINE_H_
#define COMMANDLINE_H_

#include "HerculesStudio.h"

#include <QtGui/QWidget>
#include <QLineEdit>

class CommandLine : public QLineEdit
{
  Q_OBJECT
public:
  CommandLine(QWidget * parent=NULL );
  virtual ~CommandLine();
  void setFont();
  bool empty();
  void save();
  void restore();

  using QLineEdit::sizeHint;
  QSize sizeHint();

protected:
  virtual void keyPressEvent ( QKeyEvent * event );

private slots:
    void enterPressed(const QString & text);

signals:
    void ctrl_c();

private:
  QVector<QString> mHistory;
  int mHistoryPtr;

  void setLine();
};

#endif /* COMMANDLINE_H_ */
