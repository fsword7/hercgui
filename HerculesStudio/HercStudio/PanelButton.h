/*
 *  File:       PanelButton.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id$
 *
 *	This object implements one panel button
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

#ifndef PANELBUTTON_H_
#define PANELBUTTON_H_

#include <QPushButton>
#include <QPaintEvent>

class PanelButton : public QPushButton
{
    Q_OBJECT
public:
    PanelButton(const QString& upButtonName, const QString& downButtonName, QWidget * parent=NULL);
    virtual ~PanelButton();
    virtual void paintEvent(QPaintEvent *pEvent);

public slots:
    void pressedSignal();
    void releasedSignal();

private:
    bool mDown;
    QString mUpButtonName, mDownButtonName;
};

#endif /* PANELBUTTON_H_ */
