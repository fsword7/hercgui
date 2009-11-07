/*
 *  File: NewReaderFile
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009 Jacob Dekel
 *  $Id: NewReaderFile.h 34 2009-11-07 06:15:58Z jacob $
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

#ifndef NEWREADERFILE_H_
#define NEWREADERFILE_H_

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class NewReaderFile  : public QDialog
{
    Q_OBJECT
public:
    NewReaderFile(QWidget * parent = 0);
    virtual ~NewReaderFile();

private:
    QLabel    mLabel;
    QLineEdit mFileName;
    QPushButton mBrowseButton;
    QPushButton mOKButton;
    QPushButton mCancelButton;

    void initialize();

private slots:
    void doCancel();
    void doOK();
    void doBrowse();

signals:
    void okPressed(QString&);
};

#endif /* NEWREADERFILE_H_ */
