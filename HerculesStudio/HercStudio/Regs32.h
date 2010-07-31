/*
 *  File:       Regs32.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id: Regs32.h 34 2009-11-07 06:15:58Z jacob $
 *
 *	This object implements the 32-bit registers view
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

#ifndef REGS_H_
#define REGS_H_

#include "StatusUpdateCollector.h"

#include <QtGui/QWidget>
#include <QLabel>
#include <QCheckBox>

#include <string>

class Regs32 : public QWidget, public StatusUpdateCollector
{
  Q_OBJECT
public:
  Regs32(QWidget * parent, std::string command);
  virtual ~Regs32();

  void setFont();
  virtual void setActive(bool active);
  bool notify(const std::string& statusLine);

  static std::string gCommand;
  static std::string fCommand;
  static std::string cCommand;
  static std::string aCommand;

protected:
  QWidget * mParent;
  QLabel mLine1, mLine2, mLine3, mLine4;  // subclass can add more
  std::string mCommand;
};

#endif /* REGS_H_ */
