/*
 *  File: ReaderFileListView.h
 *
 *  Author:     Jacob Dekel
 *  Created on: Aug 7, 2009
 *
 *  Copyright (c) 2009-2013 Jacob Dekel
 *  $Id$
 *
 *	This object presents a file list that can be managed with the mouse
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

#ifndef READERFILELISTVIEW_H_
#define READERFILELISTVIEW_H_

#include <QListView>

class ReaderFileListView : public QListView
{
public:
    ReaderFileListView(QWidget * parent = 0 );
    virtual ~ReaderFileListView();
    void setReaderParent(QWidget * parent);

private:
    QWidget * mReaderParent;

    void dropEvent ( QDropEvent * event ) ;
};

#endif /* READERFILELISTVIEW_H_ */
