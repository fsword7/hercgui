/*
 *  File:       UtilityRunner.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *
 *	This object tracks on running utilites and updates their status
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

#include <QThread>

#ifndef UTILIYRUNNER_H_
#define UTILIYRUNNER_H_

class UtilityRunner : public QThread
{
    Q_OBJECT
public:
    UtilityRunner(FILE * file);
    virtual ~UtilityRunner();
    void run();

signals:
    void ended();
    void valueChanged(int value);
    void maximumChanged(int maximum);
    void error(QString errorLine);
    void started(int);

private:
    FILE * mFile;

    void updateStatus(const std::string& line);
};

#endif /* UTILIYRUNNER_H_ */
