/*
 *  File:       Regs32.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *
 *	This object is a base class for all registers presentation
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

#ifdef REGS_H_
#define REGS_H_

#include <QtGui/QWidget>
#include <QLabel>
#include <QCheckBox>
#include "StatusUpdateCollector.h"

class Regs : public QWidget, public StatusUpdateCollector
{
  Q_OBJECT
public:
  Regs(QWidget * parent);
  virtual ~Regs();

  virtual void setActiveAll(bool active) = 0;
  void notify(const std::string& statusLine) = 0;

protected:
  QWidget * mParent;

  QCheckBox regCheck;

private slots:
  void regCheckChanged(int);
};

#endif /* REGS32_H_ */
